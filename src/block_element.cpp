#include "block_element.h"

namespace Bypass {

	BlockElement::BlockElement() {

	}

	BlockElement::~BlockElement() {
		while(!spanElements.empty()) {
			delete spanElements.back();
			spanElements.pop_back();
		}

		while(!blockElements.empty()) {
			delete blockElements.back();
			blockElements.pop_back();
		}
	}

	void BlockElement::setText(std::string text) {
		this->text = text;
	}

	std::string BlockElement::getText() {
		return this->text;
	}

	void  BlockElement::append(BlockElement* blockElement) {
		this->blockElements.push_back(blockElement);
	}

	void BlockElement::append(SpanElement* spanElement) {
		this->spanElements.push_back(spanElement);
	}

	void
	BlockElement::setSpanElements(std::vector<SpanElement*> elements) {
		spanElements = elements;
	}
}
