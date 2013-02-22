#include "span_element.h"

namespace Bypass {

	SpanElement::SpanElement() {

	}

	SpanElement::~SpanElement() {

	}

	void 
	SpanElement::setText(std::string text) {
		this->text = text;
	}

	void
	SpanElement::setExtra(std::string extra) {
		this->extra = extra;
	}
}
