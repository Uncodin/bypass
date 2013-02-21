/* renderers.h - example markdown renderers */

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

#include "renderers.h"

#include <strings.h>


/*****************************
 * EXPORTED HELPER FUNCTIONS *
 *****************************/

/* lus_attr_escape • copy the buffer entity-escaping '<', '>', '&' and '"' */
void
lus_attr_escape(struct buf *ob, char *src, size_t size) {
	size_t  i = 0, org;
	while (i < size) {
		/* copying directly unescaped characters */
		org = i;
		while (i < size && src[i] != '<' && src[i] != '>'
		&& src[i] != '&' && src[i] != '"')
			i += 1;
		if (i > org) bufput(ob, src + org, i - org);

		/* escaping */
		if (i >= size) break;
		else if (src[i] == '<') BUFPUTSL(ob, "&lt;");
		else if (src[i] == '>') BUFPUTSL(ob, "&gt;");
		else if (src[i] == '&') BUFPUTSL(ob, "&amp;");
		else if (src[i] == '"') BUFPUTSL(ob, "&quot;");
		i += 1; } }


/* lus_body_escape • copy the buffer entity-escaping '<', '>' and '&' */
void
lus_body_escape(struct buf *ob, char *src, size_t size) {
	size_t  i = 0, org;
	while (i < size) {
		/* copying directly unescaped characters */
		org = i;
		while (i < size && src[i] != '<' && src[i] != '>'
		&& src[i] != '&')
			i += 1;
		if (i > org) bufput(ob, src + org, i - org);

		/* escaping */
		if (i >= size) break;
		else if (src[i] == '<') BUFPUTSL(ob, "&lt;");
		else if (src[i] == '>') BUFPUTSL(ob, "&gt;");
		else if (src[i] == '&') BUFPUTSL(ob, "&amp;");
		i += 1; } }



/********************
 * GENERIC RENDERER *
 ********************/

static int
rndr_autolink(struct buf *ob, struct buf *link, enum mkd_autolink type,
						void *opaque) {
	if (!link || !link->size) return 0;
	BUFPUTSL(ob, "<a href=\"");
	if (type == MKDA_IMPLICIT_EMAIL) BUFPUTSL(ob, "mailto:");
	lus_attr_escape(ob, link->data, link->size);
	BUFPUTSL(ob, "\">");
	if (type == MKDA_EXPLICIT_EMAIL && link->size > 7)
		lus_body_escape(ob, link->data + 7, link->size - 7);
	else	lus_body_escape(ob, link->data, link->size);
	BUFPUTSL(ob, "</a>");
	return 1; }

static void
rndr_blockcode(struct buf *ob, struct buf *text, void *opaque) {
	if (ob->size) bufputc(ob, '\n');
	BUFPUTSL(ob, "<pre><code>");
	if (text) lus_body_escape(ob, text->data, text->size);
	BUFPUTSL(ob, "</code></pre>\n"); }

static void
rndr_blockquote(struct buf *ob, struct buf *text, void *opaque) {
	if (ob->size) bufputc(ob, '\n');
	BUFPUTSL(ob, "<blockquote>\n");
	if (text) bufput(ob, text->data, text->size);
	BUFPUTSL(ob, "</blockquote>\n"); }

static int
rndr_codespan(struct buf *ob, struct buf *text, void *opaque) {
	BUFPUTSL(ob, "<code>");
	if (text) lus_body_escape(ob, text->data, text->size);
	BUFPUTSL(ob, "</code>");
	return 1; }

static int
rndr_double_emphasis(struct buf *ob, struct buf *text, char c, void *opaque) {
	if (!text || !text->size) return 0;
	BUFPUTSL(ob, "<strong>");
	bufput(ob, text->data, text->size);
	BUFPUTSL(ob, "</strong>");
	return 1; }

static int
rndr_emphasis(struct buf *ob, struct buf *text, char c, void *opaque) {
	if (!text || !text->size) return 0;
	BUFPUTSL(ob, "<em>");
	if (text) bufput(ob, text->data, text->size);
	BUFPUTSL(ob, "</em>");
	return 1; }

static void
rndr_header(struct buf *ob, struct buf *text, int level, void *opaque) {
	if (ob->size) bufputc(ob, '\n');
	bufprintf(ob, "<h%d>", level);
	if (text) bufput(ob, text->data, text->size);
	bufprintf(ob, "</h%d>\n", level); }

static int
rndr_link(struct buf *ob, struct buf *link, struct buf *title,
			struct buf *content, void *opaque) {
	BUFPUTSL(ob, "<a href=\"");
	if (link && link->size) lus_attr_escape(ob, link->data, link->size);
	if (title && title->size) {
		BUFPUTSL(ob, "\" title=\"");
		lus_attr_escape(ob, title->data, title->size); }
	BUFPUTSL(ob, "\">");
	if (content && content->size) bufput(ob, content->data, content->size);
	BUFPUTSL(ob, "</a>");
	return 1; }

static void
rndr_list(struct buf *ob, struct buf *text, int flags, void *opaque) {
	if (ob->size) bufputc(ob, '\n');
	bufput(ob, flags & MKD_LIST_ORDERED ? "<ol>\n" : "<ul>\n", 5);
	if (text) bufput(ob, text->data, text->size);
	bufput(ob, flags & MKD_LIST_ORDERED ? "</ol>\n" : "</ul>\n", 6); }

static void
rndr_listitem(struct buf *ob, struct buf *text, int flags, void *opaque) {
	BUFPUTSL(ob, "<li>");
	if (text) {
		while (text->size && text->data[text->size - 1] == '\n')
			text->size -= 1;
		bufput(ob, text->data, text->size); }
	BUFPUTSL(ob, "</li>\n"); }

static void
rndr_normal_text(struct buf *ob, struct buf *text, void *opaque) {
	if (text) lus_body_escape(ob, text->data, text->size); }

static void
rndr_paragraph(struct buf *ob, struct buf *text, void *opaque) {
	if (ob->size) bufputc(ob, '\n');
	BUFPUTSL(ob, "<p>");
	if (text) bufput(ob, text->data, text->size);
	BUFPUTSL(ob, "</p>\n"); }

static void
rndr_raw_block(struct buf *ob, struct buf *text, void *opaque) {
	size_t org, sz;
	if (!text) return;
	sz = text->size;
	while (sz > 0 && text->data[sz - 1] == '\n') sz -= 1;
	org = 0;
	while (org < sz && text->data[org] == '\n') org += 1;
	if (org >= sz) return;
	if (ob->size) bufputc(ob, '\n');
	bufput(ob, text->data + org, sz - org);
	bufputc(ob, '\n'); }

static int
rndr_raw_inline(struct buf *ob, struct buf *text, void *opaque) {
	bufput(ob, text->data, text->size);
	return 1; }

static int
rndr_triple_emphasis(struct buf *ob, struct buf *text, char c, void *opaque) {
	if (!text || !text->size) return 0;
	BUFPUTSL(ob, "<strong><em>");
	bufput(ob, text->data, text->size);
	BUFPUTSL(ob, "</em></strong>");
	return 1; }



/*******************
 * HTML 4 RENDERER *
 *******************/

static void
html_hrule(struct buf *ob, void *opaque) {
	if (ob->size) bufputc(ob, '\n');
	BUFPUTSL(ob, "<hr>\n"); }

static int
html_image(struct buf *ob, struct buf *link, struct buf *title,
			struct buf *alt, void *opaque) {
	if (!link || !link->size) return 0;
	BUFPUTSL(ob, "<img src=\"");
	lus_attr_escape(ob, link->data, link->size);
	BUFPUTSL(ob, "\" alt=\"");
	if (alt && alt->size)
		lus_attr_escape(ob, alt->data, alt->size);
	if (title && title->size) {
		BUFPUTSL(ob, "\" title=\"");
		lus_attr_escape(ob, title->data, title->size); }
	BUFPUTSL(ob, "\">");
	return 1; }

static int
html_linebreak(struct buf *ob, void *opaque) {
	BUFPUTSL(ob, "<br>\n");
	return 1; }


/* exported renderer structure */
const struct mkd_renderer mkd_html = {
	NULL,
	NULL,

	rndr_blockcode,
	rndr_blockquote,
	rndr_raw_block,
	rndr_header,
	html_hrule,
	rndr_list,
	rndr_listitem,
	rndr_paragraph,
	NULL,
	NULL,
	NULL,

	rndr_autolink,
	rndr_codespan,
	rndr_double_emphasis,
	rndr_emphasis,
	html_image,
	html_linebreak,
	rndr_link,
	rndr_raw_inline,
	rndr_triple_emphasis,

	NULL,
	rndr_normal_text,

	64,
	"*_",
	NULL };



/**********************
 * XHTML 1.0 RENDERER *
 **********************/

static void
xhtml_hrule(struct buf *ob, void *opaque) {
	if (ob->size) bufputc(ob, '\n');
	BUFPUTSL(ob, "<hr />\n"); }

static int
xhtml_image(struct buf *ob, struct buf *link, struct buf *title,
			struct buf *alt, void *opaque) {
	if (!link || !link->size) return 0;
	BUFPUTSL(ob, "<img src=\"");
	lus_attr_escape(ob, link->data, link->size);
	BUFPUTSL(ob, "\" alt=\"");
	if (alt && alt->size)
		lus_attr_escape(ob, alt->data, alt->size);
	if (title && title->size) {
		BUFPUTSL(ob, "\" title=\"");
		lus_attr_escape(ob, title->data, title->size); }
	BUFPUTSL(ob, "\" />");
	return 1; }

static int
xhtml_linebreak(struct buf *ob, void *opaque) {
	BUFPUTSL(ob, "<br />\n");
	return 1; }


/* exported renderer structure */
const struct mkd_renderer mkd_xhtml = {
	NULL,
	NULL,

	rndr_blockcode,
	rndr_blockquote,
	rndr_raw_block,
	rndr_header,
	xhtml_hrule,
	rndr_list,
	rndr_listitem,
	rndr_paragraph,
	NULL,
	NULL,
	NULL,

	rndr_autolink,
	rndr_codespan,
	rndr_double_emphasis,
	rndr_emphasis,
	xhtml_image,
	xhtml_linebreak,
	rndr_link,
	rndr_raw_inline,
	rndr_triple_emphasis,

	NULL,
	rndr_normal_text,

	64,
	"*_",
	NULL };



/**********************
 * DISCOUNT RENDERERS *
 **********************/

static int
print_link_wxh(struct buf *ob, struct buf *link) {
	size_t eq, ex, end;
	eq = link->size - 1;
	while (eq > 0 && (link->data[eq - 1] != ' ' || link->data[eq] != '='))
		eq -= 1;
	if (eq <= 0) return 0;
	ex = eq + 1;
	while (ex < link->size
	&& link->data[ex] >= '0' && link->data[ex] <= '9')
		ex += 1;
	if (ex >= link->size || ex == eq + 1 || link->data[ex] != 'x') return 0;
	end = ex + 1;
	while (end < link->size
	&& link->data[end] >= '0' && link->data[end] <= '9')
		end += 1;
	if (end == ex + 1) return 0;
	/* everything is fine, proceeding to actual printing */
	lus_attr_escape(ob, link->data, eq - 1);
	BUFPUTSL(ob, "\" width=");
	bufput(ob, link->data + eq + 1, ex - eq - 1);
	BUFPUTSL(ob, " height=");
	bufput(ob, link->data + ex + 1, end - ex - 1);
	return 1; }

static int
discount_image(struct buf *ob, struct buf *link, struct buf *title,
			struct buf *alt, int xhtml) {
	if (!link || !link->size) return 0;
	BUFPUTSL(ob, "<img src=\"");
	if (!print_link_wxh(ob, link)) {
		lus_attr_escape(ob, link->data, link->size);
		bufputc(ob, '"'); }
	BUFPUTSL(ob, " alt=\"");
	if (alt && alt->size)
		lus_attr_escape(ob, alt->data, alt->size);
	if (title && title->size) {
		BUFPUTSL(ob, "\" title=\"");
		lus_attr_escape(ob, title->data, title->size); }
	bufputs(ob, xhtml ? "\" />" : "\">");
	return 1; }

static int
html_discount_image(struct buf *ob, struct buf *link, struct buf *title,
			struct buf *alt, void *opaque) {
	return discount_image(ob, link, title, alt, 0); }

static int
xhtml_discount_image(struct buf *ob, struct buf *link, struct buf *title,
			struct buf *alt, void *opaque) {
	return discount_image(ob, link, title, alt, 1); }

static int
discount_link(struct buf *ob, struct buf *link, struct buf *title,
			struct buf *content, void *opaque) {
	if (!link) return rndr_link(ob, link, title, content, opaque);
	else if (link->size > 5 && !strncasecmp(link->data, "abbr:", 5)) {
		BUFPUTSL(ob, "<abbr title=\"");
		lus_attr_escape(ob, link->data + 5, link->size - 5);
		BUFPUTSL(ob, "\">");
		bufput(ob, content->data, content->size);
		BUFPUTSL(ob, "</abbr>");
		return 1; }
	else if (link->size > 6 && !strncasecmp(link->data, "class:", 6)) {
		BUFPUTSL(ob, "<span class=\"");
		lus_attr_escape(ob, link->data + 6, link->size - 6);
		BUFPUTSL(ob, "\">");
		bufput(ob, content->data, content->size);
		BUFPUTSL(ob, "</span>");
		return 1; }
	else if (link->size > 3 && !strncasecmp(link->data, "id:", 3)) {
		BUFPUTSL(ob, "<a id=\"");
		lus_attr_escape(ob, link->data + 3, link->size - 3);
		BUFPUTSL(ob, "\">");
		bufput(ob, content->data, content->size);
		BUFPUTSL(ob, "</span>");
		return 1; }
	else if (link->size > 4 && !strncasecmp(link->data, "raw:", 4)) {
		lus_attr_escape(ob, link->data + 4, link->size - 4);
		return 1; }
	return rndr_link(ob, link, title, content, opaque); }

static void
discount_blockquote(struct buf *ob, struct buf *text, void *opaque) {
	size_t i = 5, size = text->size;
	char *data = text->data;
	if (text->size < 5 || strncasecmp(text->data, "<p>%", 4)) {
		rndr_blockquote(ob, text, opaque);
		return; }
	while (i < size && data[i] != '\n' && data[i] != '%')
		i += 1;
	if (i >= size || data[i] != '%') {
		rndr_blockquote(ob, text, opaque);
		return; }
	BUFPUTSL(ob, "<div class=\"");
	bufput(ob, text->data + 4, i - 4);
	BUFPUTSL(ob, "\"><p>");
	i += 1;
	if (i + 4 >= text->size && !strncasecmp(text->data + i, "</p>", 4)) {
		size_t old_i = i;
		i += 4;
		while (i + 3 < text->size
		&& (data[i] != '<' || data[i + 1] != 'p' || data[i + 2] != '>'))
			i += 1;
		if (i + 3 >= text->size) i = old_i; }
	bufput(ob, text->data + i, text->size - i);
	BUFPUTSL(ob, "</div>\n"); }

static void
discount_table(struct buf *ob, struct buf *head_row, struct buf *rows,
					void *opaque) {
	if (ob->size) bufputc(ob, '\n');
	BUFPUTSL(ob, "<table>\n");
	if (head_row) {
		BUFPUTSL(ob, "<thead>\n");
		bufput(ob, head_row->data, head_row->size);
		BUFPUTSL(ob, "</thead>\n<tbody>\n"); }
	if (rows)
		bufput(ob, rows->data, rows->size);
	if (head_row)
		BUFPUTSL(ob, "</tbody>\n");
	BUFPUTSL(ob, "</table>\n"); }

static void
discount_table_row(struct buf *ob, struct buf *cells, int flags, void *opaque){
	(void)flags;
	BUFPUTSL(ob, "  <tr>\n");
	if (cells) bufput(ob, cells->data, cells->size);
	BUFPUTSL(ob, "  </tr>\n"); }

static void
discount_table_cell(struct buf *ob, struct buf *text, int flags, void *opaque){
	if (flags & MKD_CELL_HEAD)
		BUFPUTSL(ob, "    <th");
	else
		BUFPUTSL(ob, "    <td");
	switch (flags & MKD_CELL_ALIGN_MASK) {
		case MKD_CELL_ALIGN_LEFT:
			BUFPUTSL(ob, " align=\"left\"");
			break;
		case MKD_CELL_ALIGN_RIGHT:
			BUFPUTSL(ob, " align=\"right\"");
			break;
		case MKD_CELL_ALIGN_CENTER:
			BUFPUTSL(ob, " align=\"center\"");
			break; }
	bufputc(ob, '>');
	if (text) bufput(ob, text->data, text->size);
	if (flags & MKD_CELL_HEAD)
		BUFPUTSL(ob, "</th>\n");
	else
		BUFPUTSL(ob, "</td>\n"); }

/* exported renderer structures */
const struct mkd_renderer discount_html = {
	NULL,
	NULL,

	rndr_blockcode,
	discount_blockquote,
	rndr_raw_block,
	rndr_header,
	html_hrule,
	rndr_list,
	rndr_listitem,
	rndr_paragraph,
	discount_table,
	discount_table_cell,
	discount_table_row,

	rndr_autolink,
	rndr_codespan,
	rndr_double_emphasis,
	rndr_emphasis,
	html_discount_image,
	html_linebreak,
	discount_link,
	rndr_raw_inline,
	rndr_triple_emphasis,

	NULL,
	rndr_normal_text,

	64,
	"*_",
	NULL };
const struct mkd_renderer discount_xhtml = {
	NULL,
	NULL,

	rndr_blockcode,
	discount_blockquote,
	rndr_raw_block,
	rndr_header,
	xhtml_hrule,
	rndr_list,
	rndr_listitem,
	rndr_paragraph,
	discount_table,
	discount_table_cell,
	discount_table_row,

	rndr_autolink,
	rndr_codespan,
	rndr_double_emphasis,
	rndr_emphasis,
	xhtml_discount_image,
	xhtml_linebreak,
	discount_link,
	rndr_raw_inline,
	rndr_triple_emphasis,

	NULL,
	rndr_normal_text,

	64,
	"*_",
	NULL };


/****************************
 * NATACHA'S OWN EXTENSIONS *
 ****************************/

static void
nat_span(struct buf *ob, struct buf *text, char *tag) {
	bufprintf(ob, "<%s>", tag);
	bufput(ob, text->data, text->size);
	bufprintf(ob, "</%s>", tag); }

static int
nat_emphasis(struct buf *ob, struct buf *text, char c, void *opaque) {
	if (!text || !text->size || c == '+' || c == '-') return 0;
	if (c == '|') nat_span(ob, text, "span");
	else nat_span(ob, text, "em");
	return 1; }

static int
nat_double_emphasis(struct buf *ob, struct buf *text, char c, void *opaque) {
	if (!text || !text->size || c == '|') return 0;
	if (c == '+') nat_span(ob, text, "ins");
	else if (c == '-') nat_span(ob, text, "del");
	else nat_span(ob, text, "strong");
	return 1; }

static int
nat_triple_emphasis(struct buf *ob, struct buf *text, char c, void *opaque) {
	if (!text || !text->size || c == '+' || c == '-' || c == '|') return 0;
	BUFPUTSL(ob, "<strong><em>");
	bufput(ob, text->data, text->size);
	BUFPUTSL(ob, "</em></strong>");
	return 1; }

static void
nat_header(struct buf *ob, struct buf *text, int level, void *opaque) {
	size_t i = 0;
	if (ob->size) bufputc(ob, '\n');
	while (i < text->size && (text->data[i] == '-' || text->data[i] == '_'
			 ||  text->data[i] == '.' || text->data[i] == ':'
			 || (text->data[i] >= 'a' && text->data[i] <= 'z')
			 || (text->data[i] >= 'A' && text->data[i] <= 'Z')
			 || (text->data[i] >= '0' && text->data[i] <= '0')))
		i += 1;
	bufprintf(ob, "<h%d", level);
	if (i < text->size && text->data[i] == '#') {
		bufprintf(ob, " id=\"%.*s\">", (int)i, text->data);
		i += 1; }
	else {
		bufputc(ob, '>');
		i = 0; }
	bufput(ob, text->data + i, text->size - i);
	bufprintf(ob, "</h%d>\n", level); }

static void
nat_paragraph(struct buf *ob, struct buf *text, void *opaque) {
	size_t i = 0;
	if (ob->size) bufputc(ob, '\n');
	BUFPUTSL(ob, "<p");
	if (text && text->size && text->data[0] == '(') {
		i = 1;
		while (i < text->size && (text->data[i] == ' '
			/* this seems to be a bit more restrictive than */
			/* what is allowed for class names */
			 || (text->data[i] >= 'a' && text->data[i] <= 'z')
			 || (text->data[i] >= 'A' && text->data[i] <= 'Z')
			 || (text->data[i] >= '0' && text->data[i] <= '0')))
			i += 1;
		if (i < text->size && text->data[i] == ')') {
			bufprintf(ob, " class=\"%.*s\"",
						(int)(i - 1), text->data + 1);
			i += 1; }
		else i = 0; }
	bufputc(ob, '>');
	if (text) bufput(ob, text->data + i, text->size - i);
	BUFPUTSL(ob, "</p>\n"); }


/* exported renderer structures */
const struct mkd_renderer nat_html = {
	NULL,
	NULL,

	rndr_blockcode,
	discount_blockquote,
	rndr_raw_block,
	nat_header,
	html_hrule,
	rndr_list,
	rndr_listitem,
	nat_paragraph,
	NULL,
	NULL,
	NULL,

	rndr_autolink,
	rndr_codespan,
	nat_double_emphasis,
	nat_emphasis,
	html_discount_image,
	html_linebreak,
	discount_link,
	rndr_raw_inline,
	nat_triple_emphasis,

	NULL,
	rndr_normal_text,

	64,
	"*_-+|",
	NULL };
const struct mkd_renderer nat_xhtml = {
	NULL,
	NULL,

	rndr_blockcode,
	discount_blockquote,
	rndr_raw_block,
	nat_header,
	xhtml_hrule,
	rndr_list,
	rndr_listitem,
	nat_paragraph,
	NULL,
	NULL,
	NULL,

	rndr_autolink,
	rndr_codespan,
	nat_double_emphasis,
	nat_emphasis,
	xhtml_discount_image,
	xhtml_linebreak,
	discount_link,
	rndr_raw_inline,
	nat_triple_emphasis,

	NULL,
	rndr_normal_text,

	64,
	"*_-+|",
	NULL };
