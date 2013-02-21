#ifndef _BYPASS_H_
#define _BYPASS_H_

class BypassParser
{
private:

public:

	BypassParser();
	~BypassParser();

	void parse(const char *markdown);
	void parse(std::string markdown);

};

#endif