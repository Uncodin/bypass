#ifndef _BYPASS_PARSER_H_
#define _BYPASS_PARSER_H_

#include "document.h"

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