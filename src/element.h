//
//  Copyright 2013 Uncodin, Inc.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//

#ifndef _BYPASS_ELEMENT_H_
#define _BYPASS_ELEMENT_H_

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <set>
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
		TEXT            = 0x114,
		STRIKETHROUGH   = 0x115
	};

	class Element {
	public:
		typedef std::map<std::string, std::string> AttributeMap;

		Element();
		~Element();

		std::string text;

		void setText(const std::string& text);
		const std::string& getText();

		void addAttribute(const std::string& name, const std::string& value);
		std::string getAttribute(const std::string& name);

		AttributeMap::iterator attrBegin();
		AttributeMap::iterator attrEnd();
		size_t attrSize() const;

		void append(const Element& blockElement);
		Element getChild(size_t i);
		Element operator[](size_t i);
		void setType(Type type);
		Type getType();
		bool isBlockElement();
		bool isSpanElement();
		size_t size();
		friend std::ostream& operator<<(std::ostream& out, const Element& element);
	private:
		AttributeMap attributes;
		std::vector<Element> children;
		Type type;
	};

}

#endif // _BYPASS_ELEMENT_H_
