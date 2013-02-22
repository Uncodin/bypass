#ifndef _BYPASS_BLOCK_ELEMENT_H_
#define _BYPASS_BLOCK_ELEMENT_H_

#include <vector>
#include "span_element.h"

namespace Bypass {

	class BlockElement {
	private:
		std::vector<BlockElement> blockElements;
		std::vector<SpanElement> spanElements;
	public:
		BlockElement();
		~BlockElement();
	};

}

#endif // _BYPASS_BLOCK_ELEMENT_H_
