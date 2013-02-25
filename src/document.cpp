#include "document.h"

namespace Bypass {

	Document::Document() {

	}

	Document::~Document() {
		 while(!blockElements.empty()) {
			delete blockElements.back();
			blockElements.pop_back();
		}
	}

	void
	Document::append(BlockElement* blockElement)
	{
		this->blockElements.push_back(blockElement);	
	}
}
