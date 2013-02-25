#include "document.h"

namespace Bypass {

	Document::Document() {
		blockElements = std::vector<BlockElement*>();
	}

	Document::~Document() {
		while(!blockElements.empty()) {
			delete blockElements.back();
			blockElements.pop_back();
		}
	}

	void Document::append(const BlockElement& blockElement) {
		blockElements.push_back(new BlockElement(blockElement));
	}

	size_t Document::size() {
		return blockElements.size();
	}

	BlockElement* Document::operator[](size_t i) {
		return blockElements[i];
	}

}
