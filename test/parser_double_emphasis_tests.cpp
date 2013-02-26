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
		BOOST_TEST_MESSAGE( "setup fixture" );
	}

	~F() {
		BOOST_TEST_MESSAGE( "teardown fixture" );
	}

	Parser parser;
};

BOOST_FIXTURE_TEST_CASE(parse_double_emphasis_with_simple_example, F) {
	Document document = parser.parse("**hello world**");

	BOOST_REQUIRE(document.size() == 1);
	BOOST_REQUIRE(document[0].getType() == PARAGRAPH);
	BOOST_REQUIRE(document[0].getText().length() == 0);
	BOOST_REQUIRE(document[0].size() == 1);
	BOOST_REQUIRE(document[0][0].getType() == DOUBLE_EMPHASIS);
	BOOST_REQUIRE(document[0][0].getText() == "hello world");
	BOOST_REQUIRE(document[0][0].size() == 0);
}