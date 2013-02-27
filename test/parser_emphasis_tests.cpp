#define BOOST_TEST_MODULE BypassParserTests

#include <boost/test/included/unit_test.hpp>
#include <string>
#include <iostream>
#include "parser.h"

using namespace Bypass;

struct F {
	F()
	: parser()
	{

	}

	~F() {

	}

	Parser parser;
};

BOOST_FIXTURE_TEST_CASE(parse_emphasis_with_simple_example, F) {
	Document document = parser.parse("*hello world*");

	BOOST_REQUIRE(document.size() == 1);
	BOOST_REQUIRE(document[0].getType() == PARAGRAPH);
	BOOST_REQUIRE(document[0].getText().length() == 0);
	BOOST_REQUIRE(document[0].size() == 1);
	BOOST_REQUIRE(document[0][0].getType() == EMPHASIS);
	BOOST_REQUIRE(document[0][0].getText() == "hello world");
	BOOST_REQUIRE(document[0][0].size() == 0);
}

BOOST_FIXTURE_TEST_CASE(parse_single_interspersed_emphasis, F) {
	Document document = parser.parse("one *two* three");

	BOOST_REQUIRE(document.size() == 1);
	BOOST_REQUIRE(document[0].getType() == PARAGRAPH);
	BOOST_REQUIRE(document[0].getText().length() == 0);
	BOOST_REQUIRE(document[0].size() == 3);
	BOOST_REQUIRE(document[0][0].getType() == TEXT);
	BOOST_REQUIRE(document[0][0].getText() == "one ");
	BOOST_REQUIRE(document[0][0].size() == 0);
	BOOST_REQUIRE(document[0][1].getType() == EMPHASIS);
	BOOST_REQUIRE(document[0][1].getText() == "two");
	BOOST_REQUIRE(document[0][1].size() == 0);
	BOOST_REQUIRE(document[0][2].getType() == TEXT);
	BOOST_REQUIRE(document[0][2].getText() == " three");
	BOOST_REQUIRE(document[0][2].size() == 0);
}

BOOST_FIXTURE_TEST_CASE(parse_multiple_interspersed_emphasis, F) {
	Document document = parser.parse("*one* two *three*");

	BOOST_REQUIRE(document.size() == 1);
	BOOST_REQUIRE(document[0].getType() == PARAGRAPH);
	BOOST_REQUIRE(document[0].getText().length() == 0);
	BOOST_REQUIRE(document[0].size() == 3);
	BOOST_REQUIRE(document[0][0].getType() == EMPHASIS);
	BOOST_REQUIRE(document[0][0].getText() == "one");
	BOOST_REQUIRE(document[0][0].size() == 0);
	BOOST_REQUIRE(document[0][1].getType() == TEXT);
	BOOST_REQUIRE(document[0][1].getText() == " two ");
	BOOST_REQUIRE(document[0][1].size() == 0);
	BOOST_REQUIRE(document[0][2].getType() == EMPHASIS);
	BOOST_REQUIRE(document[0][2].getText() == "three");
	BOOST_REQUIRE(document[0][2].size() == 0);
}