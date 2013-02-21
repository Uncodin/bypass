#define BOOST_TEST_MODULE MyTest

#include <boost/test/included/unit_test.hpp>
#include <BypassParser.h>

BOOST_AUTO_TEST_CASE( parse_with_null_input )
{
	BypassParser parser;

	const char *markdown = NULL;
	MOMTree momTree = parser.parse(markdown);
}