#include <iostream>
#include "parser.h"

using namespace std;

static void rndr_blockcode(struct buf *ob, struct buf *text, void *opaque);
static void rndr_blockquote(struct buf *ob, struct buf *text, void *opaque);
static void rndr_header(struct buf *ob, struct buf *text, int level, void *opaque);
static void rndr_list(struct buf *ob, struct buf *text, int flags, void *opaque);
static void rndr_listitem(struct buf *ob, struct buf *text, int flags, void *opaque);
static void rndr_paragraph(struct buf *ob, struct buf *text, void *opaque);
static int rndr_codespan(struct buf *ob, struct buf *text, void *opaque);
static int rndr_double_emphasis(struct buf *ob, struct buf *text, char c, void *opaque);
static int rndr_emphasis(struct buf *ob, struct buf *text, char c, void *opaque);
static int rndr_triple_emphasis(struct buf *ob, struct buf *text, char c, void *opaque);
static int rndr_linebreak(struct buf *ob, void *opaque);
static int rndr_link(struct buf *ob, struct buf *link, struct buf *title, struct buf *content, void *opaque);
static void rndr_normal_text(struct buf *ob, struct buf *text, void *opaque);

struct mkd_renderer mkd_callbacks = {
	/* document-level callbacks */
	NULL,                 // prolog
	NULL,                 // epilogue

	/* block-level callbacks */
	rndr_blockcode,       // block code
	rndr_blockquote,      // block quote
	NULL,                 // block html
	rndr_header,          // header
	NULL,                 // hrule
	rndr_list,            // list
	rndr_listitem,        // listitem
	rndr_paragraph,       // paragraph
	NULL,                 // table
	NULL,                 // table cell
	NULL,                 // table row

	/* span-level callbacks */
	NULL,                 // autolink
	rndr_codespan,        // codespan
	rndr_double_emphasis, // double emphasis
	rndr_emphasis,        // emphasis
	NULL,                 // image
	rndr_linebreak,       // line break
	rndr_link,            // link
	NULL,                 // raw html tag
	rndr_triple_emphasis, // triple emphasis

	/* low-level callbacks */
	NULL,                 // entity
	rndr_normal_text,     // normal text

	/* renderer data */
	64, // max stack
	"*_",
	NULL // opaque
};

namespace Bypass {

	const static std::string TWO_SPACES = "  ";

	Parser::Parser()
	: pendingSpanElements()
	{

	}

	Parser::~Parser() {

	}

	Document Parser::parse(const char* mkd) {
		document = Document();

		if (mkd) {
			struct buf *ib, *ob;

			ib = bufnew(INPUT_UNIT);
			bufputs(ib, mkd);

			ob = bufnew(OUTPUT_UNIT);

			mkd_callbacks.opaque = this;

			//parse and assemble document
			markdown(ob, ib, &mkd_callbacks);

			if (pendingSpanElements.size() > 0) {
				parsedParagraph(NULL, NULL);
			}

			bufrelease(ib);
			bufrelease(ob);
		}

		return document;
	}

	Document Parser::parse(const string& markdown) {
		return parse(markdown.c_str());
	}

	void Parser::addElement(Element element) {
		document.append(element);
		pendingSpanElements.clear();
	}

	// Block Element Callbacks

	void Parser::parsedBlockcode(struct buf *ob, struct buf *text) {

	}

	void Parser::parsedBlockquote(struct buf *ob, struct buf *text) {

	}

	void Parser::parsedHeader(struct buf *ob, struct buf *text, int level) {
		char levelStr[2];
		snprintf(levelStr, 2, "%d", level);

		Element header;
		header.setType(HEADER);
		header.addAttribute("level", levelStr);
		header.setChildren(pendingSpanElements);
		addElement(header);
	}

	void Parser::parsedList(struct buf *ob, struct buf *text, int flags) {

	}

	void Parser::parsedListItem(struct buf *ob, struct buf *text, int flags) {

	}

	void Parser::parsedParagraph(struct buf *ob, struct buf *text) {
		Element paragraph;
		paragraph.setType(PARAGRAPH);
		paragraph.setChildren(pendingSpanElements);
		addElement(paragraph);
	}

	// Span Element Callbacks

	int Parser::parsedDoubleEmphasis(struct buf *ob, struct buf *text, char c) {
		pendingSpanElements.back().setType(DOUBLE_EMPHASIS);
		return 1;
	}

	int Parser::parsedEmphasis(struct buf *ob, struct buf *text, char c) {
		pendingSpanElements.back().setType(EMPHASIS);
		return 1;
	}

	int Parser::parsedTripleEmphasis(struct buf *ob, struct buf *text, char c) {
		pendingSpanElements.back().setType(TRIPLE_EMPHASIS);
		return 1;
	}

	int Parser::parsedLink(struct buf *ob, struct buf *link, struct buf *title, struct buf *content) {
		pendingSpanElements.back().setType(LINK);

		if (link && link->size > 0) {
			pendingSpanElements.back().addAttribute("link", std::string(link->data).substr(0, link->size));
		}

		if (title && title->size > 0) {
			pendingSpanElements.back().addAttribute("title", std::string(title->data).substr(0, title->size));
		}

		if (content && content->size) {
			pendingSpanElements.back().addAttribute("content", std::string(content->data).substr(0, content->size));
		}

		return 1;
	}

	int Parser::parsedCodeSpan(struct buf *ob, struct buf *text) {
		if (text && text->size > 0) {
			Element codeSpan;
			codeSpan.setType(CODE_SPAN);
			codeSpan.setText(std::string(text->data).substr(0, text->size));
			pendingSpanElements.push_back(codeSpan);
		}

		return 1;
	}

	/*!
	\brief Erases errant control characters when markdown.c encounters a line break.

	markdown.c interprets "  \n" as a line break, however it leaves the trailing
	spaces associated with the previous span element. This method will erase those
	control characters from the previous element.
	 */
	void Parser::eraseLinebreakControlCharacters() {
		std::string precedingText = pendingSpanElements.back().getText();
		if (precedingText.size() > 2) {
			if (precedingText.substr(precedingText.length() - 2) == TWO_SPACES) {
				pendingSpanElements.back().setText(precedingText.substr(0, precedingText.length() - 2));
			}
		}
	}

	int Parser::parsedLinebreak(struct buf *ob) {
		eraseLinebreakControlCharacters();

		Element lineBreak;
		lineBreak.setType(LINEBREAK);
		pendingSpanElements.push_back(lineBreak);
		return 1;
	}

	// Low Level Callbacks

	void Parser::parsedNormalText(struct buf *ob, struct buf *text) {

		// The parser will spuriously emit a text callback for an empty string
		// that butts up against a span-level element. This will ignore it.

		if (text && text->size > 0) {
			Element normalText;
			normalText.setType(TEXT);
			normalText.setText(std::string(text->data).substr(0, text->size));
			pendingSpanElements.push_back(normalText);
		}
	}

}

// Block Element callbacks

static void rndr_blockcode(struct buf *ob, struct buf *text, void *opaque) {
	((Bypass::Parser*) opaque)->parsedBlockcode(ob, text);
}

static void rndr_blockquote(struct buf *ob, struct buf *text, void *opaque) {
	((Bypass::Parser*) opaque)->parsedBlockquote(ob, text);
}

static void rndr_header(struct buf *ob, struct buf *text, int level, void *opaque) {
	((Bypass::Parser*) opaque)->parsedHeader(ob, text, level);
}

static void rndr_list(struct buf *ob, struct buf *text, int flags, void *opaque) {
	((Bypass::Parser*) opaque)->parsedList(ob, text, flags);
}

static void rndr_listitem(struct buf *ob, struct buf *text, int flags, void *opaque) {
	((Bypass::Parser*) opaque)->parsedListItem(ob, text, flags);
}

static void rndr_paragraph(struct buf *ob, struct buf *text, void *opaque) {
	((Bypass::Parser*) opaque)->parsedParagraph(ob, text);
}

// Span Element callbacks

static int rndr_codespan(struct buf *ob, struct buf *text, void *opaque) {
	return ((Bypass::Parser*) opaque)->parsedCodeSpan(ob, text);
}

static int rndr_double_emphasis(struct buf *ob, struct buf *text, char c, void *opaque) {
	return ((Bypass::Parser*) opaque)->parsedDoubleEmphasis(ob, text, c);
}

static int rndr_emphasis(struct buf *ob, struct buf *text, char c, void *opaque) {
	return ((Bypass::Parser*) opaque)->parsedEmphasis(ob, text, c);
}

static int rndr_triple_emphasis(struct buf *ob, struct buf *text, char c, void *opaque) {
	return ((Bypass::Parser*) opaque)->parsedTripleEmphasis(ob, text, c);
}

static int rndr_linebreak(struct buf *ob, void *opaque) {
	return ((Bypass::Parser*) opaque)->parsedLinebreak(ob);
}

static int rndr_link(struct buf *ob, struct buf *link, struct buf *title, struct buf *content, void *opaque) {
	return ((Bypass::Parser*) opaque)->parsedLink(ob, link, title, content);
}

//	Low Level Callbacks

static void rndr_normal_text(struct buf *ob, struct buf *text, void *opaque) {
	return ((Bypass::Parser*) opaque)->parsedNormalText(ob, text);
}

