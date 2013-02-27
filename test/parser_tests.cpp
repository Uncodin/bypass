#define BOOST_TEST_MODULE ParserTests

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

// Edge Cases ------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE(parse_null, F) {
	Document document = parser.parse(NULL);
	BOOST_REQUIRE(document.size() == 0);
}

BOOST_FIXTURE_TEST_CASE(parse_empty, F) {
	Document document = parser.parse(std::string(""));
	BOOST_REQUIRE(document.size() == 0);
}

// Text ------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE(parse_text, F) {
	Document document = parser.parse("hello world");

	BOOST_REQUIRE(document.size() == 1);
	BOOST_REQUIRE(document[0].getType() == PARAGRAPH);
	BOOST_REQUIRE(document[0].getText().length() == 0);

	BOOST_REQUIRE(document[0].size() == 1);
	BOOST_REQUIRE(document[0][0].getType() == TEXT);
	BOOST_REQUIRE(document[0][0].getText() == "hello world");
	BOOST_REQUIRE(document[0][0].size() == 0);
}

// Emphasis --------------------------------------------------------------------

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

// Double Emphasis -------------------------------------------------------------

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

BOOST_FIXTURE_TEST_CASE(parse_single_interspersed_double_emphasis, F) {
	Document document = parser.parse("one **two** three");

	BOOST_REQUIRE(document.size() == 1);
	BOOST_REQUIRE(document[0].getType() == PARAGRAPH);
	BOOST_REQUIRE(document[0].getText().length() == 0);
	BOOST_REQUIRE(document[0].size() == 3);
	BOOST_REQUIRE(document[0][0].getType() == TEXT);
	BOOST_REQUIRE(document[0][0].getText() == "one ");
	BOOST_REQUIRE(document[0][0].size() == 0);
	BOOST_REQUIRE(document[0][1].getType() == DOUBLE_EMPHASIS);
	BOOST_REQUIRE(document[0][1].getText() == "two");
	BOOST_REQUIRE(document[0][1].size() == 0);
	BOOST_REQUIRE(document[0][2].getType() == TEXT);
	BOOST_REQUIRE(document[0][2].getText() == " three");
	BOOST_REQUIRE(document[0][2].size() == 0);
}

BOOST_FIXTURE_TEST_CASE(parse_multiple_interspersed_double_emphasis, F) {
	Document document = parser.parse("**one** two **three**");

	BOOST_REQUIRE(document.size() == 1);
	BOOST_REQUIRE(document[0].getType() == PARAGRAPH);
	BOOST_REQUIRE(document[0].getText().length() == 0);
	BOOST_REQUIRE(document[0].size() == 3);
	BOOST_REQUIRE(document[0][0].getType() == DOUBLE_EMPHASIS);
	BOOST_REQUIRE(document[0][0].getText() == "one");
	BOOST_REQUIRE(document[0][0].size() == 0);
	BOOST_REQUIRE(document[0][1].getType() == TEXT);
	BOOST_REQUIRE(document[0][1].getText() == " two ");
	BOOST_REQUIRE(document[0][1].size() == 0);
	BOOST_REQUIRE(document[0][2].getType() == DOUBLE_EMPHASIS);
	BOOST_REQUIRE(document[0][2].getText() == "three");
	BOOST_REQUIRE(document[0][2].size() == 0);
}

// Triple Emphasis -------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE(parse_triple_emphasis_with_simple_example, F) {
	Document document = parser.parse("***hello world***");

	BOOST_REQUIRE(document.size() == 1);
	BOOST_REQUIRE(document[0].getType() == PARAGRAPH);
	BOOST_REQUIRE(document[0].getText().length() == 0);
	BOOST_REQUIRE(document[0].size() == 1);
	BOOST_REQUIRE(document[0][0].getType() == TRIPLE_EMPHASIS);
	BOOST_REQUIRE(document[0][0].getText() == "hello world");
	BOOST_REQUIRE(document[0][0].size() == 0);
}

BOOST_FIXTURE_TEST_CASE(parse_single_interspersed_triple_emphasis, F) {
	Document document = parser.parse("one ***two*** three");

	BOOST_REQUIRE(document.size() == 1);
	BOOST_REQUIRE(document[0].getType() == PARAGRAPH);
	BOOST_REQUIRE(document[0].getText().length() == 0);
	BOOST_REQUIRE(document[0].size() == 3);
	BOOST_REQUIRE(document[0][0].getType() == TEXT);
	BOOST_REQUIRE(document[0][0].getText() == "one ");
	BOOST_REQUIRE(document[0][0].size() == 0);
	BOOST_REQUIRE(document[0][1].getType() == TRIPLE_EMPHASIS);
	BOOST_REQUIRE(document[0][1].getText() == "two");
	BOOST_REQUIRE(document[0][1].size() == 0);
	BOOST_REQUIRE(document[0][2].getType() == TEXT);
	BOOST_REQUIRE(document[0][2].getText() == " three");
	BOOST_REQUIRE(document[0][2].size() == 0);
}

BOOST_FIXTURE_TEST_CASE(parse_multiple_interspersed_triple_emphasis, F) {
	Document document = parser.parse("***one*** two ***three***");

	BOOST_REQUIRE(document.size() == 1);
	BOOST_REQUIRE(document[0].getType() == PARAGRAPH);
	BOOST_REQUIRE(document[0].getText().length() == 0);
	BOOST_REQUIRE(document[0].size() == 3);
	BOOST_REQUIRE(document[0][0].getType() == TRIPLE_EMPHASIS);
	BOOST_REQUIRE(document[0][0].getText() == "one");
	BOOST_REQUIRE(document[0][0].size() == 0);
	BOOST_REQUIRE(document[0][1].getType() == TEXT);
	BOOST_REQUIRE(document[0][1].getText() == " two ");
	BOOST_REQUIRE(document[0][1].size() == 0);
	BOOST_REQUIRE(document[0][2].getType() == TRIPLE_EMPHASIS);
	BOOST_REQUIRE(document[0][2].getText() == "three");
	BOOST_REQUIRE(document[0][2].size() == 0);
}

// Link ------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE(parse_link_with_simple_example, F) {
	Document document = parser.parse("[one](http://example.net/)");

	BOOST_REQUIRE(document.size() == 1);
	BOOST_REQUIRE(document[0].getType() == PARAGRAPH);
	BOOST_REQUIRE(document[0].getText().length() == 0);
	BOOST_REQUIRE(document[0].size() == 1);
	BOOST_REQUIRE(document[0][0].getType() == LINK);
	BOOST_REQUIRE(document[0][0].getText() == "one");
	BOOST_REQUIRE(document[0][0].getAttribute("link") == "http://example.net/");
	BOOST_REQUIRE(document[0][0].getAttributeNames().size() == 1);
	BOOST_REQUIRE(document[0][0].size() == 0);
}

BOOST_FIXTURE_TEST_CASE(parse_link_with_simple_titled_example, F) {
	Document document = parser.parse("[one](http://example.net/ \"One\")");

	BOOST_REQUIRE(document.size() == 1);
	BOOST_REQUIRE(document[0].getType() == PARAGRAPH);
	BOOST_REQUIRE(document[0].getText().length() == 0);
	BOOST_REQUIRE(document[0].size() == 1);
	BOOST_REQUIRE(document[0][0].getType() == LINK);
	BOOST_REQUIRE(document[0][0].getText() == "one");
	BOOST_REQUIRE(document[0][0].getAttribute("link") == "http://example.net/");
	BOOST_REQUIRE(document[0][0].getAttribute("title") == "One");
	BOOST_REQUIRE(document[0][0].getAttributeNames().size() == 2);
	BOOST_REQUIRE(document[0][0].size() == 0);
}

BOOST_FIXTURE_TEST_CASE(parse_link_with_single_interspersed_simple_example, F) {
	Document document = parser.parse("one [two](http://example.net/) three");

	BOOST_REQUIRE(document.size() == 1);
	BOOST_REQUIRE(document[0].getType() == PARAGRAPH);
	BOOST_REQUIRE(document[0].getText().length() == 0);
	BOOST_REQUIRE(document[0].size() == 3);
	BOOST_REQUIRE(document[0][0].getType() == TEXT);
	BOOST_REQUIRE(document[0][0].getText() == "one ");
	BOOST_REQUIRE(document[0][0].size() == 0);
	BOOST_REQUIRE(document[0][1].getType() == LINK);
	BOOST_REQUIRE(document[0][1].getText() == "two");
	BOOST_REQUIRE(document[0][1].getAttribute("link") == "http://example.net/");
	BOOST_REQUIRE(document[0][1].getAttributeNames().size() == 1);
	BOOST_REQUIRE(document[0][1].size() == 0);
	BOOST_REQUIRE(document[0][2].getType() == TEXT);
	BOOST_REQUIRE(document[0][2].getText() == " three");
	BOOST_REQUIRE(document[0][2].size() == 0);
}

BOOST_FIXTURE_TEST_CASE(parse_link_with_single_interspersed_simple_titled_example, F) {
	Document document = parser.parse("one [two](http://example.net/ \"Two\") three");

	BOOST_REQUIRE(document.size() == 1);
	BOOST_REQUIRE(document[0].getType() == PARAGRAPH);
	BOOST_REQUIRE(document[0].getText().length() == 0);
	BOOST_REQUIRE(document[0].size() == 3);
	BOOST_REQUIRE(document[0][0].getType() == TEXT);
	BOOST_REQUIRE(document[0][0].getText() == "one ");
	BOOST_REQUIRE(document[0][0].size() == 0);
	BOOST_REQUIRE(document[0][1].getType() == LINK);
	BOOST_REQUIRE(document[0][1].getText() == "two");
	BOOST_REQUIRE(document[0][1].getAttribute("link") == "http://example.net/");
	BOOST_REQUIRE(document[0][1].getAttribute("title") == "Two");
	BOOST_REQUIRE(document[0][1].getAttributeNames().size() == 2);
	BOOST_REQUIRE(document[0][1].size() == 0);
	BOOST_REQUIRE(document[0][2].getType() == TEXT);
	BOOST_REQUIRE(document[0][2].getText() == " three");
	BOOST_REQUIRE(document[0][2].size() == 0);
}

BOOST_FIXTURE_TEST_CASE(parse_link_with_multiple_interspersed_simple_example, F) {
	Document document = parser.parse("[one](http://example.net/) two [three](http://example.net/)");

	BOOST_REQUIRE(document.size() == 1);
	BOOST_REQUIRE(document[0].getType() == PARAGRAPH);
	BOOST_REQUIRE(document[0].getText().length() == 0);
	BOOST_REQUIRE(document[0].size() == 3);
	BOOST_REQUIRE(document[0][0].getType() == LINK);
	BOOST_REQUIRE(document[0][0].getText() == "one");
	BOOST_REQUIRE(document[0][0].getAttribute("link") == "http://example.net/");
	BOOST_REQUIRE(document[0][0].getAttributeNames().size() == 1);
	BOOST_REQUIRE(document[0][0].size() == 0);
	BOOST_REQUIRE(document[0][1].getType() == TEXT);
	BOOST_REQUIRE(document[0][1].getText() == " two ");
	BOOST_REQUIRE(document[0][1].size() == 0);
	BOOST_REQUIRE(document[0][2].getType() == LINK);
	BOOST_REQUIRE(document[0][2].getText() == "three");
	BOOST_REQUIRE(document[0][2].getAttribute("link") == "http://example.net/");
	BOOST_REQUIRE(document[0][2].getAttributeNames().size() == 1);
	BOOST_REQUIRE(document[0][2].size() == 0);
}

BOOST_FIXTURE_TEST_CASE(parse_link_with_multiple_interspersed_simple_titled_example, F) {
	Document document = parser.parse("[one](http://example.net/ \"One\") two [three](http://example.net/ \"Three\")");

	BOOST_REQUIRE(document.size() == 1);
	BOOST_REQUIRE(document[0].getType() == PARAGRAPH);
	BOOST_REQUIRE(document[0].getText().length() == 0);
	BOOST_REQUIRE(document[0].size() == 3);
	BOOST_REQUIRE(document[0][0].getType() == LINK);
	BOOST_REQUIRE(document[0][0].getText() == "one");
	BOOST_REQUIRE(document[0][0].getAttribute("link") == "http://example.net/");
	BOOST_REQUIRE(document[0][0].getAttribute("title") == "One");
	BOOST_REQUIRE(document[0][0].getAttributeNames().size() == 2);
	BOOST_REQUIRE(document[0][0].size() == 0);
	BOOST_REQUIRE(document[0][1].getType() == TEXT);
	BOOST_REQUIRE(document[0][1].getText() == " two ");
	BOOST_REQUIRE(document[0][1].size() == 0);
	BOOST_REQUIRE(document[0][2].getType() == LINK);
	BOOST_REQUIRE(document[0][2].getText() == "three");
	BOOST_REQUIRE(document[0][2].getAttribute("link") == "http://example.net/");
	BOOST_REQUIRE(document[0][2].getAttribute("title") == "Three");
	BOOST_REQUIRE(document[0][2].getAttributeNames().size() == 2);
	BOOST_REQUIRE(document[0][2].size() == 0);
}

// Code Span -------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE(parse_code_span_with_simple_example, F) {
	Document document = parser.parse("`one`");

	BOOST_REQUIRE(document.size() == 1);
	BOOST_REQUIRE(document[0].getType() == PARAGRAPH);
	BOOST_REQUIRE(document[0].getText().length() == 0);
	BOOST_REQUIRE(document[0].size() == 1);
	BOOST_REQUIRE(document[0][0].getType() == CODE_SPAN);
	BOOST_REQUIRE(document[0][0].getText() == "one");
	BOOST_REQUIRE(document[0][0].size() == 0);
}

BOOST_FIXTURE_TEST_CASE(parse_single_interspersed_code_span, F) {
	Document document = parser.parse("one `two` three");

	BOOST_REQUIRE(document.size() == 1);
	BOOST_REQUIRE(document[0].getType() == PARAGRAPH);
	BOOST_REQUIRE(document[0].getText().length() == 0);
	BOOST_REQUIRE(document[0].size() == 3);
	BOOST_REQUIRE(document[0][0].getType() == TEXT);
	BOOST_REQUIRE(document[0][0].getText() == "one ");
	BOOST_REQUIRE(document[0][0].size() == 0);
	BOOST_REQUIRE(document[0][1].getType() == CODE_SPAN);
	BOOST_REQUIRE(document[0][1].getText() == "two");
	BOOST_REQUIRE(document[0][1].size() == 0);
	BOOST_REQUIRE(document[0][2].getType() == TEXT);
	BOOST_REQUIRE(document[0][2].getText() == " three");
	BOOST_REQUIRE(document[0][2].size() == 0);
}

BOOST_FIXTURE_TEST_CASE(parse_multiple_interspersed_code_span, F) {
	Document document = parser.parse("`one` two `three`");

	BOOST_REQUIRE(document.size() == 1);
	BOOST_REQUIRE(document[0].getType() == PARAGRAPH);
	BOOST_REQUIRE(document[0].getText().length() == 0);
	BOOST_REQUIRE(document[0].size() == 3);
	BOOST_REQUIRE(document[0][0].getType() == CODE_SPAN);
	BOOST_REQUIRE(document[0][0].getText() == "one");
	BOOST_REQUIRE(document[0][0].size() == 0);
	BOOST_REQUIRE(document[0][1].getType() == TEXT);
	BOOST_REQUIRE(document[0][1].getText() == " two ");
	BOOST_REQUIRE(document[0][1].size() == 0);
	BOOST_REQUIRE(document[0][2].getType() == CODE_SPAN);
	BOOST_REQUIRE(document[0][2].getText() == "three");
	BOOST_REQUIRE(document[0][2].size() == 0);
}

// Line Break ------------------------------------------------------------------
//
// BOOST_FIXTURE_TEST_CASE(parse_simple_linebreak, F) {
// 	Document document = parser.parse("linebreak  \n");
//
// 	BOOST_REQUIRE(document.size() == 1);
// 	BOOST_REQUIRE(document[0].getType() == PARAGRAPH);
// 	BOOST_REQUIRE(document[0].getText().length() == 0);
// 	BOOST_REQUIRE(document[0].size() == 2);
// 	BOOST_REQUIRE(document[0][0].getType() == TEXT);
// 	BOOST_REQUIRE(document[0][0].getText() == "linebreak");
// 	BOOST_REQUIRE(document[0][0].size() == 0);
// 	BOOST_REQUIRE(document[0][1].getType() == LINEBREAK);
// 	BOOST_REQUIRE(document[0][1].getText() == "");
// 	BOOST_REQUIRE(document[0][1].size() == 0);
// }