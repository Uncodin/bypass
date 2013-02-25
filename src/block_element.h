#ifndef _BYPASS_BLOCK_ELEMENT_H_
#define _BYPASS_BLOCK_ELEMENT_H_

#include <string>
#include <vector>
#include "span_element.h"

namespace Bypass {

	class BlockElement {
	public:
		enum Type {
			BLOCK_CODE,
			BLOCK_QUOTE,
			BLOCK_HTML,
			HEADER,
			HRULE,
			LIST,
			LIST_ITEM,
			PARAGRAPH,
			TABLE,
			TABLE_CELL,
			TABLE_ROW
		};

		BlockElement();
		~BlockElement();
		void setText(std::string text);
		std::string getText();
		void append(const BlockElement& blockElement);
		void append(const SpanElement& spanElement);
		void setSpanElements(std::vector<SpanElement*> elements);
		void setType(Type type);
		Type getType();
	private:
		std::string text;
		std::vector<BlockElement*> blockElements;
		std::vector<SpanElement*> spanElements;
		Type type;
	};

}

#endif // _BYPASS_BLOCK_ELEMENT_H_
