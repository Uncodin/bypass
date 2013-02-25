#ifndef _BYPASS_ELEMENT_H_
#define _BYPASS_ELEMENT_H_

#include <string>
#include <vector>
#include "element.h"

namespace Bypass {

	enum Type {

		// Block Element Types

		BLOCK_CODE      = 0x000,
		BLOCK_QUOTE     = 0x001,
		BLOCK_HTML      = 0x002,
		HEADER          = 0x003,
		HRULE           = 0x004,
		LIST            = 0x005,
		LIST_ITEM       = 0x006,
		PARAGRAPH       = 0x007,
		TABLE           = 0x008,
		TABLE_CELL      = 0x009,
		TABLE_ROW       = 0x00A,

		// Span Element Types

		AUTOLINK        = 0x10B,
		CODE_SPAN       = 0x10C,
		DOUBLE_EMPHASIS = 0x10D,
		EMPHASIS        = 0x10E,
		IMAGE           = 0x10F,
		LINEBREAK       = 0x110,
		LINK            = 0x111,
		RAW_HTML_TAG    = 0x112,
		TRIPLE_EMPHASIS = 0x113,
		TEXT            = 0x114

	};

	class Element {
	public:
		Element();
		~Element();
		void setText(std::string text);
		std::string getText();
		void append(const Element& blockElement);
		Element* getChild(size_t i);
		Element* operator[](size_t i);
		void setChildren(std::vector<Element*> children);
		void setType(Type type);
		Type getType();
		bool isBlockElement();
		bool isSpanElement();
		size_t size();
	private:
		std::string text;
		std::vector<Element*> children;
		Type type;
	};

}

#endif // _BYPASS_BLOCK_ELEMENT_H_
