#ifndef _BYPASS_SPAN_ELEMENT_H_
#define _BYPASS_SPAN_ELEMENT_H_

#include <string>

namespace Bypass {

	class SpanElement {
	public:
		enum Type {
			AUTOLINK,
			CODE_SPAN,
			DOUBLE_EMPHASIS,
			EMPHASIS,
			IMAGE,
			LINEBREAK,
			LINK,
			RAW_HTML_TAG,
			TRIPLE_EMPHASIS,
			TEXT
		};
		SpanElement();
		~SpanElement();
		void setText(std::string text);
		void setExtra(std::string extra);
		void setType(Type type);
		Type getType();
	private:
		std::string text;
		std::string extra;
		Type type;
	};

}

#endif // _BYPASS_SPAN_ELEMENT_H_
