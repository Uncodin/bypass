#ifndef _BLOCK_ELEMENT_H_
#define _BLOCK_ELEMENT_H_

#include <vector>
#include "span_element.h"

class BlockElement
{
private:
	std::vector<BlockElement> blockElements;
	std::vector<SpanElement> spanElements;
public:

	BlockElement();
	~BlockElement();

};

#endif // _BLOCK_ELEMENT_H_
