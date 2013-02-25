#define BOOST_TEST_MODULE DocumentTests

#include <boost/test/included/unit_test.hpp>
#include <string>
#include <stdio.h>
#include "document.h"

using namespace Bypass;

struct F {
	F() : document() {
		BOOST_TEST_MESSAGE( "setup fixture" );
	}

	~F() {
		BOOST_TEST_MESSAGE( "teardown fixture" );
	}

	Document document;
};

BOOST_FIXTURE_TEST_CASE(document_initial_size, F) {
	BOOST_REQUIRE(document.size() == 0);
}

BOOST_FIXTURE_TEST_CASE(document_append_once, F) {
	BlockElement blockElement;
	blockElement.setText("block");

	document.append(blockElement);
	BOOST_REQUIRE(document.size() == 1);
}

BOOST_FIXTURE_TEST_CASE(document_append_multiple, F) {
	int i, count = 3;

	for (i = 0; i < count; i++) {
		BlockElement blockElement;
		char str[10];
		sprintf(str, "%d", i);
		blockElement.setText(std::string(str));
		document.append(blockElement);
	}

	BOOST_REQUIRE(document.size() == 3);
}

BOOST_FIXTURE_TEST_CASE(document_access_with_single_member, F) {
	BlockElement blockElement;
	blockElement.setText("block");

	document.append(blockElement);
	BlockElement *expected = document[0];

	BOOST_REQUIRE(expected->getText() == "block");
}

BOOST_FIXTURE_TEST_CASE(document_access_with_multiple_members, F) {
	int i, count = 3;

	for (i = 0; i < count; i++) {
		BlockElement blockElement;

		char str[10];
		sprintf(str, "%d", i);

		const std::string value = std::string(str);

		blockElement.setText(value);
		document.append(blockElement);
	}

	BOOST_REQUIRE(document[0]->getText().length() == 1);
	BOOST_REQUIRE(document[0]->getText() == "0");

	BOOST_REQUIRE(document[1]->getText().length() == 1);
	BOOST_REQUIRE(document[1]->getText() == "1");

	BOOST_REQUIRE(document[2]->getText().length() == 1);
	BOOST_REQUIRE(document[2]->getText() == "2");
}