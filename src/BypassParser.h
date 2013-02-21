#ifndef _BYPASS_PARSER_H_
#define _BYPASS_PARSER_H_

#include "MOMTree.h"

class BypassParser
{
private:

public:

	BypassParser();
	~BypassParser();

	MOMTree parse(const char *markdown);
	MOMTree parse(const std::string &markdown);

};

#endif // _BYPASS_PARSER_H_