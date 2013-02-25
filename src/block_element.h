#ifndef _BYPASS_BLOCK_ELEMENT_H_
#define _BYPASS_BLOCK_ELEMENT_H_

#include <string>
#include <vector>
#include "span_element.h"

namespace Bypass {

	class BlockElement {
	private:
		std::string text;
		std::vector<BlockElement*> blockElements;
		std::vector<SpanElement*> spanElements;
	public:
		BlockElement();
		~BlockElement();
		void setText(std::string text);
		std::string getText();
		void append(BlockElement* blockElement);
		void append(SpanElement* spanElement);
		void setSpanElements(std::vector<SpanElement*> elements);
	};

}

#endif // _BYPASS_BLOCK_ELEMENT_H_
