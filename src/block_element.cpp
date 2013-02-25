#include "block_element.h"

namespace Bypass {

	BlockElement::BlockElement() {
		type = PARAGRAPH;
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

	BlockElement* BlockElement::getBlockElementAtIndex(size_t i) {
		return blockElements[i];
	}

	void BlockElement::append(const SpanElement& spanElement) {
		this->spanElements.push_back(new SpanElement(spanElement));
	}

	void BlockElement::setSpanElements(std::vector<SpanElement*> elements) {
		for (std::vector<SpanElement*>::iterator it = elements.begin(); it!=elements.end(); ++it) {
		    spanElements.push_back(new SpanElement(**it));
		}
	}

	SpanElement* BlockElement::getSpanElementAtIndex(size_t i) {
		return spanElements[i];
	}

	void BlockElement::setType(Type type) {

	}

	BlockElement::Type BlockElement::getType() {
		return type;
	}
}
