#include "document.h"

namespace Bypass {

	Document::Document()
	: elements()
	{

	}

	Document::~Document() {

	}

	void Document::append(const Element& element) {
		elements.push_back(Element(element));
	}

	size_t Document::size() {
		return elements.size();
	}

	Element Document::operator[](size_t i) {
		return elements[i];
	}

}
