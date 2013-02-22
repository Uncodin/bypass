#ifndef _BYPASS_PARSER_H_
#define _BYPASS_PARSER_H_

#include <string>

extern "C" {
#include "markdown.h"
}

#include "document.h"
#include "block_element.h"

#define INPUT_UNIT 1024
#define OUTPUT_UNIT 64

namespace Bypass {

	class Parser {
	public:
		Parser();
		~Parser();

		Document parse(const char *markdown);
		Document parse(const std::string &markdown);

		// Parsing Internals

		void moveTempToDocument();
		void stackTempElement(BlockElement* blockElement);

		// Block Element Callbacks

		void parsedBlockcode(struct buf *ob, struct buf *text);
		void parsedBlockquote(struct buf *ob, struct buf *text);
		void parsedHeader(struct buf *ob, struct buf *text, int level);
		void parsedList(struct buf *ob, struct buf *text, int flags);
		void parsedListItem(struct buf *ob, struct buf *text, int flags);
		void parsedParagraph(struct buf *ob, struct buf *text);

		// Span Element Callbacks

		int parsedCodeSpan(struct buf *ob, struct buf *text);
		int parsedDoubleEmphasis(struct buf *ob, struct buf *text, char c);
		int parsedEmphasis(struct buf *ob, struct buf *text, char c);
		int parsedTripleEmphasis(struct buf *ob, struct buf *text, char c);
		int parsedLinebreak(struct buf *ob);
		int parsedLink(struct buf *ob, struct buf *link, struct buf *title, struct buf *content);

		// Low Level Callbacks

		void parsedNormalText(struct buf *ob, struct buf *text);

	private:
		Document* document;
		BlockElement* tempBlockElement;
	};

}

#endif // _BYPASS_PARSER_H_
