#ifndef _BYPASS_DOCUMENT_H_
#define _BYPASS_DOCUMENT_H_

#include <vector>
#include "element.h"

namespace Bypass
{
	class Document
	{
	private:
		std::vector<Element*> elements;
	public:
		Document();
		~Document();
		void append(const Element& elements);
		Element* operator[](size_t i);
		size_t size();
	};
}

#endif // _BYPASS_DOCUMENT_H_
