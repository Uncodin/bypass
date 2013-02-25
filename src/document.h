#ifndef _BYPASS_DOCUMENT_H_
#define _BYPASS_DOCUMENT_H_

#include <vector>
#include "block_element.h"

namespace Bypass
{
	class Document
	{
	private:
		std::vector<BlockElement*> blockElements;
	public:
		Document();
		~Document();

		void append(const BlockElement& blockElement);

		size_t size();

		BlockElement* operator[](size_t i);
	};

}

#endif // _BYPASS_DOCUMENT_H_
