/* mkd2man.c - man-page-formatted output from markdown text */

/*
 * Copyright (c) 2009, Baptiste Daroussin and Natacha Porté
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

#include "markdown.h"

#include <stdio.h>
#include <errno.h>
#include <string.h>

#define READ_UNIT 1024
#define OUTPUT_UNIT 64


/****************************
 * MARKDOWN TO MAN RENDERER *
 ****************************/

void
man_text_escape(struct buf *ob, char *src, size_t size) {
	size_t  i = 0, org;
	while (i < size) {
		/* copying directly unescaped characters */
		org = i;
		while (i < size && src[i] != '-')
			i += 1;
		if (i > org) bufput(ob, src + org, i - org);

		/* escaping */
		if (i >= size) break;
		else if (src[i] == '-') BUFPUTSL(ob, "\\-");
		i += 1; } }

static void
man_blockcode(struct buf *ob, struct buf *text, void *opaque) {
	if (ob->size) bufputc(ob, '\n');
	BUFPUTSL(ob, ".nf\n");
	if (text) man_text_escape(ob, text->data, text->size);
	BUFPUTSL(ob, ".fi\n"); }

static int
man_codespan(struct buf *ob, struct buf *text, void *opaque) {
	if (ob->size) bufputc(ob, '\n');
	BUFPUTSL(ob, ".nf\n");
	if (text) man_text_escape(ob, text->data, text->size);
	BUFPUTSL(ob, ".fi\n"); 
	return 1; }

static void
man_header(struct buf *ob, struct buf *text, int level, void *opaque) {
	if (ob->size) bufputc(ob, '\n');
	switch(level) {
		case 1:
			bufprintf(ob,".TH "); 
			break;
		case 2:
			bufprintf(ob, ".SH ");
			break;
		case 3:
			bufprintf(ob, ".SS ");
			break;
	}
	if (text) bufput(ob, text->data, text->size);
}

static int
man_double_emphasis(struct buf *ob, struct buf *text, char c, void *opaque) {
	if (!text || !text->size) return 0;
	BUFPUTSL(ob, "\\fB");
	bufput(ob, text->data, text->size);
	BUFPUTSL(ob, "\\fP");
	return 1; }

static int
man_emphasis(struct buf *ob, struct buf *text, char c, void *opaque) {
	if (!text || !text->size) return 0;
	BUFPUTSL(ob, "\\fI");
	if (text) bufput(ob, text->data, text->size);
	BUFPUTSL(ob, "\\fP");
	return 1; }

static int
man_linebreak(struct buf *ob, void *opaque) {
	BUFPUTSL(ob, "\n.LP\n");
	return 1; }

static void
man_paragraph(struct buf *ob, struct buf *text, void *opaque) {
	if (ob->size) bufputc(ob, '\n');
	BUFPUTSL(ob, ".TP\n");
	if (text) bufput(ob, text->data, text->size);
	BUFPUTSL(ob, "\n"); }

static void
man_list(struct buf *ob, struct buf *text, int flags, void *opaque) {
	if (ob->size) bufputc(ob, '\n');
	if (flags & MKD_LIST_ORDERED)
		BUFPUTSL(ob,"\n.nr step 0 1\n");
	else
		BUFPUTSL(ob,".\n");
	if (text) bufput(ob, text->data, text->size);
	BUFPUTSL(ob, "\n"); }

static void
man_listitem(struct buf *ob, struct buf *text, int flags, void *opaque) {
	if (flags & MKD_LIST_ORDERED)
		BUFPUTSL(ob, ".IP \\n+[step]\n");
	else
		BUFPUTSL(ob, ".IP \\[bu] 2 \n");

	if (text) {
		while (text->size && text->data[text->size - 1] == '\n')
			text->size -= 1;
		bufput(ob, text->data, text->size); }
	BUFPUTSL(ob, "\n"); }

static void
man_normal_text(struct buf *ob, struct buf *text, void *opaque) {
	if (text) man_text_escape(ob, text->data, text->size); }


/* renderer structure */
struct mkd_renderer to_man = {
	/* document-level callbacks */
	NULL,
	NULL,

	/* block-level callbacks */
	man_blockcode,
	NULL,
	NULL,
	man_header,
	NULL,
	man_list,
	man_listitem,
	man_paragraph,
	NULL,
	NULL,
	NULL,

	/* span-level callbacks */
	NULL,
	man_codespan,
	man_double_emphasis,
	man_emphasis,
	NULL,
	man_linebreak,
	NULL,
	NULL,
	NULL,

	/* low-level callbacks */
	NULL,
	man_normal_text,

	/* renderer data */
	64,
	"*_",
	NULL };



/*****************
 * MAIN FUNCTION *
 *****************/

/* main • main function, interfacing STDIO with the parser */
int
main(int argc, char **argv) {
	struct buf *ib, *ob;
	size_t ret;
	FILE *in = stdin;

	/* opening the file if given from the command line */
	if (argc > 1) {
		in = fopen(argv[1], "r");
		if (!in) {
			fprintf(stderr,"Unable to open input file \"%s\": %s\n",
				argv[1], strerror(errno));
			return 1; } }

	/* reading everything */
	ib = bufnew(READ_UNIT);
	bufgrow(ib, READ_UNIT);
	while ((ret = fread(ib->data + ib->size, 1,
			ib->asize - ib->size, in)) > 0) {
		ib->size += ret;
		bufgrow(ib, ib->size + READ_UNIT); }
	if (in != stdin) fclose(in);

	/* performing markdown to man */
	ob = bufnew(OUTPUT_UNIT);
	markdown(ob, ib, &to_man);

	/* writing the result to stdout */
	ret = fwrite(ob->data, 1, ob->size, stdout);
	if (ret < ob->size)
		fprintf(stderr, "Warning: only %zu output byte written, "
				"out of %zu\n",
				ret,
				ob->size);

	/* cleanup */
	bufrelease(ib);
	bufrelease(ob);
	return 0; }

/* vim: set filetype=c: */
