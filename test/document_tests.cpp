#define BOOST_TEST_MODULE DocumentTests

#include <boost/test/included/unit_test.hpp>
#include <string>
#include <document.h>

using namespace Bypass;

BOOST_AUTO_TEST_CASE(parse_with_null_input)
{
	Document document;
}