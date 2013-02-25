#define BOOST_TEST_MODULE DocumentTests

#include <boost/test/included/unit_test.hpp>
#include <string>
#include <stdio.h>
#include "element.h"

using namespace Bypass;

struct F {
	F() : element() {
		BOOST_TEST_MESSAGE( "setup fixture" );
	}

	~F() {
		BOOST_TEST_MESSAGE( "teardown fixture" );
	}

	Element element;
};

BOOST_FIXTURE_TEST_CASE(element_initial_size, F) {
	BOOST_REQUIRE(element.size() == 0);
}

BOOST_FIXTURE_TEST_CASE(element_block_code_type, F) {
	element.setType(BLOCK_CODE);

	BOOST_REQUIRE(element.isBlockElement());
	BOOST_REQUIRE(!element.isSpanElement());
}

BOOST_FIXTURE_TEST_CASE(element_block_quote_type, F) {
	element.setType(BLOCK_QUOTE);

	BOOST_REQUIRE(element.isBlockElement());
	BOOST_REQUIRE(!element.isSpanElement());
}

BOOST_FIXTURE_TEST_CASE(element_block_html_type, F) {
	element.setType(BLOCK_HTML);

	BOOST_REQUIRE(element.isBlockElement());
	BOOST_REQUIRE(!element.isSpanElement());
}

BOOST_FIXTURE_TEST_CASE(element_header_type, F) {
	element.setType(HEADER);

	BOOST_REQUIRE(element.isBlockElement());
	BOOST_REQUIRE(!element.isSpanElement());
}

BOOST_FIXTURE_TEST_CASE(element_hrule_type, F) {
	element.setType(HRULE);

	BOOST_REQUIRE(element.isBlockElement());
	BOOST_REQUIRE(!element.isSpanElement());
}

BOOST_FIXTURE_TEST_CASE(element_list_type, F) {
	element.setType(LIST);

	BOOST_REQUIRE(element.isBlockElement());
	BOOST_REQUIRE(!element.isSpanElement());
}

BOOST_FIXTURE_TEST_CASE(element_list_item_type, F) {
	element.setType(LIST_ITEM);

	BOOST_REQUIRE(element.isBlockElement());
	BOOST_REQUIRE(!element.isSpanElement());
}

BOOST_FIXTURE_TEST_CASE(element_paragraph_type, F) {
	element.setType(PARAGRAPH);

	BOOST_REQUIRE(element.isBlockElement());
	BOOST_REQUIRE(!element.isSpanElement());
}

BOOST_FIXTURE_TEST_CASE(element_table_type, F) {
	element.setType(TABLE);

	BOOST_REQUIRE(element.isBlockElement());
	BOOST_REQUIRE(!element.isSpanElement());
}

BOOST_FIXTURE_TEST_CASE(element_table_cell_type, F) {
	element.setType(TABLE_CELL);

	BOOST_REQUIRE(element.isBlockElement());
	BOOST_REQUIRE(!element.isSpanElement());
}

BOOST_FIXTURE_TEST_CASE(element_table_row_type, F) {
	element.setType(TABLE_ROW);

	BOOST_REQUIRE(element.isBlockElement());
	BOOST_REQUIRE(!element.isSpanElement());
}

BOOST_FIXTURE_TEST_CASE(element_autolink_type, F) {
	element.setType(AUTOLINK);

	BOOST_REQUIRE(!element.isBlockElement());
	BOOST_REQUIRE(element.isSpanElement());
}

BOOST_FIXTURE_TEST_CASE(element_code_span_type, F) {
	element.setType(CODE_SPAN);

	BOOST_REQUIRE(!element.isBlockElement());
	BOOST_REQUIRE(element.isSpanElement());
}

BOOST_FIXTURE_TEST_CASE(element_double_emphasis_type, F) {
	element.setType(DOUBLE_EMPHASIS);

	BOOST_REQUIRE(!element.isBlockElement());
	BOOST_REQUIRE(element.isSpanElement());
}

BOOST_FIXTURE_TEST_CASE(element_emphasis_type, F) {
	element.setType(EMPHASIS);

	BOOST_REQUIRE(!element.isBlockElement());
	BOOST_REQUIRE(element.isSpanElement());
}

BOOST_FIXTURE_TEST_CASE(element_image_type, F) {
	element.setType(IMAGE);

	BOOST_REQUIRE(!element.isBlockElement());
	BOOST_REQUIRE(element.isSpanElement());
}

BOOST_FIXTURE_TEST_CASE(element_linebreak_type, F) {
	element.setType(LINEBREAK);

	BOOST_REQUIRE(!element.isBlockElement());
	BOOST_REQUIRE(element.isSpanElement());
}

BOOST_FIXTURE_TEST_CASE(element_link_type, F) {
	element.setType(LINK);

	BOOST_REQUIRE(!element.isBlockElement());
	BOOST_REQUIRE(element.isSpanElement());
}

BOOST_FIXTURE_TEST_CASE(element_raw_html_type, F) {
	element.setType(RAW_HTML_TAG);

	BOOST_REQUIRE(!element.isBlockElement());
	BOOST_REQUIRE(element.isSpanElement());
}

BOOST_FIXTURE_TEST_CASE(element_triple_emphasis_type, F) {
	element.setType(TRIPLE_EMPHASIS);

	BOOST_REQUIRE(!element.isBlockElement());
	BOOST_REQUIRE(element.isSpanElement());
}

BOOST_FIXTURE_TEST_CASE(element_text_type, F) {
	element.setType(TEXT);

	BOOST_REQUIRE(!element.isBlockElement());
	BOOST_REQUIRE(element.isSpanElement());
}