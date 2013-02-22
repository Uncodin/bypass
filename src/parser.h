#ifndef _BYPASS_PARSER_H_
#define _BYPASS_PARSER_H_

#include <string>

extern "C" { 
#include "markdown.h" 
} 

#include "document.h"

#define INPUT_UNIT 1024 
#define OUTPUT_UNIT 64

namespace Bypass
{

	class Parser
	{
	public:
		Parser();
		~Parser();
		Document parse(const char *markdown);
		Document parse(const std::string &markdown);
	};

}

/*
 *	Block Element callbacks
 */

static void rndr_blockcode(struct buf *ob, struct buf *text, void *opaque);

static void rndr_header(struct buf *ob, struct buf *text, int level, void *opaque);

static void rndr_list(struct buf *ob, struct buf *text, int flags, void *opaque);

static void rndr_listitem(struct buf *ob, struct buf *text, int flags, void *opaque);

static void rndr_paragraph(struct buf *ob, struct buf *text, void *opaque);

/*
 *	Span Element callbacks
 */

static int rndr_codespan(struct buf *ob, struct buf *text, void *opaque);

static int rndr_double_emphasis(struct buf *ob, struct buf *text, char c,
		void *opaque);

static int rndr_emphasis(struct buf *ob, struct buf *text, char c,
		void *opaque);

static int rndr_triple_emphasis(struct buf *ob, struct buf *text, char c,
		void *opaque);

static int rndr_linebreak(struct buf *ob, void *opaque);

static int rndr_link(struct buf *ob, struct buf *link, struct buf *title,
			struct buf *content, void *opaque);

/*
 *	Low Level Callbacks
 */

static void rndr_normal_text(struct buf *ob, struct buf *text, void *opaque);

struct mkd_renderer mkd_callbacks = {
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

#endif // _BYPASS_PARSER_H_
