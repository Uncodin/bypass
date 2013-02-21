/* markdown.h - generic markdown parser */

/*
 * Copyright (c) 2009, Natacha Porté
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef LITHIUM_MARKDOWN_H
#define LITHIUM_MARKDOWN_H

#include "buffer.h"


/********************
 * TYPE DEFINITIONS *
 ********************/

/* mkd_autolink • type of autolink */
enum mkd_autolink {
	MKDA_NOT_AUTOLINK,	/* used internally when it is not an autolink*/
	MKDA_NORMAL,		/* normal http/http/ftp/etc link */
	MKDA_EXPLICIT_EMAIL,	/* e-mail link with explit mailto: */
	MKDA_IMPLICIT_EMAIL	/* e-mail link without mailto: */
};

/* mkd_renderer • functions for rendering parsed data */
struct mkd_renderer {
	/* document level callbacks */
	void (*prolog)(struct buf *ob, void *opaque);
	void (*epilog)(struct buf *ob, void *opaque);

	/* block level callbacks - NULL skips the block */
	void (*blockcode)(struct buf *ob, struct buf *text, void *opaque);
	void (*blockquote)(struct buf *ob, struct buf *text, void *opaque);
	void (*blockhtml)(struct buf *ob, struct buf *text, void *opaque);
	void (*header)(struct buf *ob, struct buf *text,
						int level, void *opaque);
	void (*hrule)(struct buf *ob, void *opaque);
	void (*list)(struct buf *ob, struct buf *text, int flags, void *opaque);
	void (*listitem)(struct buf *ob, struct buf *text,
						int flags, void *opaque);
	void (*paragraph)(struct buf *ob, struct buf *text, void *opaque);
	void (*table)(struct buf *ob, struct buf *head_row, struct buf *rows,
							void *opaque);
	void (*table_cell)(struct buf *ob, struct buf *text, int flags,
							void *opaque);
	void (*table_row)(struct buf *ob, struct buf *cells, int flags,
							void *opaque);

	/* span level callbacks - NULL or return 0 prints the span verbatim */
	int (*autolink)(struct buf *ob, struct buf *link,
					enum mkd_autolink type, void *opaque);
	int (*codespan)(struct buf *ob, struct buf *text, void *opaque);
	int (*double_emphasis)(struct buf *ob, struct buf *text,
						char c, void *opaque);
	int (*emphasis)(struct buf *ob, struct buf *text, char c,void*opaque);
	int (*image)(struct buf *ob, struct buf *link, struct buf *title,
						struct buf *alt, void *opaque);
	int (*linebreak)(struct buf *ob, void *opaque);
	int (*link)(struct buf *ob, struct buf *link, struct buf *title,
					struct buf *content, void *opaque);
	int (*raw_html_tag)(struct buf *ob, struct buf *tag, void *opaque);
	int (*triple_emphasis)(struct buf *ob, struct buf *text,
						char c, void *opaque);

	/* low level callbacks - NULL copies input directly into the output */
	void (*entity)(struct buf *ob, struct buf *entity, void *opaque);
	void (*normal_text)(struct buf *ob, struct buf *text, void *opaque);

	/* renderer data */
	int max_work_stack; /* prevent arbitrary deep recursion, cf README */
	const char *emph_chars; /* chars that trigger emphasis rendering */
	void *opaque; /* opaque data send to every rendering callback */
};



/*********
 * FLAGS *
 *********/

/* list/listitem flags */
#define MKD_LIST_ORDERED	1
#define MKD_LI_BLOCK		2  /* <li> containing block data */

/* table cell flags */
#define MKD_CELL_ALIGN_DEFAULT	0
#define MKD_CELL_ALIGN_LEFT	1
#define MKD_CELL_ALIGN_RIGHT	2
#define MKD_CELL_ALIGN_CENTER	3  /* LEFT | RIGHT */
#define MKD_CELL_ALIGN_MASK	3
#define MKD_CELL_HEAD		4



/**********************
 * EXPORTED FUNCTIONS *
 **********************/

/* markdown • parses the input buffer and renders it into the output buffer */
void
markdown(struct buf *ob, struct buf *ib, const struct mkd_renderer *rndr);


#endif /* ndef LITHIUM_MARKDOWN_H */

/* vim: set filetype=c: */
