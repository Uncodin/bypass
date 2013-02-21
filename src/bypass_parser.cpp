#include <string>

extern "C" {
#include "markdown.h"
}

#include "bypass_parser.h"

using namespace std;

BypassParser::BypassParser()
{

}

BypassParser::~BypassParser()
{

}

Document
BypassParser::parse(const char *markdown)
{
	if (!markdown)
	{
		return BypassParser::parse(string());
	}

	return BypassParser::parse(string(markdown));
}

Document
BypassParser::parse(const string &markdown)
{
	Document document;

	//parse and assemble document
	//markdown(ob, ib, &to_spannable);

	// We have finished parsing, move any data left in the temp string to the main string

	return document;
}

//Call this from block level elements to see if we need to move the
//Buffered data to the main string
static void checkDataMove(struct buf *ob) {
	if (ob->size == 0) {
		//move data off temp string and onto main
	}
}

static void rndr_blockcode(struct buf *ob, struct buf *text, void *opaque) {
}

static void rndr_header(struct buf *ob, struct buf *text, int level, void *opaque) {
	//check if the text contains a hash from the last block level
	//if it exists make a new container level and stick the old temp string into it

	//if there is no hash
	checkDataMove(ob);
}

static void rndr_list(struct buf *ob, struct buf *text, int flags, void *opaque) {

}

static void rndr_listitem(struct buf *ob, struct buf *text, int flags, void *opaque) {

}

static void rndr_paragraph(struct buf *ob, struct buf *text, void *opaque) {
	checkDataMove(ob);
}

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

static void rndr_normal_text(struct buf *ob, struct buf *text, void *opaque) {

}

struct mkd_renderer to_spannable = {
	/* document-level callbacks */
	NULL, // prolog
	NULL, // epilogue

	/* block-level callbacks */
	rndr_blockcode, // BlockCode
	NULL, // blockQuote
	NULL, // block html
	rndr_header, // header
	NULL, // hrule
	rndr_list, // list
	rndr_listitem, // listitem
	rndr_paragraph, // paragraph
	NULL, // table
	NULL, // table cell
	NULL, // table row

	/* span-level callbacks */
	NULL, // autolink
	rndr_codespan, // codespan
	rndr_double_emphasis, // double_emphasis
	rndr_emphasis, // emphasis
	NULL, // image
	rndr_linebreak, // line break
	rndr_link, // link
	NULL, // raw html tag
	rndr_triple_emphasis, // triple emphasis

	/* low-level callbacks */
	NULL, // entity
	rndr_normal_text, // normal text

	/* renderer data */
	64, // max stack
	"*_",
	NULL // opaque
};
