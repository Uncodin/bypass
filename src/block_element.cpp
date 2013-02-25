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

	void  BlockElement::append(const BlockElement& blockElement) {
		this->blockElements.push_back(new BlockElement(blockElement));
	}

	void BlockElement::append(const SpanElement& spanElement) {
		this->spanElements.push_back(new SpanElement(spanElement));
	}

	void
	BlockElement::setSpanElements(std::vector<SpanElement*> elements) {
		for (std::vector<SpanElement*>::iterator it = elements.begin(); it!=elements.end(); ++it) {
		    spanElements.push_back(new SpanElement(**it));
		}
	}
}
