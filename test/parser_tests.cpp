#define BOOST_TEST_MODULE BypassParserTests

#include <boost/test/included/unit_test.hpp>
#include <string>
#include "parser.h"

using namespace Bypass;

struct F {
	F() :
		parser(),
		p("A paragraph is simply one or more consecutive lines of text, "
		  " separated by one or more blank lines. (A blank line is any line"
		  " that looks like a blank line — a line containing nothing but spaces"
		  " or tabs is considered blank.) Normal paragraphs should not be "
		  " indented with spaces or tabs.")
	{
		BOOST_TEST_MESSAGE( "setup fixture" );
	}

	~F() {
		BOOST_TEST_MESSAGE( "teardown fixture" );
	}

	Parser parser;
	std::string p;
};

BOOST_FIXTURE_TEST_CASE(parse_with_null_input, F) {
	Document document = parser.parse(NULL);
}