#include "block_element.h"

namespace Bypass {

	BlockElement::BlockElement() {

	}

	BlockElement::~BlockElement() {

	}

	void BlockElement::setText(std::string text) {
		this->text = text;
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
