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

#ifndef BYPASS_PARSER_H
#define BYPASS_PARSER_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <boost/algorithm/string.hpp>

extern "C" {
#include "markdown.h"
}

#include "document.h"
#include "element.h"

#define INPUT_UNIT 1024
#define OUTPUT_UNIT 64

namespace Bypass {

	class Parser {
	public:
		Parser();
		~Parser();

		Document parse(const char* markdown);
		Document parse(const std::string &markdown);

		// Block Element Callbacks

		void parsedBlockCode(struct buf *ob, struct buf *text);
		void parsedBlockQuote(struct buf *ob, struct buf *text);
		void parsedHeader(struct buf *ob, struct buf *text, int level);
		void parsedHrule(struct buf *ob);
		void parsedList(struct buf *ob, struct buf *text, int flags);
		void parsedListItem(struct buf *ob, struct buf *text, int flags);
		void parsedParagraph(struct buf *ob, struct buf *text);

		// Span Element Callbacks

		int parsedAutolink(struct buf *ob, struct buf *link, enum mkd_autolink type);
		int parsedCodeSpan(struct buf *ob, struct buf *text);
		int parsedDoubleEmphasis(struct buf *ob, struct buf *text, char c);
		int parsedEmphasis(struct buf *ob, struct buf *text, char c);
		int parsedImage(struct buf *ob, struct buf *link, struct buf *title, struct buf *alt);
		int parsedTripleEmphasis(struct buf *ob, struct buf *text, char c);
		int parsedLinebreak(struct buf *ob);
		int parsedLink(struct buf *ob, struct buf *link, struct buf *title, struct buf *content);

		// Low Level Callbacks

		void parsedNormalText(struct buf *ob, struct buf *text);

		// Debugging

		void printBuf(struct buf *b);

	private:
		Document document;
		std::map<int, Element> elementSoup;
		int elementCount;
		void handleBlock(Type, struct buf *ob, struct buf *text = NULL, int extra = -1);
		void handleSpan(Type, struct buf *ob, struct buf *text, struct buf *extra = NULL, struct buf *extra2 = NULL, bool output = true);
		void handleNontextSpan(Type, struct buf *ob, struct buf *link, struct buf *title = NULL, struct buf *alt = NULL);
		void createSpan(const Element&, struct buf *ob);
		void appendElementMarker(struct buf *ob);
		void eraseTrailingControlCharacters(const std::string& controlCharacters);
	};

}

#endif // BYPASS_PARSER_H
