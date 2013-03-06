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

#define BOOST_TEST_MODULE DocumentTests

#include <boost/test/included/unit_test.hpp>
#include <string>
#include <stdio.h>
#include "document.h"

using namespace Bypass;

struct F {
	F()
	: document()
	{

	}

	~F() {

	}

	Document document;
};

BOOST_FIXTURE_TEST_CASE(document_initial_size, F) {
	BOOST_REQUIRE(document.size() == 0);
}

BOOST_FIXTURE_TEST_CASE(document_append_single, F) {
	Element element;

	document.append(element);
	BOOST_REQUIRE(document.size() == 1);
}

BOOST_FIXTURE_TEST_CASE(document_append_multiple, F) {
	int i, count = 3;

	for (i = 0; i < count; i++) {
		Element element;
		char str[10];
		sprintf(str, "%d", i);
		element.setText(std::string(str));
		document.append(element);
	}

	BOOST_REQUIRE(document.size() == 3);
}

BOOST_FIXTURE_TEST_CASE(document_access_single, F) {
	std::string expected = "expected";

	Element element;
	element.setText(expected);

	document.append(element);

	BOOST_REQUIRE(document[0].getText() == expected);
}

BOOST_FIXTURE_TEST_CASE(document_access_multiple, F) {
	int i, count = 3;

	for (i = 0; i < count; i++) {
		Element element;

		char str[10];
		sprintf(str, "%d", i);

		const std::string value = std::string(str);

		element.setText(value);
		document.append(element);
	}

	BOOST_REQUIRE(document[0].getText() == "0");
	BOOST_REQUIRE(document[1].getText() == "1");
	BOOST_REQUIRE(document[2].getText() == "2");
}