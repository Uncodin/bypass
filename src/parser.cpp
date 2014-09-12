//
//  Copyright 2013 Uncodin, Inc.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//

#include "parser.h"

using namespace std;

static void rndr_blockcode(struct buf *ob, struct buf *text, void *opaque);
static void rndr_blockquote(struct buf *ob, struct buf *text, void *opaque);
static void rndr_header(struct buf *ob, struct buf *text, int level, void *opaque);
static void rndr_hrule(struct buf *ob, void *opaque);
static void rndr_list(struct buf *ob, struct buf *text, int flags, void *opaque);
static void rndr_listitem(struct buf *ob, struct buf *text, int flags, void *opaque);
static void rndr_paragraph(struct buf *ob, struct buf *text, void *opaque);
static int rndr_autolink(struct buf *ob, struct buf *link, enum mkd_autolink type, void *opaque);
static int rndr_codespan(struct buf *ob, struct buf *text, void *opaque);
static int rndr_double_emphasis(struct buf *ob, struct buf *text, char c, void *opaque);
static int rndr_emphasis(struct buf *ob, struct buf *text, char c, void *opaque);
static int rndr_image(struct buf *ob, struct buf *link, struct buf *title, struct buf *alt, void *opaque);
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
	rndr_hrule,           // hrule
	rndr_list,            // list
	rndr_listitem,        // listitem
	rndr_paragraph,       // paragraph
	NULL,                 // table
	NULL,                 // table cell
	NULL,                 // table row

	/* span-level callbacks */
	rndr_autolink,        // autolink
	rndr_codespan,        // codespan
	rndr_double_emphasis, // double emphasis
	rndr_emphasis,        // emphasis
	rndr_image,           // image
	rndr_linebreak,       // line break
	rndr_link,            // link
	NULL,                 // raw html tag
	rndr_triple_emphasis, // triple emphasis

	/* low-level callbacks */
	NULL,                 // entity
	rndr_normal_text,     // normal text

	/* renderer data */
	64, // max stack
	"*_~",
	NULL // opaque
};

namespace Bypass {

	const static std::string TWO_SPACES = "  ";
	const static std::string NEWLINE = "\n";

	Parser::Parser()
	: elementSoup()
	{
		elementCount = 1;
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

			for (std::map<int, Element>::iterator it = elementSoup.begin(); it != elementSoup.end(); ++it) {
				document.append(it->second);
			}

			bufrelease(ib);
			bufrelease(ob);
		}

		return document;
	}

	Document Parser::parse(const string& markdown) {
		return parse(markdown.c_str());
	}

	void Parser::eraseTrailingControlCharacters(const std::string& controlCharacters) {
		std::map<int, Element>::iterator it = elementSoup.find(elementCount);

		if ( it != elementSoup.end() ) {
			Element * element = &((*it).second);

			if (boost::ends_with(element->text, controlCharacters)) {
				boost::erase_tail(element->text, controlCharacters.size());
			}
		}
	}

	// Appends the marker "<elementcount>|", so we can later reference the
	// element by position
	void Parser::appendElementMarker(struct buf *ob) {
		std::ostringstream oss;
		oss << elementCount << '|';
		bufputs(ob, oss.str().c_str());
	}

	// Block Element Callbacks

	void Parser::handleBlock(Type type, struct buf *ob, struct buf *text, int extra) {
		Element block;
		block.setType(type);

		if (type == HEADER) {
			char levelStr[2];
			snprintf(levelStr, 2, "%d", extra);
			block.addAttribute("level", levelStr);
		}
		else if (type == LIST) {
			char flagsStr[3];
			snprintf(flagsStr, 3, "%d", extra);
			block.addAttribute("flags", flagsStr);
		}

		if (text) {
			std::string textString(text->data, text->data + text->size);
			std::vector<std::string> strs;
			boost::split(strs, textString, boost::is_any_of("|"));

			for(vector<std::string>::iterator it = strs.begin(); it != strs.end(); it++) {
				int pos = atoi((*it).c_str());
				std::map<int, Element>::iterator elit = elementSoup.find(pos);

				if ( elit != elementSoup.end() ) {
					block.append((*elit).second);
					elementSoup.erase(pos);
				}
			}
		}

		elementCount++;
		elementSoup[elementCount] = block;
		appendElementMarker(ob);
	}

	void Parser::parsedBlockCode(struct buf *ob, struct buf *text) {
		if(!text) return; // Analyze seems to believe that text can be null here
		parsedNormalText(ob, text);
		eraseTrailingControlCharacters(NEWLINE);

		bufreset(text);
		appendElementMarker(text);
		handleBlock(BLOCK_CODE, ob, text);
	}

	void Parser::parsedBlockQuote(struct buf *ob, struct buf *text) {
		handleBlock(BLOCK_QUOTE, ob, text);
	}

	void Parser::parsedHeader(struct buf *ob, struct buf *text, int level) {
		handleBlock(HEADER, ob, text, level);
	}

	void Parser::parsedHrule(struct buf *ob) {
		handleBlock(HRULE, ob);
	}

	void Parser::parsedList(struct buf *ob, struct buf *text, int flags) {
		handleBlock(LIST, ob, text, flags);
	}

	void Parser::parsedListItem(struct buf *ob, struct buf *text, int flags) {
		handleBlock(LIST_ITEM, ob, text);
	}

	void Parser::parsedParagraph(struct buf *ob, struct buf *text) {
		handleBlock(PARAGRAPH, ob, text);
	}

	// Span Element Callbacks

	void Parser::handleSpan(Type type, struct buf *ob, struct buf *text, struct buf *extra, struct buf *extra2, bool output) {

		std::vector<std::string> strs;
		std::string textString;
		if (text) {
			textString = std::string(text->data, text->data + text->size);
			boost::split(strs, textString, boost::is_any_of("|"));
		}
		if (strs.size() > 0) {
            std::string str0 = strs[0];

            if (str0.length() > 0) {
                int pos = atoi(str0.c_str());
                std::map<int, Element>::iterator elit = elementSoup.find(pos);

                Element element = elit->second;
                element.setType(type);

                if (extra != NULL && extra->size) {
                    if (element.getType() == LINK) {
                        element.addAttribute("link", std::string(extra->data, extra->data + extra->size));
                    }
                }

                if (extra2 != NULL && extra2->size) {
                    if (element.getType() == LINK) {
                        element.addAttribute("title", std::string(extra2->data, extra2->data + extra2->size));
                    }
                }
                
                elementSoup.erase(pos);
                if (output) {
                    elementSoup[pos] = element;
                }
            }

            if (output) {
                bufputs(ob, textString.c_str());
            }
		}
		else {
			Element element;
			element.setType(type);

			createSpan(element, ob);
		}
	}

	void Parser::handleNontextSpan(Type type, struct buf *ob, struct buf *link, struct buf *title, struct buf *alt) {
		Element element;
		element.setType(type);

		if (link) {
			element.addAttribute("link", std::string(link->data, link->data + link->size));
		}
		if (title) {
			element.addAttribute("title", std::string(title->data, title->data + title->size));
		}
		if (alt) {
			element.addAttribute("alt", std::string(alt->data, alt->data + alt->size));
		}

		// libsoldout does this neat trick with the output buffer during image
		// processing; it outputs the '!' as normal text, but then undoes the
		// buffer position when it finds out it was actually part of an image.
		//
		// Unfortunately, we are not using text as our output buffer, so we
		// have to get rid of that bang ourselves.
		if (type == IMAGE) {
			elementSoup[elementCount].text.erase(elementSoup[elementCount].text.size() - 1);
		}

		elementCount++;
		elementSoup[elementCount] = element;
		appendElementMarker(ob);
	}

	void Parser::createSpan(const Element& element, struct buf *ob) {
		elementCount++;
		elementSoup[elementCount] = element;
		appendElementMarker(ob);
	}

	int Parser::parsedDoubleEmphasis(struct buf *ob, struct buf *text, char c) {
        if (c == '~') {
            handleSpan(STRIKETHROUGH, ob, text);
        } else {
            handleSpan(DOUBLE_EMPHASIS, ob, text);
        }
		return 1;
	}

	int Parser::parsedEmphasis(struct buf *ob, struct buf *text, char c) {
        if (c == '~') {
            handleSpan(STRIKETHROUGH, ob, text, NULL, NULL, false);
            return 0;
        } else {
            handleSpan(EMPHASIS, ob, text);
            return 1;
        }
	}

	int Parser::parsedImage(struct buf *ob, struct buf *link, struct buf *title, struct buf *alt) {
		handleNontextSpan(IMAGE, ob, link, title, alt);
		return 1;
	}

	int Parser::parsedTripleEmphasis(struct buf *ob, struct buf *text, char c) {
		if (c == '~') {
            handleSpan(STRIKETHROUGH, ob, text, NULL, NULL, false);
            return 0;
        } else {
            handleSpan(TRIPLE_EMPHASIS, ob, text);
            return 1;
        }
	}

	int Parser::parsedLink(struct buf *ob, struct buf *link, struct buf *title, struct buf *content) {
		handleSpan(LINK, ob, content, link, title);
		return 1;
	}

	int Parser::parsedAutolink(struct buf *ob, struct buf *link, enum mkd_autolink type) {
		handleNontextSpan(AUTOLINK, ob, link);
		return 1;
	}

	int Parser::parsedCodeSpan(struct buf *ob, struct buf *text) {
		if (text && text->size > 0) {
			Element codeSpan;
			codeSpan.setType(CODE_SPAN);
			codeSpan.text.assign(text->data, text->data + text->size);
			createSpan(codeSpan, ob);
		}
		return 1;
	}

	int Parser::parsedLinebreak(struct buf *ob) {
		eraseTrailingControlCharacters(TWO_SPACES);
		handleSpan(LINEBREAK, ob, NULL);
		return 1;
	}

	// Low Level Callbacks

	void Parser::parsedNormalText(struct buf *ob, struct buf *text) {
		// The parser will spuriously emit a text callback for an empty string
		// that butts up against a span-level element. This will ignore it.

		if (text && text->size > 0) {
			Element normalText;
			normalText.setType(TEXT);
			normalText.text.assign(text->data, text->data + text->size);
			createSpan(normalText, ob);
		}
	}

}

// Block Element callbacks

static void rndr_blockcode(struct buf *ob, struct buf *text, void *opaque) {
	((Bypass::Parser*) opaque)->parsedBlockCode(ob, text);
}

static void rndr_blockquote(struct buf *ob, struct buf *text, void *opaque) {
	((Bypass::Parser*) opaque)->parsedBlockQuote(ob, text);
}

static void rndr_header(struct buf *ob, struct buf *text, int level, void *opaque) {
	((Bypass::Parser*) opaque)->parsedHeader(ob, text, level);
}

static void rndr_hrule(struct buf *ob, void *opaque) {
	((Bypass::Parser*) opaque)->parsedHrule(ob);
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

static int rndr_autolink(struct buf *ob, struct buf *link, enum mkd_autolink type, void *opaque) {
	return ((Bypass::Parser*) opaque)->parsedAutolink(ob, link, type);
}

static int rndr_codespan(struct buf *ob, struct buf *text, void *opaque) {
	return ((Bypass::Parser*) opaque)->parsedCodeSpan(ob, text);
}

static int rndr_double_emphasis(struct buf *ob, struct buf *text, char c, void *opaque) {
	return ((Bypass::Parser*) opaque)->parsedDoubleEmphasis(ob, text, c);
}

static int rndr_emphasis(struct buf *ob, struct buf *text, char c, void *opaque) {
	return ((Bypass::Parser*) opaque)->parsedEmphasis(ob, text, c);
}

static int rndr_image(struct buf *ob, struct buf *link, struct buf *title, struct buf *alt, void *opaque) {
	return ((Bypass::Parser*) opaque)->parsedImage(ob, link, title, alt);
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

