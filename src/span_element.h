#ifndef _BYPASS_SPAN_ELEMENT_H_
#define _BYPASS_SPAN_ELEMENT_H_

#include <string>

namespace Bypass {

	class SpanElement {
	private:
		std::string text;
		std::string extra;
	public:
		SpanElement();
		~SpanElement();
		void setText(std::string text);
		void setExtra(std::string extra);
	};

}

#endif // _BYPASS_SPAN_ELEMENT_H_
