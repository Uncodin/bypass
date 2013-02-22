#include "block_element.h"

namespace Bypass {

	BlockElement::BlockElement(std::string text) {
		this->text = text;
	}

	BlockElement::~BlockElement() {

	}

	void 
	BlockElement::append(BlockElement* blockElement) {
		this->blockElements.push_back(blockElement);
	}

	void
	BlockElement::append(SpanElement* spanElement) {
		this->spanElements.push_back(spanElement);
	}
}
