#include "document.h"

namespace Bypass {

	Document::Document() {
		elements = std::vector<Element*>();
	}

	Document::~Document() {
		while(!elements.empty()) {
			delete elements.back();
			elements.pop_back();
		}
	}

	void Document::append(const Element& element) {
		elements.push_back(new Element(element));
	}

	size_t Document::size() {
		return elements.size();
	}

	Element* Document::operator[](size_t i) {
		return elements[i];
	}

}
