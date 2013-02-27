#define BOOST_TEST_MODULE BypassParserTests

#include <boost/test/included/unit_test.hpp>
#include <string>
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

BOOST_FIXTURE_TEST_CASE(parse_null, F) {
	Document document = parser.parse(NULL);
	BOOST_REQUIRE(document.size() == 0);
}

BOOST_FIXTURE_TEST_CASE(parse_empty, F) {
	Document document = parser.parse(std::string(""));
	BOOST_REQUIRE(document.size() == 0);
}