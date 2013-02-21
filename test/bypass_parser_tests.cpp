#define BOOST_TEST_MODULE BypassParserTests

#include <boost/test/included/unit_test.hpp>
#include <string>
#include <bypass_parser.h>

struct F
{
	F() : parser()
	{
		BOOST_TEST_MESSAGE( "setup fixture" );
	}

	~F()
	{
		BOOST_TEST_MESSAGE( "teardown fixture" );
	}

	BypassParser parser;
};

BOOST_FIXTURE_TEST_CASE(parse_with_null_input, F)
{
	Document document = parser.parse(NULL);
}

BOOST_FIXTURE_TEST_CASE(parse_with_empty_string, F)
{
	std::string s = std::string();
	Document document = parser.parse(s);
}