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
		BlockElement(std::string text);
		~BlockElement();
		void append(BlockElement* blockElement);
		void append(SpanElement* spanElement);
	};

}

#endif // _BYPASS_BLOCK_ELEMENT_H_
