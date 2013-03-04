/* benchmark.c - main function for markdown module benchmarking */

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

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define READ_UNIT 1024
#define OUTPUT_UNIT 64


/* buffer statistics, to track some memleaks */
extern long buffer_stat_nb;
extern size_t buffer_stat_alloc_bytes;



/* markdown_file • performs markdown transformation on FILE* */
static void
benchmark(FILE *in, int nb) {
	struct buf *ib, *ob;
	size_t ret, i, n;
	if (!in) return;
	n = (nb <= 1) ? 1 : nb;

	/* reading everything */
	ib = bufnew(READ_UNIT);
	bufgrow(ib, READ_UNIT);
	while ((ret = fread(ib->data + ib->size, 1,
			ib->asize - ib->size, in)) > 0) {
		ib->size += ret;
		bufgrow(ib, ib->size + READ_UNIT); }

	/* performing markdown parsing */
	for (i = 0; i < n; i += 1) {
		ob = bufnew(OUTPUT_UNIT);
		ob->size = 0;
		markdown(ob, ib, &mkd_xhtml);
		bufrelease(ob); }

	/* cleanup */
	bufrelease(ib); }



/* main • main function, interfacing STDIO with the parser */
int
main(int argc, char **argv) {
	int nb = 1, i, j, f, files = 0;
	FILE *in = 0;

	/* looking for a count number */
	if (argc > 1) {
		for (i = 1; i < argc; i += 1)
			if (argv[i][0] == '-' && argv[i][1] == '-')
				nb = atoi(argv[i] + 2);
			else files += 1;
		if (nb < 1) {
			fprintf(stderr, "Usage: %s [--<number>] "
					"[file] [file] ...\n", argv[0]);
			return 2; } }

	/* if no file is given, using stdin as the only file */
	if (files <= 0) {
		in = stdin;
		files = 1; }

	/* performing the markdown */
	f = 0;
	for (j = 0; j < files; j += 1) {
		if (in != stdin) {
			f += 1;
			while (f < argc
			&& argv[f][0] == '-' && argv[f][1] == '-')
				f += 1;
			if (f >= argc) break;
			in = fopen(argv[f], "r");
			if (!in) {
				fprintf(stderr, "Unable to open \"%s\": %s\n",
					argv[f], strerror(errno)); 
				continue; } }
		benchmark(in, nb);
		if (in != stdin) fclose(in); }

	/* memory checks */
	if (buffer_stat_nb)
		fprintf(stderr, "Warning: %ld buffers still active\n",
				buffer_stat_nb);
	if (buffer_stat_alloc_bytes)
		fprintf(stderr, "Warning: %zu bytes still allocated\n",
				buffer_stat_alloc_bytes);
	return 0; }

/* vim: set filetype=c: */
