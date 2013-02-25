#include "element.h"

namespace Bypass {

	Element::Element() {
		type = TEXT;
		children = std::vector<Element*>();
		attributes = std::map<std::string, std::string>();
	}

	Element::~Element() {
		while (!children.empty()) {
			delete children.back();
			children.pop_back();
		}
	}

	void Element::setText(std::string text) {
		this->text = text;
	}

	std::string Element::getText() {
		return this->text;
	}

	void Element::addAttribute(std::string name, std::string value) {
		attributes[name] = value;
	}

	std::string Element::getAttribute(std::string name) {
		return attributes[name];
	}

	void  Element::append(const Element& child) {
		children.push_back(new Element(child));
	}

	Element* Element::operator[](size_t i) {
		return children[i];
	}

	void Element::setChildren(std::vector<Element*> children) {
		this->children.insert(this->children.end(), children.begin(), children.end());
	}

	void Element::setType(Type type) {
		this->type = type;
	}

	Type Element::getType() {
		return type;
	}

	bool Element::isBlockElement() {
		return (type & 0x100) == 0x000;
	}

	bool Element::isSpanElement() {
		return (type & 0x100) == 0x100;
	}

	size_t Element::size() {
		return children.size();
	}

}
