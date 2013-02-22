#ifndef _BYPASS_SPAN_ELEMENT_H_
#define _BYPASS_SPAN_ELEMENT_H_

#include <string>

namespace Bypass
{

	class SpanElement
	{
	private:
		std::string text;
		std::string extra;
	public:
		SpanElement();
		~SpanElement();
	};

}

#endif // _BYPASS_SPAN_ELEMENT_H_
