#ifndef _BYPASS_PARSER_H_
#define _BYPASS_PARSER_H_

#include <string>

extern "C" {
#include "markdown.h"
}

#include "document.h"

#define INPUT_UNIT 1024
#define OUTPUT_UNIT 64

namespace Bypass
{

	class Parser
	{
	public:
		Parser();
		~Parser();
		Document parse(const char *markdown);
		Document parse(const std::string &markdown);
	};

}

#endif // _BYPASS_PARSER_H_
