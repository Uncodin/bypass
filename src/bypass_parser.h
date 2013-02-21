#ifndef _BYPASS_PARSER_H_
#define _BYPASS_PARSER_H_

#include "document.h"

class BypassParser
{
private:

public:

	BypassParser();
	~BypassParser();

	Document parse(const char *markdown);
	Document parse(const std::string &markdown);

};

#endif // _BYPASS_PARSER_H_