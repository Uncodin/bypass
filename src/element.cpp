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

#include "element.h"

namespace Bypass {

	Element::Element()
	: text()
	, attributes()
	{
		type = PARAGRAPH;
	}

	Element::~Element() {

	}

	void Element::setText(const std::string& text) {
		this->text = text;
	}

	const std::string& Element::getText() {
		return text;
	}

	void Element::addAttribute(const std::string& name, const std::string& value) {
		attributes.insert(std::make_pair(name, value));
	}

	std::string Element::getAttribute(const std::string& name) {
		return attributes[name];
	}

	Element::AttributeMap::iterator Element::attrBegin() {
		return attributes.begin();
	}

	Element::AttributeMap::iterator Element::attrEnd() {
		return attributes.end();
	}
	
	size_t Element::attrSize() const {
		return attributes.size();
	}

	void Element::append(const Element& child) {
		children.push_back(Element(child));
	}

	Element Element::operator[](size_t i) {
		return children[i];
	}

	void Element::setType(Type type) {
		this->type = type;
	}

	Type Element::getType() {
		return type;
	}

	bool Element::isBlockElement() {
		return (type & 0x100) == 0x000;
	}

	bool Element::isSpanElement() {
		return (type & 0x100) == 0x100;
	}

	size_t Element::size() {
		return children.size();
	}

	std::ostream& operator<<(std::ostream& out, const Element& element) {

		std::string type;

		switch (element.type) {
			case BLOCK_CODE:
				type = "BLOCK_CODE";
				break;
			case BLOCK_QUOTE:
				type = "BLOCK_QUOTE";
				break;
			case BLOCK_HTML:
				type = "BLOCK_HTML";
				break;
			case HEADER:
				type = "HEADER";
				break;
			case HRULE:
				type = "HRULE";
				break;
			case LIST:
				type = "LIST";
				break;
			case LIST_ITEM:
				type = "LIST_ITEM";
				break;
			case PARAGRAPH:
				type = "PARAGRAPH";
				break;
			case TABLE:
				type = "TABLE";
				break;
			case TABLE_CELL:
				type = "TABLE_CELL";
				break;
			case TABLE_ROW:
				type = "TABLE_ROW";
				break;
			case AUTOLINK:
				type = "AUTOLINK";
				break;
			case CODE_SPAN:
				type = "CODE_SPAN";
				break;
			case DOUBLE_EMPHASIS:
				type = "DOUBLE_EMPHASIS";
				break;
			case EMPHASIS:
				type = "EMPHASIS";
				break;
			case IMAGE:
				type = "IMAGE";
				break;
			case LINEBREAK:
				type = "LINEBREAK";
				break;
			case LINK:
				type = "LINK";
				break;
			case RAW_HTML_TAG:
				type = "RAW_HTML_TAG";
				break;
			case TRIPLE_EMPHASIS:
				type = "TRIPLE_EMPHASIS";
				break;
			case TEXT:
				type = "TEXT";
				break;
			case STRIKETHROUGH:
				type = "STRIKETHROUGH";
				break;
		}

		out << "Element {" << std::endl;
		out << "  type: " << type << std::endl;
		out << ", text: \"" << element.text << "\"" << std::endl;
		out << "}" << std::endl;
		return out;
	}
}
