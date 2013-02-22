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
		void moveTempToDocument();
		void stackTempElement(BlockElement* blockElement);
	private:
		Document* document;
		BlockElement* tempBlockElement;
	};

}

#endif // _BYPASS_PARSER_H_
