#include "document.h"

namespace Bypass {

	Document::Document() {

	}

	Document::~Document() {

	}

	void
	Document::append(BlockElement* blockElement)
	{
		this->blockElements.push_back(blockElement);	
	}
}
