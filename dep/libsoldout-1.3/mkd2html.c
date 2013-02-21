/* main.c - main function for markdown module testing */

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

#include "markdown.h"
#include "renderers.h"

#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define READ_UNIT 1024
#define OUTPUT_UNIT 64


/* buffer statistics, to track some memleaks */
extern long buffer_stat_nb;
extern size_t buffer_stat_alloc_bytes;



/* usage • print the option list */
void
usage(FILE *out, const char *name) {
	fprintf(out, "Usage: %s [-h | -x] [-d | -m | -n] [input-file]\n\n",
	    name);
	fprintf(out, "\t-d, --discount\n"
	    "\t\tEnable some Discount extensions (image size specficiation,\n"
	    "\t\tclass blocks and 'abbr:', 'class:', 'id:' and 'raw:'\n"
	    "\t\tpseudo-protocols)\n"
	    "\t-H, --html\n"
	    "\t\tOutput HTML-style self-closing tags (e.g. <br>)\n"
	    "\t-h, --help\n"
	    "\t\tDisplay this help text and exit without further processing\n"
	    "\t-m, --markdown\n"
	    "\t\tDisable all extensions and use strict markdown syntax\n"
	    "\t-n, --natext\n"
	    "\t\tEnable support Discount extensions and Natasha's own\n"
	    "\t\textensions (id header attribute, class paragraph attribute,\n"
	    "\t\t'ins' and 'del' elements, and plain span elements)\n"
	    "\t-x, --xhtml\n"
	    "\t\tOutput XHTML-style self-closing tags (e.g. <br />)\n"); }



/* main • main function, interfacing STDIO with the parser */
int
main(int argc, char **argv) {
	struct buf *ib, *ob;
	size_t ret;
	FILE *in = stdin;
	const struct mkd_renderer *hrndr, *xrndr;
	const struct mkd_renderer **prndr;
	int ch, argerr, help;
	struct option longopts[] = {
	    { "discount",	no_argument,	0,	'd' },
	    { "html",		no_argument,	0,	'H' },
	    { "help",		no_argument,	0,	'h' },
	    { "markdown",	no_argument,	0,	'm' },
	    { "natext",		no_argument,	0,	'n' },
	    { "xhtml",		no_argument,	0,	'x' },
	    { 0,		0,		0,	0 } };

	/* default options: strict markdown input, HTML output */
	hrndr = &mkd_html;
	xrndr = &mkd_xhtml;
	prndr = &hrndr;

	/* argument parsing */
	argerr = help = 0;
	while (!argerr &&
	    (ch = getopt_long(argc, argv, "dHhmnx", longopts, 0)) != -1)
		switch (ch) {
		    case 'd': /* discount extension */
			hrndr = &discount_html;
			xrndr = &discount_xhtml;
			break;
		    case 'H': /* HTML output */
			prndr = &hrndr;
			break;
		    case 'h': /* display help */
			argerr = help = 1;
			break;
		    case 'm': /* strict markdown */
			hrndr = &mkd_html;
			xrndr = &mkd_xhtml;
			break;
		    case 'n': /* Discount + Natasha's extensions */
			hrndr = &nat_html;
			xrndr = &nat_xhtml;
			break;
		    case 'x': /* XHTML output */
			prndr = &xrndr;
			break;
		    default:
			argerr = 1; }
	if (argerr) {
		usage(help ? stdout : stderr, argv[0]);
		return help ? EXIT_SUCCESS : EXIT_FAILURE; }
	argc -= optind;
	argv += optind;

	/* opening the file if given from the command line */
	if (argc > 0) {
		in = fopen(argv[0], "r");
		if (!in) {
			fprintf(stderr,"Unable to open input file \"%s\": %s\n",
				argv[0], strerror(errno));
			return 1; } }

	/* reading everything */
	ib = bufnew(READ_UNIT);
	bufgrow(ib, READ_UNIT);
	while ((ret = fread(ib->data + ib->size, 1,
			ib->asize - ib->size, in)) > 0) {
		ib->size += ret;
		bufgrow(ib, ib->size + READ_UNIT); }
	if (in != stdin) fclose(in);

	/* performing markdown parsing */
	ob = bufnew(OUTPUT_UNIT);
	markdown(ob, ib, *prndr);

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

	/* memory checks */
	if (buffer_stat_nb)
		fprintf(stderr, "Warning: %ld buffers still active\n",
				buffer_stat_nb);
	if (buffer_stat_alloc_bytes)
		fprintf(stderr, "Warning: %zu bytes still allocated\n",
				buffer_stat_alloc_bytes);
	return 0; }

/* vim: set filetype=c: */
