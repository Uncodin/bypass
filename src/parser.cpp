#include "parser.h"

using namespace std;

/*
 *	Block Element callbacks
 */

static void rndr_blockcode(struct buf *ob, struct buf *text, void *opaque);

static void rndr_header(struct buf *ob, struct buf *text, int level, void *opaque);

static void rndr_list(struct buf *ob, struct buf *text, int flags, void *opaque);

static void rndr_listitem(struct buf *ob, struct buf *text, int flags, void *opaque);

static void rndr_paragraph(struct buf *ob, struct buf *text, void *opaque);

/*
 *	Span Element callbacks
 */

static int rndr_codespan(struct buf *ob, struct buf *text, void *opaque);

static int rndr_double_emphasis(struct buf *ob, struct buf *text, char c,
		void *opaque);

static int rndr_emphasis(struct buf *ob, struct buf *text, char c,
		void *opaque);

static int rndr_triple_emphasis(struct buf *ob, struct buf *text, char c,
		void *opaque);

static int rndr_linebreak(struct buf *ob, void *opaque);

static int rndr_link(struct buf *ob, struct buf *link, struct buf *title,
			struct buf *content, void *opaque);

/*
 *	Low Level Callbacks
 */

static void rndr_normal_text(struct buf *ob, struct buf *text, void *opaque);

struct mkd_renderer mkd_callbacks = {
	/* document-level callbacks */
	NULL, // prolog
	NULL, // epilogue

	/* block-level callbacks */
	rndr_blockcode, // BlockCode
	NULL, // blockQuote
	NULL, // block html
	rndr_header, // header
	NULL, // hrule
	rndr_list, // list
	rndr_listitem, // listitem
	rndr_paragraph, // paragraph
	NULL, // table
	NULL, // table cell
	NULL, // table row

	/* span-level callbacks */
	NULL, // autolink
	rndr_codespan, // codespan
	rndr_double_emphasis, // double_emphasis
	rndr_emphasis, // emphasis
	NULL, // image
	rndr_linebreak, // line break
	rndr_link, // link
	NULL, // raw html tag
	rndr_triple_emphasis, // triple emphasis

	/* low-level callbacks */
	NULL, // entity
	rndr_normal_text, // normal text

	/* renderer data */
	64, // max stack
	"*_",
	NULL // opaque
};

namespace Bypass {

	Parser::Parser() {

	}

	Parser::~Parser() {

	}

	Document
	Parser::parse(const char *str) {
		this->document = new Document();

		if (str) {
			struct buf *ib, *ob;

			ib = bufnew(INPUT_UNIT);
			bufputs(ib, str);

			ob = bufnew(OUTPUT_UNIT);

			mkd_callbacks.opaque = this;
			//parse and assemble document
			markdown(ob, ib, &mkd_callbacks);

			// We have finished parsing, move any data left in the temp string to the main string
			if (tempBlockElement != NULL) {
				moveTempToDocument();
			}

			/* cleanup */
			bufrelease(ib);
			bufrelease(ob);
		}

		return *document;
	}

	Document
	Parser::parse(const string &str) {
		return Parser::parse(str.c_str());
	}

	void 
	Parser::moveTempToDocument() {
		this->document->append(tempBlockElement);
		tempBlockElement = NULL;
	}

	void Parser::stackTempElement(BlockElement* blockElement) {
		if (tempBlockElement != NULL) {
			blockElement->append(this->tempBlockElement);
		}
		this->tempBlockElement = blockElement;
	}
}

static Bypass::Parser* getParserFromOpaque(void *opaque)
{
	return (Bypass::Parser*) opaque;
}

//Call this from block level elements to see if we need to move the
//Buffered data to the main string
static void checkDataMove(struct buf *ob, void *opaque) {
	if (ob->size == 0) {
		Bypass::Parser* parser = getParserFromOpaque(opaque);
		parser->moveTempToDocument();
	}
}

static void stackTempElement(Bypass::BlockElement* blockElement, void *opaque) {
	Bypass::Parser* parser = getParserFromOpaque(opaque);
	parser->stackTempElement(blockElement);
}

/*
 *	Block Element callbacks
 */

static void rndr_blockcode(struct buf *ob, struct buf *text, void *opaque) {

	checkDataMove(ob, opaque);
}

static void rndr_header(struct buf *ob, struct buf *text, int level, void *opaque) {
	//check if the text contains a hash from the last block level
	//if it exists make a new container level and stick the old temp string into it

	//if there is no hash
	checkDataMove(ob, opaque);

	Bypass::BlockElement* element = new Bypass::BlockElement();
	element->setText(text->data);
	stackTempElement(element, opaque);
}

static void rndr_list(struct buf *ob, struct buf *text, int flags, void *opaque) {

	checkDataMove(ob, opaque);
}

static void rndr_listitem(struct buf *ob, struct buf *text, int flags, void *opaque) {

	checkDataMove(ob, opaque);
}

static void rndr_paragraph(struct buf *ob, struct buf *text, void *opaque) {

	checkDataMove(ob, opaque);
}

/*
 *	Span Element callbacks
 */

static int rndr_codespan(struct buf *ob, struct buf *text, void *opaque) {
	return 0;
}

static int rndr_double_emphasis(struct buf *ob, struct buf *text, char c,
		void *opaque) {
	return 0;
}

static int rndr_emphasis(struct buf *ob, struct buf *text, char c,
		void *opaque) {
	return 0;
}

static int rndr_triple_emphasis(struct buf *ob, struct buf *text, char c,
		void *opaque) {
	return 0;
}

static int rndr_linebreak(struct buf *ob, void *opaque) {
	return 0;
}

static int rndr_link(struct buf *ob, struct buf *link, struct buf *title,
			struct buf *content, void *opaque) {
	return 0;
}

/*
 *	Low Level Callbacks
 */

static void rndr_normal_text(struct buf *ob, struct buf *text, void *opaque) {

}

