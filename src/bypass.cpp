extern "C" {
#include "markdown.h"
}

using namespace std;

struct mkd_renderer to_spannable = {
	/* document-level callbacks */
	NULL,
	NULL,

	/* block-level callbacks */
	NULL, //BlockCode
	NULL, //blockQuote
	NULL, // block html
	NULL, // header
	NULL, //hrule
	NULL, //list
	NULL, //listitem
	NULL, //paragraph
	NULL, //table
	NULL, //table cell
	NULL, // table row

	/* span-level callbacks */
	NULL, // autolink
	NULL, //codespan
	NULL, //double_emphasis
	NULL, //emphasis
	NULL, //image
	NULL, //line break
	NULL, //link
	NULL, //raw html tag
	NULL, // triple emphasis

	/* low-level callbacks */
	NULL, //entity
	NULL, //normal text

	/* renderer data */
	64, // max stack
	"*_", 
	NULL //opaque
};

