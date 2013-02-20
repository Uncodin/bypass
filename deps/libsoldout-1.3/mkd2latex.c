/* mkd2latex.c - LaTeX-formatted output from markdown text */

/*
 * Copyright (c) 2009, Baptiste Daroussin, Natacha Porté, and Michael Huang
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
 
 /*
  * Links require the hyperref package, and images require the graphicx
  * package.
  */

#include "markdown.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define READ_UNIT 1024
#define OUTPUT_UNIT 64

/*********************
 * ENTITY CONVERSION *
 *********************/

struct str_pair {
	const char *entity;
	const char *latex; };

static struct str_pair entity_latex[] = {
	{ "&AElig;",	"\\AE{}" },
	{ "&Aacute;",	"\\'A" },
	{ "&Acirc;",	"\\^A" },
	{ "&Agrave;",	"\\`A" },
	{ "&Aring;",	"\\AA{}" },
	{ "&Atilde;",	"\\~A" },
	{ "&Auml;",	"\\\"A" },
	{ "&Ccedil;",	"\\c{C}" },
//	{ "&ETH;",	"\\DH{}" },			// not available in OT1
	{ "&Eacute;",	"\\'E" },
	{ "&Ecirc;",	"\\^E" },
	{ "&Egrave;",	"\\`E" },
	{ "&Euml;",	"\\\"E" },
	{ "&Iacute;",	"\\'I" },
	{ "&Icirc;",	"\\^I" },
	{ "&Igrave;",	"\\`I" },
	{ "&Iuml;",	"\\\"I" },
	{ "&Ntilde;",	"\\~N" },
	{ "&Oacute;",	"\\'O" },
	{ "&Ocirc;",	"\\^O" },
	{ "&Ograve;",	"\\`O" },
	{ "&Oslash;",	"\\O{}" },
	{ "&Otilde;",	"\\~O" },
	{ "&Ouml;",	"\\\"O" },
//	{ "&THORN;",	"\\TH{}" },			// not available in OT1
	{ "&Uacute;",	"\\'U" },
	{ "&Ucirc;",	"\\^U" },
	{ "&Ugrave;",	"\\`U" },
	{ "&Uuml;",	"\\\"U" },
	{ "&Ygrave;",	"\\`Y" },
	{ "&aacute;",	"\\'a" },
	{ "&acirc;",	"\\^a" },
//	{ "&acute;",	"\\textasciiacute{}" },		// requires textcomp
	{ "&aelig;",	"\\ae{}" },
	{ "&agrave;",	"\\`a" },
	{ "&amp;",	"\\&" },
	{ "&apos;",	"'" },
	{ "&aring;",	"\\aa{}" },
	{ "&atilde;",	"\\~a" },
	{ "&auml;",	"\\\"a" },
//	{ "&brvbar;",	"\\textbrokenbar{}" },		// requires textcomp
	{ "&ccedil;",	"\\c{c}" },
	{ "&cedil;",	"\\c{}" },
//	{ "&cent;",	"\\textcent{}" },		// requires textcomp
	{ "&copy;",	"\\copyright{}" },
//	{ "&curren;",	"\\textcurrency{}" },		// requires textcomp
//	{ "&deg;",	"\\textdegree{}" },		// requires textcomp
//	{ "&divide;",	"\\textdiv{}" },		// requires textcomp
	{ "&eacute;",	"\\'e" },
	{ "&ecirc;",	"\\^e" },
	{ "&egrave;",	"\\`e" },
//	{ "&eth;",	"\\dh{}" }.			// not available in OT1
	{ "&euml;",	"\\\"e" },
//	{ "&frac12;",	"\\textonehalf{}" },		// requires textcomp
//	{ "&frac14;",	"\\textonequarter{}" },		// requires textcomp
//	{ "&frac34;",	"\\textthreequarter{}" },	// requires textcomp
	{ "&gt;",	"$>$" },
	{ "&iacute;",	"\\'\\i{}" },
	{ "&icirc;",	"\\^\\i{}" },
	{ "&iexcl;",	"\\textexclamdown{}" },
	{ "&igrave;",	"\\`\\i{}" },
	{ "&iquest;",	"\\" },
	{ "&iuml;",	"\\\"\\i{}" },
//	{ "&laquo;",	"\\guillemotleft{}" },		// not available in OT1
	{ "&lt;",	"$<$" },
//	{ "&macr;",	"\\textasciimacaron{}" },	// requires textcomp
//	{ "&micro;",	"\\textmu{}"},			// requires textcomp
	{ "&middot;",	"\\textperiodcentered{}" },
	{ "&nbsp;",	"~" },
//	{ "&not;",	"\\textlnot{}" },		// requires textcomp
	{ "&ntilde;",	"\\~n" },
	{ "&oacute;",	"\\'o" },
	{ "&ocirc;",	"\\^o" },
	{ "&ograve;",	"\\`o" },
	{ "&ordf;",	"\\textordfeminine{}" },
	{ "&ordm;",	"\\textordmasculine{}" },
	{ "&oslash;",	"\\o{}" },
	{ "&otilde;",	"\\~o" },
	{ "&ouml;",	"\\\"o" },
	{ "&para;",	"\\P{}" },
//	{ "&plusmn;",	"\\textpm{}" },			// requires textcomp
	{ "&pound;",	"\\textsterling{}" },
	{ "&quot;",	"\"" },
//	{ "&raquo;",	"\\guillemotright{}" },		// not available in OT1
	{ "&reg;",	"\\textregistered{}" },
	{ "&sect;",	"\\S{}" },
	{ "&shy;",	"\\-" },
//	{ "&sup1;",	"\\textonesuperior{}" },	// requires textcomp
//	{ "&sup2;",	"\\texttwosuperior{}" },	// requires textcomp
//	{ "&sup3;",	"\\textthreesuperior{}" },	// requires textcomp
	{ "&szlig;",	"\\ss{}" },
//	{ "&thorn;",	"\\th{}" },			// not available in OT1
//	{ "&times;",	"\\texttimes{}" },		// requires textcomp
	{ "&uacute;",	"\\'u" },
	{ "&ucirc;",	"\\^u" },
	{ "&ugrave;",	"\\`u" },
//	{ "&uml;",	"\\textasciidieresis{}" },	// requires textcomp
	{ "&uuml;",	"\\\"u" },
	{ "&yacute;",	"\\'y" },
//	{ "&yen;",	"\\textyen{}" },		// requires textcomp
	{ "&yuml;",	"\\\"y" },
};

static int cmp_entity(const void *key, const void *element) {
	const struct str_pair *pair = element;
	const struct buf *entity = key;
	return bufcmps(entity, pair->entity); }

static const char *entity2latex(const struct buf *entity) {
	const struct str_pair *pair;
	pair = bsearch(entity, entity_latex,
	    sizeof entity_latex / sizeof *entity_latex,
	    sizeof *entity_latex,
	    &cmp_entity);
	return pair ? pair->latex : 0; }



/******************************
 * MARKDOWN TO LATEX RENDERER *
 ******************************/

void
latex_text_escape(struct buf *ob, char *src, size_t size) {
	size_t  i = 0, org;
	while (i < size) {
		/* copying directly unescaped characters */
		org = i;
		while (i < size && src[i] != '&' && src[i] != '%'
		&& src[i] != '$' && src[i] != '#' && src[i] != '_'
		&& src[i] != '{' && src[i] != '}' && src[i] != '~'
		&& src[i] != '^' && src[i] != '\\' && src[i] != '<'
		&& src[i] != '>')
			i += 1;
		if (i > org) bufput(ob, src + org, i - org);

		/* escaping */
		if (i >= size) break;
		else if (src[i] == '&') BUFPUTSL(ob, "\\&");
		else if (src[i] == '%') BUFPUTSL(ob, "\\%");
		else if (src[i] == '$') BUFPUTSL(ob, "\\$");
		else if (src[i] == '#') BUFPUTSL(ob, "\\#");
		else if (src[i] == '_') BUFPUTSL(ob, "\\_");
		else if (src[i] == '{') BUFPUTSL(ob, "\\{");
		else if (src[i] == '}') BUFPUTSL(ob, "\\}");
		else if (src[i] == '<') BUFPUTSL(ob, "$<$");
		else if (src[i] == '>') BUFPUTSL(ob, "$<$");
		else if (src[i] == '~') BUFPUTSL(ob, "\\textasciitilde{}");
		else if (src[i] == '^') BUFPUTSL(ob, "\\textasciicircum{}");
		else if (src[i] == '\\') BUFPUTSL(ob, "\\textbackslash{}");
		i += 1; } }

static void
latex_prolog(struct buf *ob, void *opaque) {
	BUFPUTSL(ob,
		"\\documentclass{article}\n"
		"\\usepackage{hyperref}\n"
		"\\usepackage{graphicx}\n"
		"\\begin{document}\n"); }

static void
latex_epilog(struct buf *ob, void *opaque) {
	BUFPUTSL(ob, "\n\\end{document}\n"); }

static int
latex_autolink(struct buf *ob, struct buf *link, enum mkd_autolink type,
						void *opaque) {
	if (!link || !link->size) return 0;
	BUFPUTSL(ob, "\\href{");
	if (type == MKDA_IMPLICIT_EMAIL) BUFPUTSL(ob, "mailto:");
		bufput(ob, link->data, link->size);
	BUFPUTSL(ob, "}{");
	if (type == MKDA_EXPLICIT_EMAIL && link->size > 7)
		latex_text_escape(ob, link->data + 7, link->size - 7);
	else	latex_text_escape(ob, link->data, link->size);
	BUFPUTSL(ob, "}");
	return 1; }
	
static int
latex_link(struct buf *ob, struct buf *link, struct buf *title,
			struct buf *content, void *opaque) {
	BUFPUTSL(ob, "\\href{");
	if (link && link->size) bufput(ob, link->data, link->size);
	BUFPUTSL(ob, "}{");
	if (content && content->size)
		bufput(ob, content->data, content->size);
	BUFPUTSL(ob, "}");
	return 1; }

static int
latex_image(struct buf *ob, struct buf *link, struct buf *title,
			struct buf *alt, void *opaque) {
	if (!link || !link->size) return 0;
	BUFPUTSL(ob, "\\includegraphics{");
	bufput(ob, link->data, link->size);
	BUFPUTSL(ob, "}");
	return 1; }	

static void
latex_blockcode(struct buf *ob, struct buf *text, void *opaque) {
	if (ob->size) bufputc(ob, '\n');
	BUFPUTSL(ob, "\\begin{verbatim}\n");
	if (text) bufput(ob, text->data, text->size);
	BUFPUTSL(ob, "\\end{verbatim}\n"); }

static void
latex_blockquote(struct buf *ob, struct buf *text, void *opaque) {
	if (ob->size) bufputc(ob, '\n');
	BUFPUTSL(ob, "\\begin{quote}\n");
	if (text) bufput(ob, text->data, text->size);
	BUFPUTSL(ob, "\\end{quote}\n"); }

static int
latex_codespan(struct buf *ob, struct buf *text, void *opaque) {
	BUFPUTSL(ob, "\\texttt{");
	if (text) latex_text_escape(ob, text->data, text->size);
	BUFPUTSL(ob, "}"); 
	return 1; }

static void
latex_header(struct buf *ob, struct buf *text, int level, void *opaque) {
	if (ob->size) bufputc(ob, '\n');
	switch(level) {
		case 1:
			BUFPUTSL(ob,"\\section{"); 
			break;
		case 2:
			BUFPUTSL(ob, "\\subsection{");
			break;
		case 3:
			BUFPUTSL(ob, "\\subsubsection{");
			break;
		default:
			fprintf(stderr, "Warning: ignoring header level %d\n",
                                        level);
	}
	if (text) bufput(ob, text->data, text->size);
	if (level >= 1 && level <= 3) BUFPUTSL(ob, "}\n");
}

static int
latex_double_emphasis(struct buf *ob, struct buf *text, char c, void *opaque) {
	if (!text || !text->size) return 0;
	BUFPUTSL(ob, "\\textbf{");
	bufput(ob, text->data, text->size);
	BUFPUTSL(ob, "}");
	return 1; }

static int
latex_emphasis(struct buf *ob, struct buf *text, char c, void *opaque) {
	if (!text || !text->size) return 0;
	BUFPUTSL(ob, "\\emph{");
	if (text) bufput(ob, text->data, text->size);
	BUFPUTSL(ob, "}");
	return 1; }

static int
latex_linebreak(struct buf *ob, void *opaque) {
	BUFPUTSL(ob, "\\\\");
	return 1; }

static void
latex_paragraph(struct buf *ob, struct buf *text, void *opaque) {
	if (ob->size) bufputc(ob, '\n');
	if (text) bufput(ob, text->data, text->size);
	BUFPUTSL(ob, "\n"); }

static void
latex_list(struct buf *ob, struct buf *text, int flags, void *opaque) {
	if (ob->size) bufputc(ob, '\n');
	if (flags & MKD_LIST_ORDERED)
		BUFPUTSL(ob, "\\begin{enumerate}\n");
	else
		BUFPUTSL(ob, "\\begin{itemize}\n");
	if (text) bufput(ob, text->data, text->size);
	if (flags & MKD_LIST_ORDERED)
		BUFPUTSL(ob, "\\end{enumerate}\n");
	else
		BUFPUTSL(ob, "\\end{itemize}\n"); }

static void
latex_listitem(struct buf *ob, struct buf *text, int flags, void *opaque) {
	BUFPUTSL(ob, "\\item ");
	if (text) {
		while (text->size && text->data[text->size - 1] == '\n')
			text->size -= 1;
		bufput(ob, text->data, text->size); }
	BUFPUTSL(ob, "\n"); }

static void
latex_hrule(struct buf *ob, void *opaque) {
	if (ob->size) bufputc(ob, '\n');
	BUFPUTSL(ob, "\\hrule"); }

static int
latex_triple_emphasis(struct buf *ob, struct buf *text, char c, void *opaque) {
	if (!text || !text->size) return 0;
	BUFPUTSL(ob, "\\textbf{\\emph{");
	bufput(ob, text->data, text->size);
	BUFPUTSL(ob, "}}");
	return 1; }

static void
latex_entity(struct buf *ob, struct buf *entity, void *opaque) {
	const char *rendered = entity2latex(entity);
	if (rendered)
		bufputs(ob, rendered);
	else {
		BUFPUTSL(ob, "\\texttt{");
		bufput(ob, entity->data, entity->size);
		BUFPUTSL(ob, "}"); } }

static void
latex_normal_text(struct buf *ob, struct buf *text, void *opaque) {
	if (text) latex_text_escape(ob, text->data, text->size); }


/* renderer structure */
struct mkd_renderer to_latex = {
	/* document-level callbacks */
	latex_prolog,
	latex_epilog,

	/* block-level callbacks */
	latex_blockcode,
	latex_blockquote,
	latex_blockcode,
	latex_header,
	latex_hrule,
	latex_list,
	latex_listitem,
	latex_paragraph,
	NULL,
	NULL,
	NULL,

	/* span-level callbacks */
	latex_autolink,
	latex_codespan,
	latex_double_emphasis,
	latex_emphasis,
	latex_image,
	latex_linebreak,
	latex_link,
	latex_codespan,
	latex_triple_emphasis,

	/* low-level callbacks */
	latex_entity,
	latex_normal_text,

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

	/* performing markdown to LaTeX */
	ob = bufnew(OUTPUT_UNIT);
	markdown(ob, ib, &to_latex);

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
