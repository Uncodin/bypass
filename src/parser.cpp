#include "parser.h"

using namespace std;

namespace Bypass
{

	Parser::Parser()
	{

	}

	Parser::~Parser()
	{

	}

	Document
	Parser::parse(const char *str)
	{
		Document document;

		struct buf *ib, *ob;

		ib = bufnew(INPUT_UNIT);
		bufputs(ib, str);

		ob = bufnew(OUTPUT_UNIT);

		mkd_callbacks.opaque = this;
		//parse and assemble document
		markdown(ob, ib, &mkd_callbacks);

		// We have finished parsing, move any data left in the temp string to the main string

		/* cleanup */
		bufrelease(ib);
		bufrelease(ob);

		return document;
	}

	Document
	Parser::parse(const string &str)
	{
		return Parser::parse(str.c_str());
	}

}

//Call this from block level elements to see if we need to move the
//Buffered data to the main string
static void checkDataMove(struct buf *ob) {
	if (ob->size == 0) {
		//move data off temp string and onto main
	}
}

/*
 *	Block Element callbacks
 */

static void rndr_blockcode(struct buf *ob, struct buf *text, void *opaque) {

	checkDataMove(ob);
}

static void rndr_header(struct buf *ob, struct buf *text, int level, void *opaque) {
	//check if the text contains a hash from the last block level
	//if it exists make a new container level and stick the old temp string into it

	//if there is no hash
	checkDataMove(ob);
}

static void rndr_list(struct buf *ob, struct buf *text, int flags, void *opaque) {

	checkDataMove(ob);
}

static void rndr_listitem(struct buf *ob, struct buf *text, int flags, void *opaque) {

	checkDataMove(ob);
}

static void rndr_paragraph(struct buf *ob, struct buf *text, void *opaque) {

	checkDataMove(ob);
}

/*
 *	Span Element callbacks
 */

static int rndr_codespan(struct buf *ob, struct buf *text, void *opaque) {
	return 0;
}

static int rndr_double_emphasis(struct buf *ob, struct buf *text, char c,
		void *opaque) {
	return 0;
}

static int rndr_emphasis(struct buf *ob, struct buf *text, char c,
		void *opaque) {
	return 0;
}

static int rndr_triple_emphasis(struct buf *ob, struct buf *text, char c,
		void *opaque) {
	return 0;
}

static int rndr_linebreak(struct buf *ob, void *opaque) {
	return 0;
}

static int rndr_link(struct buf *ob, struct buf *link, struct buf *title,
			struct buf *content, void *opaque) {
	return 0;
}

/*
 *	Low Level Callbacks
 */

static void rndr_normal_text(struct buf *ob, struct buf *text, void *opaque) {

}

