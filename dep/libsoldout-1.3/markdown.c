/* markdown.c - generic markdown parser */

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

#include "array.h"

#include <assert.h>
#include <string.h>
#include <strings.h> /* for strncasecmp */

#define TEXT_UNIT 64	/* unit for the copy of the input buffer */
#define WORK_UNIT 64	/* block-level working buffer */

#define MKD_LI_END 8	/* internal list flag */


/***************
 * LOCAL TYPES *
 ***************/

/* link_ref • reference to a link */
struct link_ref {
	struct buf *	id;
	struct buf *	link;
	struct buf *	title; };


/* char_trigger • function pointer to render active chars */
/*   returns the number of chars taken care of */
/*   data is the pointer of the beginning of the span */
/*   offset is the number of valid chars before data */
struct render;
typedef size_t
(*char_trigger)(struct buf *ob, struct render *rndr,
		char *data, size_t offset, size_t size);


/* render • structure containing one particular render */
struct render {
	struct mkd_renderer	make;
	struct array		refs;
	char_trigger		active_char[256];
	struct parray		work; };


/* html_tag • structure for quick HTML tag search (inspired from discount) */
struct html_tag {
	char *	text;
	int	size; };



/********************
 * GLOBAL VARIABLES *
 ********************/

/* block_tags • recognised block tags, sorted by cmp_html_tag */
static struct html_tag block_tags[] = {
/*0*/	{ "p",		1 },
	{ "dl",		2 },
	{ "h1",		2 },
	{ "h2",		2 },
	{ "h3",		2 },
	{ "h4",		2 },
	{ "h5",		2 },
	{ "h6",		2 },
	{ "ol",		2 },
	{ "ul",		2 },
/*10*/	{ "del",	3 },
	{ "div",	3 },
/*12*/	{ "ins",	3 },
	{ "pre",	3 },
	{ "form",	4 },
	{ "math",	4 },
	{ "table",	5 },
	{ "iframe",	6 },
	{ "script",	6 },
	{ "fieldset",	8 },
	{ "noscript",	8 },
	{ "blockquote",	10 } };

#define INS_TAG (block_tags + 12)
#define DEL_TAG (block_tags + 10)



/***************************
 * STATIC HELPER FUNCTIONS *
 ***************************/

/* build_ref_id • collapse whitespace from input text to make it a ref id */
static int
build_ref_id(struct buf *id, const char *data, size_t size) {
	size_t beg, i;

	/* skip leading whitespace */
	while (size > 0
	&& (data[0] == ' ' || data[0] == '\t' || data[0] == '\n')) {
		data += 1;
		size -= 1; }

	/* skip trailing whitespace */
	while (size > 0
	&& (data[size - 1] == ' ' || data[size - 1] == '\t'
	 || data[size - 1] == '\n'))
		size -= 1;
	if (size == 0) return -1;

	/* making the ref id */
	i = 0;
	id->size = 0;
	while (i < size) {
		/* copy non-whitespace into the output buffer */
		beg = i;
		while (i < size
		&& !(data[i] == ' ' || data[i] == '\t' || data[i] == '\n'))
			i += 1;
		bufput(id, data + beg, i - beg);

		/* add a single space and skip all consecutive whitespace */
		if (i < size) bufputc(id, ' ');
		while (i < size
		&& (data[i] == ' ' || data[i] == '\t' || data[i] == '\n'))
			i += 1; }
	return 0; }


/* cmp_link_ref • comparison function for link_ref sorted arrays */
static int
cmp_link_ref(void *key, void *array_entry) {
	struct link_ref *lr = array_entry;
	return bufcasecmp(key, lr->id); }


/* cmp_link_ref_sort • comparison function for link_ref qsort */
static int
cmp_link_ref_sort(const void *a, const void *b) {
	const struct link_ref *lra = a;
	const struct link_ref *lrb = b;
	return bufcasecmp(lra->id, lrb->id); }


/* cmp_html_tag • comparison function for bsearch() (stolen from discount) */
static int
cmp_html_tag(const void *a, const void *b) {
	const struct html_tag *hta = a;
	const struct html_tag *htb = b;
	if (hta->size != htb->size) return hta->size - htb->size;
	return strncasecmp(hta->text, htb->text, hta->size); }


/* find_block_tag • returns the current block tag */
static struct html_tag *
find_block_tag(char *data, size_t size) {
	size_t i = 0;
	struct html_tag key;

	/* looking for the word end */
	while (i < size && ((data[i] >= '0' && data[i] <= '9')
				|| (data[i] >= 'A' && data[i] <= 'Z')
				|| (data[i] >= 'a' && data[i] <= 'z')))
		i += 1;
	if (i >= size) return 0;

	/* binary search of the tag */
	key.text = data;
	key.size = i;
	return bsearch(&key, block_tags,
				sizeof block_tags / sizeof block_tags[0],
				sizeof block_tags[0], cmp_html_tag); }


/* new_work_buffer • get a new working buffer from the stack or create one */
static struct buf *
new_work_buffer(struct render *rndr) {
	struct buf *ret = 0;

	if (rndr->work.size < rndr->work.asize) {
		ret = rndr->work.item[rndr->work.size ++];
		ret->size = 0; }
	else {
		ret = bufnew(WORK_UNIT);
		parr_push(&rndr->work, ret); }
	return ret; }


/* release_work_buffer • release the given working buffer */
static void
release_work_buffer(struct render *rndr, struct buf *buf) {
	assert(rndr->work.size > 0
	&& rndr->work.item[rndr->work.size - 1] == buf);
	rndr->work.size -= 1; }



/****************************
 * INLINE PARSING FUNCTIONS *
 ****************************/

/* is_mail_autolink • looks for the address part of a mail autolink and '>' */
/* this is less strict than the original markdown e-mail address matching */
static size_t
is_mail_autolink(char *data, size_t size) {
	size_t i = 0, nb = 0;
	/* address is assumed to be: [-@._a-zA-Z0-9]+ with exactly one '@' */
	while (i < size && (data[i] == '-' || data[i] == '.'
	|| data[i] == '_' || data[i] == '@'
	|| (data[i] >= 'a' && data[i] <= 'z')
	|| (data[i] >= 'A' && data[i] <= 'Z')
	|| (data[i] >= '0' && data[i] <= '9'))) {
		if (data[i] == '@') nb += 1;
		i += 1; }
	if (i >= size || data[i] != '>' || nb != 1) return 0;
	return i + 1; }


/* tag_length • returns the length of the given tag, or 0 is it's not valid */
static size_t
tag_length(char *data, size_t size, enum mkd_autolink *autolink) {
	size_t i, j;

	/* a valid tag can't be shorter than 3 chars */
	if (size < 3) return 0;

	/* begins with a '<' optionally followed by '/', followed by letter */
	if (data[0] != '<') return 0;
	i = (data[1] == '/') ? 2 : 1;
	if ((data[i] < 'a' || data[i] > 'z')
	&&  (data[i] < 'A' || data[i] > 'Z')) return 0;

	/* scheme test */
	*autolink = MKDA_NOT_AUTOLINK;
	if (size > 6 && strncasecmp(data + 1, "http", 4) == 0 && (data[5] == ':'
	|| ((data[5] == 's' || data[5] == 'S') && data[6] == ':'))) {
		i = data[5] == ':' ? 6 : 7;
		*autolink = MKDA_NORMAL; }
	else if (size > 5 && strncasecmp(data + 1, "ftp:", 4) == 0) {
		i = 5;
		*autolink = MKDA_NORMAL; }
	else if (size > 7 && strncasecmp(data + 1, "mailto:", 7) == 0) {
		i = 8;
		/* not changing *autolink to go to the address test */ }

	/* completing autolink test: no whitespace or ' or " */
	if (i >= size || i == '>')
		*autolink = MKDA_NOT_AUTOLINK;
	else if (*autolink) {
		j = i;
		while (i < size && data[i] != '>' && data[i] != '\''
		&& data[i] != '"' && data[i] != ' ' && data[i] != '\t'
		&& data[i] != '\t')
			i += 1;
		if (i >= size) return 0;
		if (i > j && data[i] == '>') return i + 1;
		/* one of the forbidden chars has been found */
		*autolink = MKDA_NOT_AUTOLINK; }
	else if ((j = is_mail_autolink(data + i, size - i)) != 0) {
		*autolink = (i == 8)
				? MKDA_EXPLICIT_EMAIL : MKDA_IMPLICIT_EMAIL;
		return i + j; }

	/* looking for sometinhg looking like a tag end */
	while (i < size && data[i] != '>') i += 1;
	if (i >= size) return 0;
	return i + 1; }


/* parse_inline • parses inline markdown elements */
static void
parse_inline(struct buf *ob, struct render *rndr, char *data, size_t size) {
	size_t i = 0, end = 0;
	char_trigger action = 0;
	struct buf work = { 0, 0, 0, 0, 0 };

	if (rndr->work.size > rndr->make.max_work_stack) {
		if (size) bufput(ob, data, size);
		return; }

	while (i < size) {
		/* copying inactive chars into the output */
		while (end < size
		&& (action = rndr->active_char[(unsigned char)data[end]]) == 0)
			end += 1;
		if (rndr->make.normal_text) {
			work.data = data + i;
			work.size = end - i;
			rndr->make.normal_text(ob, &work, rndr->make.opaque); }
		else
			bufput(ob, data + i, end - i);
		if (end >= size) break;
		i = end;

		/* calling the trigger */
		end = action(ob, rndr, data + i, i, size - i);
		if (!end) /* no action from the callback */
			end = i + 1;
		else { 
			i += end;
			end = i; } } }


/* find_emph_char • looks for the next emph char, skipping other constructs */
static size_t
find_emph_char(char *data, size_t size, char c) {
	size_t i = 1;

	while (i < size) {
		while (i < size && data[i] != c
		&& data[i] != '`' && data[i] != '[')
			i += 1;
		if (i >= size) return 0;
		if (data[i] == c) return i;

		/* not counting escaped chars */
		if (i && data[i - 1] == '\\') { i += 1; continue; }

		/* skipping a code span */
		if (data[i] == '`') {
			size_t span_nb = 0, bt;
			size_t tmp_i = 0;

			/* counting the number of opening backticks */
			while (i < size && data[i] == '`') {
				i += 1;
				span_nb += 1; }
			if (i >= size) return 0;

			/* finding the matching closing sequence */
			bt = 0;
			while (i < size && bt < span_nb) {
				if (!tmp_i && data[i] == c) tmp_i = i;
				if (data[i] == '`') bt += 1;
				else bt = 0;
				i += 1; }
			if (i >= size) return tmp_i;
			i += 1; }

		/* skipping a link */
		else if (data[i] == '[') {
			size_t tmp_i = 0;
			char cc;
			i += 1;
			while (i < size && data[i] != ']') {
				if (!tmp_i && data[i] == c) tmp_i = i;
				i += 1; }
			i += 1;
			while (i < size && (data[i] == ' '
			|| data[i] == '\t' || data[i] == '\n'))
				i += 1;
			if (i >= size) return tmp_i;
			if (data[i] != '[' && data[i] != '(') { /* not a link*/
				if (tmp_i) return tmp_i;
				else continue; }
			cc = data[i];
			i += 1;
			while (i < size && data[i] != cc) {
				if (!tmp_i && data[i] == c) tmp_i = i;
				i += 1; }
			if (i >= size) return tmp_i;
			i += 1; } }
	return 0; }


/* parse_emph1 • parsing single emphase */
/* closed by a symbol not preceded by whitespace and not followed by symbol */
static size_t
parse_emph1(struct buf *ob, struct render *rndr,
			char *data, size_t size, char c) {
	size_t i = 0, len;
	struct buf *work = 0;
	int r;

	if (!rndr->make.emphasis) return 0;

	/* skipping one symbol if coming from emph3 */
	if (size > 1 && data[0] == c && data[1] == c) i = 1;

	while (i < size) {
		len = find_emph_char(data + i, size - i, c);
		if (!len) return 0;
		i += len;
		if (i >= size) return 0;

		if (i + 1 < size && data[i + 1] == c) {
			i += 1;
			continue; }
		if (data[i] == c && data[i - 1] != ' '
		&& data[i - 1] != '\t' && data[i - 1] != '\n') {
			work = new_work_buffer(rndr);
			parse_inline(work, rndr, data, i);
			r = rndr->make.emphasis(ob, work, c, rndr->make.opaque);
			release_work_buffer(rndr, work);
			return r ? i + 1 : 0; } }
	return 0; }


/* parse_emph2 • parsing single emphase */
static size_t
parse_emph2(struct buf *ob, struct render *rndr,
			char *data, size_t size, char c) {
	size_t i = 0, len;
	struct buf *work = 0;
	int r;

	if (!rndr->make.double_emphasis) return 0;
	
	while (i < size) {
		len = find_emph_char(data + i, size - i, c);
		if (!len) return 0;
		i += len;
		if (i + 1 < size && data[i] == c && data[i + 1] == c
		&& i && data[i - 1] != ' '
		&& data[i - 1] != '\t' && data[i - 1] != '\n') {
			work = new_work_buffer(rndr);
			parse_inline(work, rndr, data, i);
			r = rndr->make.double_emphasis(ob, work, c,
				rndr->make.opaque);
			release_work_buffer(rndr, work);
			return r ? i + 2 : 0; }
		i += 1; }
	return 0; }


/* parse_emph3 • parsing single emphase */
/* finds the first closing tag, and delegates to the other emph */
static size_t
parse_emph3(struct buf *ob, struct render *rndr,
			char *data, size_t size, char c) {
	size_t i = 0, len;
	int r;

	while (i < size) {
		len = find_emph_char(data + i, size - i, c);
		if (!len) return 0;
		i += len;

		/* skip whitespace preceded symbols */
		if (data[i] != c || data[i - 1] == ' '
		|| data[i - 1] == '\t' || data[i - 1] == '\n')
			continue;

		if (i + 2 < size && data[i + 1] == c && data[i + 2] == c
		&& rndr->make.triple_emphasis) {
			/* triple symbol found */
			struct buf *work = new_work_buffer(rndr);
			parse_inline(work, rndr, data, i);
			r = rndr->make.triple_emphasis(ob, work, c,
							rndr->make.opaque);
			release_work_buffer(rndr, work);
			return r ? i + 3 : 0; }
		else if (i + 1 < size && data[i + 1] == c) {
			/* double symbol found, handing over to emph1 */
			len = parse_emph1(ob, rndr, data - 2, size + 2, c);
			if (!len) return 0;
			else return len - 2; }
		else {
			/* single symbol found, handing over to emph2 */
			len = parse_emph2(ob, rndr, data - 1, size + 1, c);
			if (!len) return 0;
			else return len - 1; } }
	return 0; }


/* char_emphasis • single and double emphasis parsing */
static size_t
char_emphasis(struct buf *ob, struct render *rndr,
				char *data, size_t offset, size_t size) {
	char c = data[0];
	size_t ret;
	if (size > 2 && data[1] != c) {
		/* whitespace cannot follow an opening emphasis */
		if (data[1] == ' ' || data[1] == '\t' || data[1] == '\n'
		|| (ret = parse_emph1(ob, rndr, data + 1, size - 1, c)) == 0)
			return 0;
		return ret + 1; }
	if (size > 3 && data[1] == c && data[2] != c) {
		if (data[2] == ' ' || data[2] == '\t' || data[2] == '\n'
		|| (ret = parse_emph2(ob, rndr, data + 2, size - 2, c)) == 0)
			return 0;
		return ret + 2; }
	if (size > 4 && data[1] == c && data[2] == c && data[3] != c) {
		if (data[3] == ' ' || data[3] == '\t' || data[3] == '\n'
		|| (ret = parse_emph3(ob, rndr, data + 3, size - 3, c)) == 0)
			return 0;
		return ret + 3; }
	return 0; }


/* char_linebreak • '\n' preceded by two spaces (assuming linebreak != 0) */
static size_t
char_linebreak(struct buf *ob, struct render *rndr,
				char *data, size_t offset, size_t size) {
	if (offset < 2 || data[-1] != ' ' || data[-2] != ' ') return 0;
	/* removing the last space from ob and rendering */
	if (ob->size && ob->data[ob->size - 1] == ' ') ob->size -= 1;
	return rndr->make.linebreak(ob, rndr->make.opaque) ? 1 : 0; }


/* char_codespan • '`' parsing a code span (assuming codespan != 0) */
static size_t
char_codespan(struct buf *ob, struct render *rndr,
				char *data, size_t offset, size_t size) {
	size_t end, nb = 0, i, f_begin, f_end;

	/* counting the number of backticks in the delimiter */
	while (nb < size && data[nb] == '`') nb += 1;

	/* finding the next delimiter */
	i = 0;
	for (end = nb; end < size && i < nb; end += 1)
		if (data[end] == '`') i += 1;
		else i = 0;
	if (i < nb && end >= size) return 0; /* no matching delimiter */

	/* trimming outside whitespaces */
	f_begin = nb;
	while (f_begin < end && (data[f_begin] == ' ' || data[f_begin] == '\t'))
		f_begin += 1;
	f_end = end - nb;
	while (f_end > nb && (data[f_end-1] == ' ' || data[f_end-1] == '\t'))
		f_end -= 1;

	/* real code span */
	if (f_begin < f_end) {
		struct buf work = { data + f_begin, f_end - f_begin, 0, 0, 0 };
		if (!rndr->make.codespan(ob, &work, rndr->make.opaque))
			end = 0; }
	else {
		if (!rndr->make.codespan(ob, 0, rndr->make.opaque))
			end = 0; }
	return end; }


/* char_escape • '\\' backslash escape */
static size_t
char_escape(struct buf *ob, struct render *rndr,
				char *data, size_t offset, size_t size) {
	struct buf work = { 0, 0, 0, 0, 0 };
	if (size > 1) {
		if (rndr->make.normal_text) {
			work.data = data + 1;
			work.size = 1;
			rndr->make.normal_text(ob, &work, rndr->make.opaque); }
		else bufputc(ob, data[1]); }
	return 2; }


/* char_entity • '&' escaped when it doesn't belong to an entity */
/* valid entities are assumed to be anything mathing &#?[A-Za-z0-9]+; */
static size_t
char_entity(struct buf *ob, struct render *rndr,
				char *data, size_t offset, size_t size) {
	size_t end = 1;
	struct buf work;
	if (end < size && data[end] == '#') end += 1;
	while (end < size
	&& ((data[end] >= '0' && data[end] <= '9')
	||  (data[end] >= 'a' && data[end] <= 'z')
	||  (data[end] >= 'A' && data[end] <= 'Z')))
		end += 1;
	if (end < size && data[end] == ';') {
		/* real entity */
		end += 1; }
	else {
		/* lone '&' */
		return 0; }
	if (rndr->make.entity) {
		work.data = data;
		work.size = end;
		rndr->make.entity(ob, &work, rndr->make.opaque); }
	else bufput(ob, data, end);
	return end; }


/* char_langle_tag • '<' when tags or autolinks are allowed */
static size_t
char_langle_tag(struct buf *ob, struct render *rndr,
				char *data, size_t offset, size_t size) {
	enum mkd_autolink altype = MKDA_NOT_AUTOLINK;
	size_t end = tag_length(data, size, &altype);
	struct buf work = { data, end, 0, 0, 0 };
	int ret = 0;
	if (end) {
		if (rndr->make.autolink && altype != MKDA_NOT_AUTOLINK) {
			work.data = data + 1;
			work.size = end - 2;
			ret = rndr->make.autolink(ob, &work, altype,
							rndr->make.opaque); }
		else if (rndr->make.raw_html_tag)
			ret = rndr->make.raw_html_tag(ob, &work,
							rndr->make.opaque); }
	if (!ret) return 0;
	else return end; }


/* get_link_inline • extract inline-style link and title from parenthesed data*/
static int
get_link_inline(struct buf *link, struct buf *title, char *data, size_t size) {
	size_t i = 0, mark;
	size_t link_b, link_e;
	size_t title_b = 0, title_e = 0;

	link->size = title->size = 0;

	/* skipping initial whitespace */
	while (i < size
	&& (data[i] == ' ' || data[i] == '\t' || data[i] == '\n'))
		i += 1;
	link_b = i;

	/* looking for link end: ' " */
	while (i < size && data[i] != '\'' && data[i] != '"')
		i += 1;
	link_e = i;

	/* looking for title end if present */
	if (data[i] == '\'' || data[i] == '"') {
		i += 1;
		title_b = i;

		/* skipping whitespaces after title */
		title_e = size - 1;
		while (title_e > title_b && (data[title_e] == ' ' 
		|| data[title_e] == '\t' || data[title_e] == '\n'))
			title_e -= 1;

		/* checking for closing quote presence */
		if (data[title_e] != '\'' &&  data[title_e] != '"') {
			title_b = title_e = 0;
			link_e = i; } }

	/* remove whitespace at the end of the link */
	while (link_e > link_b
	&& (data[link_e - 1] == ' ' || data[link_e - 1] == '\t'
	 || data[link_e - 1] == '\n'))
		link_e -= 1;

	/* remove optional angle brackets around the link */
	if (data[link_b] == '<') link_b += 1;
	if (data[link_e - 1] == '>') link_e -= 1;

	/* escape backslashed character from link */
	link->size = 0;
	i = link_b;
	while (i < link_e) {
		mark = i;
		while (i < link_e && data[i] != '\\') i += 1;
		bufput(link, data + mark, i - mark);
		while (i < link_e && data[i] == '\\') i += 1; }

	/* handing back title */
	title->size = 0;
	if (title_e > title_b)
		bufput(title, data + title_b, title_e - title_b);

	/* this function always succeed */
	return 0; }


/* get_link_ref • extract referenced link and title from id */
static int
get_link_ref(struct render *rndr, struct buf *link, struct buf *title,
				char * data, size_t size) {
	struct link_ref *lr;

	/* find the link from its id (stored temporarily in link) */
	link->size = 0;
	if (build_ref_id(link, data, size) < 0)
		return -1;
	lr = arr_sorted_find(&rndr->refs, link, cmp_link_ref);
	if (!lr) return -1;

	/* fill the output buffers */
	link->size = 0;
	if (lr->link)
		bufput(link, lr->link->data, lr->link->size);
	title->size = 0;
	if (lr->title)
		bufput(title, lr->title->data, lr->title->size);
	return 0; }


/* char_link • '[': parsing a link or an image */
static size_t
char_link(struct buf *ob, struct render *rndr,
				char *data, size_t offset, size_t size) {
	int is_img = (offset && data[-1] == '!'), level;
	size_t i = 1, txt_e;
	struct buf *content = 0;
	struct buf *link = 0;
	struct buf *title = 0;
	int ret;

	/* checking whether the correct renderer exists */
	if ((is_img && !rndr->make.image) || (!is_img && !rndr->make.link))
		return 0;

	/* looking for the matching closing bracket */
	for (level = 1; i < size; i += 1)
		if (data[i - 1] == '\\') continue;
		else if (data[i] == '[') level += 1;
		else if (data[i] == ']') {
			level -= 1;
			if (level <= 0) break; }
	if (i >= size) return 0;
	txt_e = i;
	i += 1;

	/* skip any amount of whitespace or newline */
	/* (this is much more laxist than original markdown syntax) */
	while (i < size
	&& (data[i] == ' ' || data[i] == '\t' || data[i] == '\n'))
		i += 1;

	/* allocate temporary buffers to store content, link and title */
	content = new_work_buffer(rndr);
	link = new_work_buffer(rndr);
	title = new_work_buffer(rndr);
	ret = 0; /* error if we don't get to the callback */

	/* inline style link */
	if (i < size && data[i] == '(') {
		size_t span_end = i;
		while (span_end < size
		&& !(data[span_end] == ')'
		 && (span_end == i || data[span_end - 1] != '\\')))
			span_end += 1;

		if (span_end >= size
		|| get_link_inline(link, title,
					data + i+1, span_end - (i+1)) < 0)
			goto char_link_cleanup;

		i = span_end + 1; }

	/* reference style link */
	else if (i < size && data[i] == '[') {
		char *id_data;
		size_t id_size, id_end = i;
		while (id_end < size && data[id_end] != ']')
			id_end += 1;

		if (id_end >= size)
			goto char_link_cleanup;

		if (i + 1 == id_end) {
			/* implicit id - use the contents */
			id_data = data + 1;
			id_size = txt_e - 1; }
		else {
			/* explici id - between brackets */
			id_data = data + i + 1;
			id_size = id_end - (i + 1); }

		if (get_link_ref(rndr, link, title, id_data, id_size) < 0)
			goto char_link_cleanup;

		i = id_end + 1; }

	/* shortcut reference style link */
	else {
		if (get_link_ref(rndr, link, title, data + 1, txt_e - 1) < 0)
			goto char_link_cleanup;

		/* rewinding the whitespace */
		i = txt_e + 1; }

	/* building content: img alt is escaped, link content is parsed */
	if (txt_e > 1) {
		if (is_img) bufput(content, data + 1, txt_e - 1);
		else parse_inline(content, rndr, data + 1, txt_e - 1); }

	/* calling the relevant rendering function */
	if (is_img) {
		if (ob->size && ob->data[ob->size - 1] == '!') ob->size -= 1;
		ret = rndr->make.image(ob, link, title, content,
							rndr->make.opaque); }
	else ret = rndr->make.link(ob, link, title, content, rndr->make.opaque);

	/* cleanup */
char_link_cleanup:
	release_work_buffer(rndr, title);
	release_work_buffer(rndr, link);
	release_work_buffer(rndr, content);
	return ret ? i : 0; }



/*********************************
 * BLOCK-LEVEL PARSING FUNCTIONS *
 *********************************/

/* is_empty • returns the line length when it is empty, 0 otherwise */
static size_t
is_empty(char *data, size_t size) {
	size_t i;
	for (i = 0; i < size && data[i] != '\n'; i += 1)
		if (data[i] != ' ' && data[i] != '\t') return 0;
	return i + 1; }


/* is_hrule • returns whether a line is a horizontal rule */
static int
is_hrule(char *data, size_t size) {
	size_t i = 0, n = 0;
	char c;

	/* skipping initial spaces */
	if (size < 3) return 0;
	if (data[0] == ' ') { i += 1;
	if (data[1] == ' ') { i += 1;
	if (data[2] == ' ') { i += 1; } } }

	/* looking at the hrule char */
	if (i + 2 >= size
	|| (data[i] != '*' && data[i] != '-' && data[i] != '_'))
		return 0;
	c = data[i];

	/* the whole line must be the char or whitespace */
	while (i < size && data[i] != '\n') {
		if (data[i] == c) n += 1;
		else if (data[i] != ' ' && data[i] != '\t')
			return 0;
		i += 1; }

	return n >= 3; }


/* is_headerline • returns whether the line is a setext-style hdr underline */
static int
is_headerline(char *data, size_t size) {
	size_t i = 0;

	/* test of level 1 header */
	if (data[i] == '=') {
		for (i = 1; i < size && data[i] == '='; i += 1);
		while (i < size && (data[i] == ' ' || data[i] == '\t')) i += 1;
		return (i >= size || data[i] == '\n') ? 1 : 0; }

	/* test of level 2 header */
	if (data[i] == '-') {
		for (i = 1; i < size && data[i] == '-'; i += 1);
		while (i < size && (data[i] == ' ' || data[i] == '\t')) i += 1;
		return (i >= size || data[i] == '\n') ? 2 : 0; }

	return 0; }


/* is_table_sep • returns wether there is a table separator at the given pos */
static int
is_table_sep(char *data, size_t pos) {
	return data[pos] == '|' && (pos == 0 || data[pos - 1] != '\\'); }


/* is_tableline • returns the number of column tables in the given line */
static int
is_tableline(char *data, size_t size) {
	size_t i = 0;
	int n_sep = 0, outer_sep = 0;

	/* skip initial blanks */
	while (i < size && (data[i] == ' ' || data[i] == '\t'))
		i += 1;

	/* check for initial '|' */
	if (i < size && data[i] == '|')
		outer_sep += 1;

	/* count the number of pipes in the line */
	for (n_sep = 0; i < size && data[i] != '\n'; i += 1)
		if (is_table_sep(data, i))
			n_sep += 1;

	/* march back to check for optional last '|' before blanks and EOL */
	while (i
	&& (data[i - 1] == ' ' || data[i - 1] == '\t' || data[i - 1] == '\n'))
		i -= 1;
	if (i && is_table_sep(data, i - 1))
		outer_sep += 1;

	/* return the number of column or 0 if it's not a table line */
	return (n_sep > 0) ? (n_sep - outer_sep + 1) : 0; }


/* prefix_quote • returns blockquote prefix length */
static size_t
prefix_quote(char *data, size_t size) {
	size_t i = 0;
	if (i < size && data[i] == ' ') i += 1;
	if (i < size && data[i] == ' ') i += 1;
	if (i < size && data[i] == ' ') i += 1;
	if (i < size && data[i] == '>') {
		if (i + 1 < size && (data[i + 1] == ' ' || data[i+1] == '\t'))
			return i + 2;
		else return i + 1; }
	else return 0; }


/* prefix_code • returns prefix length for block code*/
static size_t
prefix_code(char *data, size_t size) {
	if (size > 0 && data[0] == '\t') return 1;
	if (size > 3 && data[0] == ' ' && data[1] == ' '
			&& data[2] == ' ' && data[3] == ' ') return 4;
	return 0; }

/* prefix_oli • returns ordered list item prefix */
static size_t
prefix_oli(char *data, size_t size) {
	size_t i = 0;
	if (i < size && data[i] == ' ') i += 1;
	if (i < size && data[i] == ' ') i += 1;
	if (i < size && data[i] == ' ') i += 1;
	if (i >= size || data[i] < '0' || data[i] > '9') return 0;
	while (i < size && data[i] >= '0' && data[i] <= '9') i += 1;
	if (i + 1 >= size || data[i] != '.'
	|| (data[i + 1] != ' ' && data[i + 1] != '\t')) return 0;
	i = i + 2;
	while (i < size && (data[i] == ' ' || data[i] == '\t')) i += 1;
	return i; }


/* prefix_uli • returns ordered list item prefix */
static size_t
prefix_uli(char *data, size_t size) {
	size_t i = 0;
	if (i < size && data[i] == ' ') i += 1;
	if (i < size && data[i] == ' ') i += 1;
	if (i < size && data[i] == ' ') i += 1;
	if (i + 1 >= size
	|| (data[i] != '*' && data[i] != '+' && data[i] != '-')
	|| (data[i + 1] != ' ' && data[i + 1] != '\t'))
		return 0;
	i = i + 2;
	while (i < size && (data[i] == ' ' || data[i] == '\t')) i += 1;
	return i; }


/* parse_block • parsing of one block, returning next char to parse */
static void parse_block(struct buf *ob, struct render *rndr,
			char *data, size_t size);


/* parse_blockquote • hanldes parsing of a blockquote fragment */
static size_t
parse_blockquote(struct buf *ob, struct render *rndr,
			char *data, size_t size) {
	size_t beg, end = 0, pre, work_size = 0;
	char *work_data = 0;
	struct buf *out = new_work_buffer(rndr);

	beg = 0;
	while (beg < size) {
		for (end = beg + 1; end < size && data[end - 1] != '\n';
							end += 1);
		pre = prefix_quote(data + beg, end - beg);
		if (pre) beg += pre; /* skipping prefix */
		else if (is_empty(data + beg, end - beg)
		&& (end >= size || (prefix_quote(data + end, size - end) == 0
					&& !is_empty(data + end, size - end))))
			/* empty line followed by non-quote line */
			break;
		if (beg < end) { /* copy into the in-place working buffer */
			/* bufput(work, data + beg, end - beg); */
			if (!work_data)
				work_data = data + beg;
			else if (data + beg != work_data + work_size)
				memmove(work_data + work_size, data + beg,
						end - beg);
			work_size += end - beg; }
		beg = end; }

	parse_block(out, rndr, work_data, work_size);
	if (rndr->make.blockquote)
		rndr->make.blockquote(ob, out, rndr->make.opaque);
	release_work_buffer(rndr, out);
	return end; }


/* parse_blockquote • hanldes parsing of a regular paragraph */
static size_t
parse_paragraph(struct buf *ob, struct render *rndr,
			char *data, size_t size) {
	size_t i = 0, end = 0;
	int level = 0;
	struct buf work = { data, 0, 0, 0, 0 }; /* volatile working buffer */

	while (i < size) {
		for (end = i + 1; end < size && data[end - 1] != '\n';
								end += 1);
		if (is_empty(data + i, size - i)
		|| (level = is_headerline(data + i, size - i)) != 0)
			break;
		if ((i && data[i] == '#')
		|| is_hrule(data + i, size - i)) {
			end = i;
			break; }
		i = end; }

	work.size = i;
	while (work.size && data[work.size - 1] == '\n')
		work.size -= 1;
	if (!level) {
		struct buf *tmp = new_work_buffer(rndr);
		parse_inline(tmp, rndr, work.data, work.size);
		if (rndr->make.paragraph)
			rndr->make.paragraph(ob, tmp, rndr->make.opaque);
		release_work_buffer(rndr, tmp); }
	else {
		if (work.size) {
			size_t beg;
			i = work.size;
			work.size -= 1;
			while (work.size && data[work.size] != '\n')
				work.size -= 1;
			beg = work.size + 1;
			while (work.size && data[work.size - 1] == '\n')
				work.size -= 1;
			if (work.size) {
				struct buf *tmp = new_work_buffer(rndr);
				parse_inline(tmp, rndr, work.data, work.size);
				if (rndr->make.paragraph)
					rndr->make.paragraph(ob, tmp,
							rndr->make.opaque);
				release_work_buffer(rndr, tmp);
				work.data += beg;
				work.size = i - beg; }
			else work.size = i; }
		if (rndr->make.header) {
			struct buf *span = new_work_buffer(rndr);
			parse_inline(span, rndr, work.data, work.size);
			rndr->make.header(ob, span, level,rndr->make.opaque);
			release_work_buffer(rndr, span); } }
	return end; }


/* parse_blockquote • hanldes parsing of a block-level code fragment */
static size_t
parse_blockcode(struct buf *ob, struct render *rndr,
			char *data, size_t size) {
	size_t beg, end, pre;
	struct buf *work = new_work_buffer(rndr);

	beg = 0;
	while (beg < size) {
		for (end = beg + 1; end < size && data[end - 1] != '\n';
							end += 1);
		pre = prefix_code(data + beg, end - beg);
		if (pre) beg += pre; /* skipping prefix */
		else if (!is_empty(data + beg, end - beg))
			/* non-empty non-prefixed line breaks the pre */
			break;
		if (beg < end) {
			/* verbatim copy to the working buffer,
				escaping entities */
			if (is_empty(data + beg, end - beg))
				bufputc(work, '\n');
			else bufput(work, data + beg, end - beg); }
		beg = end; }

	while (work->size && work->data[work->size - 1] == '\n')
		work->size -= 1;
	bufputc(work, '\n');
	if (rndr->make.blockcode)
		rndr->make.blockcode(ob, work, rndr->make.opaque);
	release_work_buffer(rndr, work);
	return beg; }


/* parse_listitem • parsing of a single list item */
/*	assuming initial prefix is already removed */
static size_t
parse_listitem(struct buf *ob, struct render *rndr,
			char *data, size_t size, int *flags) {
	struct buf *work = 0, *inter = 0;
	size_t beg = 0, end, pre, sublist = 0, orgpre = 0, i;
	int in_empty = 0, has_inside_empty = 0;

	/* keeping book of the first indentation prefix */
	if (size > 1 && data[0] == ' ') { orgpre = 1;
	if (size > 2 && data[1] == ' ') { orgpre = 2;
	if (size > 3 && data[2] == ' ') { orgpre = 3; } } }
	beg = prefix_uli(data, size);
	if (!beg) beg = prefix_oli(data, size);
	if (!beg) return 0;
	/* skipping to the beginning of the following line */
	end = beg;
	while (end < size && data[end - 1] != '\n') end += 1;

	/* getting working buffers */
	work = new_work_buffer(rndr);
	inter = new_work_buffer(rndr);

	/* putting the first line into the working buffer */
	bufput(work, data + beg, end - beg);
	beg = end;

	/* process the following lines */
	while (beg < size) {
		end += 1;
		while (end < size && data[end - 1] != '\n') end += 1;

		/* process an empty line */
		if (is_empty(data + beg, end - beg)) {
			in_empty = 1;
			beg = end;
			continue; }

		/* calculating the indentation */
		i = 0;
		if (end - beg > 1 && data[beg] == ' ') { i = 1;
		if (end - beg > 2 && data[beg + 1] == ' ') { i = 2;
		if (end - beg > 3 && data[beg + 2] == ' ') { i = 3;
		if (end - beg > 3 && data[beg + 3] == ' ') { i = 4; } } } }
		pre = i;
		if (data[beg] == '\t') { i = 1; pre = 8; }

		/* checking for a new item */
		if ((prefix_uli(data + beg + i, end - beg - i)
			&& !is_hrule(data + beg + i, end - beg - i))
		||  prefix_oli(data + beg + i, end - beg - i)) {
			if (in_empty) has_inside_empty = 1;
			if (pre == orgpre) /* the following item must have */
				break;             /* the same indentation */
			if (!sublist) sublist = work->size; }

		/* joining only indented stuff after empty lines */
		else if (in_empty && i < 4 && data[beg] != '\t') {
				*flags |= MKD_LI_END;
				break; }
		else if (in_empty) {
			bufputc(work, '\n');
			has_inside_empty = 1; }
		in_empty = 0;

		/* adding the line without prefix into the working buffer */
		bufput(work, data + beg + i, end - beg - i);
		beg = end; }

	/* render of li contents */
	if (has_inside_empty) *flags |= MKD_LI_BLOCK;
	if (*flags & MKD_LI_BLOCK) {
		/* intermediate render of block li */
		if (sublist && sublist < work->size) {
			parse_block(inter, rndr, work->data, sublist);
			parse_block(inter, rndr, work->data + sublist,
						work->size - sublist); }
		else
			parse_block(inter, rndr, work->data, work->size); }
	else {
		/* intermediate render of inline li */
		if (sublist && sublist < work->size) {
			parse_inline(inter, rndr, work->data, sublist);
			parse_block(inter, rndr, work->data + sublist,
						work->size - sublist); }
		else
			parse_inline(inter, rndr, work->data, work->size); }

	/* render of li itself */
	if (rndr->make.listitem)
		rndr->make.listitem(ob, inter, *flags, rndr->make.opaque);
	release_work_buffer(rndr, inter);
	release_work_buffer(rndr, work);
	return beg; }


/* parse_list • parsing ordered or unordered list block */
static size_t
parse_list(struct buf *ob, struct render *rndr,
			char *data, size_t size, int flags) {
	struct buf *work = new_work_buffer(rndr);
	size_t i = 0, j;

	while (i < size) {
		j = parse_listitem(work, rndr, data + i, size - i, &flags);
		i += j;
		if (!j || (flags & MKD_LI_END)) break; }

	if (rndr->make.list)
		rndr->make.list(ob, work, flags, rndr->make.opaque);
	release_work_buffer(rndr, work);
	return i; }


/* parse_atxheader • parsing of atx-style headers */
static size_t
parse_atxheader(struct buf *ob, struct render *rndr,
			char *data, size_t size) {
	int level = 0;
	size_t i, end, skip, span_beg, span_size;

	if (!size || data[0] != '#') return 0;

	while (level < size && level < 6 && data[level] == '#') level += 1;
	for (i = level; i < size && (data[i] == ' ' || data[i] == '\t');
							i += 1);
	span_beg = i;

	for (end = i; end < size && data[end] != '\n'; end += 1);
	skip = end;
	if (end <= i)
		return parse_paragraph(ob, rndr, data, size);
	while (end && data[end - 1] == '#') end -= 1;
	while (end && (data[end - 1] == ' ' || data[end - 1] == '\t')) end -= 1;
	if (end <= i)
		return parse_paragraph(ob, rndr, data, size);

	span_size = end - span_beg;
	if (rndr->make.header) {
		struct buf *span = new_work_buffer(rndr);
		parse_inline(span, rndr, data + span_beg, span_size);
		rndr->make.header(ob, span, level, rndr->make.opaque);
		release_work_buffer(rndr, span); }
	return skip; }


/* htmlblock_end • checking end of HTML block : </tag>[ \t]*\n[ \t*]\n */
/*	returns the length on match, 0 otherwise */
static size_t
htmlblock_end(struct html_tag *tag, char *data, size_t size) {
	size_t i, w;

	/* assuming data[0] == '<' && data[1] == '/' already tested */

	/* checking tag is a match */
	if (tag->size + 3 >= size
	|| strncasecmp(data + 2, tag->text, tag->size)
	|| data[tag->size + 2] != '>')
		return 0;

	/* checking white lines */
	i = tag->size + 3;
	w = 0;
	if (i < size && (w = is_empty(data + i, size - i)) == 0)
		return 0; /* non-blank after tag */
	i += w;
	w = 0;
	if (i < size && (w = is_empty(data + i, size - i)) == 0)
		return 0; /* non-blank line after tag line */
	return i + w; }


/* parse_htmlblock • parsing of inline HTML block */
static size_t
parse_htmlblock(struct buf *ob, struct render *rndr,
			char *data, size_t size) {
	size_t i, j = 0;
	struct html_tag *curtag;
	int found;
	struct buf work = { data, 0, 0, 0, 0 };

	/* identification of the opening tag */
	if (size < 2 || data[0] != '<') return 0;
	curtag = find_block_tag(data + 1, size - 1);

	/* handling of special cases */
	if (!curtag) {
		/* HTML comment, laxist form */
		if (size > 5 && data[1] == '!'
		&& data[2] == '-' && data[3] == '-') {
			i = 5;
			while (i < size
			&& !(data[i - 2] == '-' && data[i - 1] == '-'
						&& data[i] == '>'))
				i += 1;
			i += 1;
			if (i < size)
				j = is_empty(data + i, size - i);
				if (j) {
					work.size = i + j;
					if (rndr->make.blockhtml)
						rndr->make.blockhtml(ob, &work,
							rndr->make.opaque);
					return work.size; } }

		/* HR, which is the only self-closing block tag considered */
		if (size > 4
		&& (data[1] == 'h' || data[1] == 'H')
		&& (data[2] == 'r' || data[2] == 'R')) {
			i = 3;
			while (i < size && data[i] != '>')
				i += 1;
			if (i + 1 < size) {
				i += 1;
				j = is_empty(data + i, size - i);
				if (j) {
					work.size = i + j;
					if (rndr->make.blockhtml)
						rndr->make.blockhtml(ob, &work,
							rndr->make.opaque);
					return work.size; } } }

		/* no special case recognised */
		return 0; }

	/* looking for an unindented matching closing tag */
	/*	followed by a blank line */
	i = 1;
	found = 0;
#if 0
	while (i < size) {
		i += 1;
		while (i < size && !(data[i - 2] == '\n'
		&& data[i - 1] == '<' && data[i] == '/'))
			i += 1;
		if (i + 2 + curtag->size >= size) break;
		j = htmlblock_end(curtag, data + i - 1, size - i + 1);
		if (j) {
			i += j - 1;
			found = 1;
			break; } }
#endif

	/* if not found, trying a second pass looking for indented match */
	/* but not if tag is "ins" or "del" (following original Markdown.pl) */
	if (!found && curtag != INS_TAG && curtag != DEL_TAG) {
		i = 1;
		while (i < size) {
			i += 1;
			while (i < size
			&& !(data[i - 1] == '<' && data[i] == '/'))
				i += 1;
		if (i + 2 + curtag->size >= size) break;
		j = htmlblock_end(curtag, data + i - 1, size - i + 1);
		if (j) {
			i += j - 1;
			found = 1;
			break; } } }

	if (!found) return 0;

	/* the end of the block has been found */
	work.size = i;
	if (rndr->make.blockhtml)
		rndr->make.blockhtml(ob, &work, rndr->make.opaque);
	return i; }


/* parse_table_cell • parse a cell inside a table */
static void
parse_table_cell(struct buf *ob, struct render *rndr, char *data, size_t size,
				int flags) {
	struct buf *span = new_work_buffer(rndr);
	parse_inline(span, rndr, data, size);
	rndr->make.table_cell(ob, span, flags, rndr->make.opaque);
	release_work_buffer(rndr, span); }


/* parse_table_row • parse an input line into a table row */
static size_t
parse_table_row(struct buf *ob, struct render *rndr, char *data, size_t size,
				int *aligns, size_t align_size, int flags) {
	size_t i = 0, col = 0;
	size_t beg, end, total = 0;
	struct buf *cells = new_work_buffer(rndr);
	int align;

	/* skip leading blanks and sperator */
	while (i < size && (data[i] == ' ' || data[i] == '\t'))
		i += 1;
	if (i < size && data[i] == '|')
		i += 1;

	/* go over all the cells */
	while (i < size && total == 0) {
		/* check optional left/center align marker */
		align = 0;
		if (data[i] == ':') {
			align |= MKD_CELL_ALIGN_LEFT;
			i += 1; }

		/* skip blanks */
		while (i < size && (data[i] == ' ' || data[i] == '\t'))
			i += 1;
		beg = i;

		/* forward to the next separator or EOL */
		while (i < size && !is_table_sep(data, i) && data[i] != '\n')
			i += 1;
		end = i;
		if (i < size) {
			i += 1;
			if (data[i - 1] == '\n')
				total = i; }

		/* check optional right/center align marker */
		if (i > beg && data[end - 1] == ':') {
			align |= MKD_CELL_ALIGN_RIGHT;
			end -= 1; }

		/* remove trailing blanks */
		while (end > beg
		&& (data[end - 1] == ' ' || data[end - 1] == '\t'))
			end -= 1;

		/* skip the last cell if it was only blanks */
		/* (because it is only the optional end separator) */
		if (total && end <= beg) continue;

		/* fallback on default alignment if not explicit */
		if (align == 0 && aligns && col < align_size)
			align = aligns[col];

		/* render cells */
		parse_table_cell(cells, rndr, data + beg, end - beg,
		    align | flags);

		col += 1; }

	/* render the whole row and clean up */
	rndr->make.table_row(ob, cells, flags, rndr->make.opaque);
	release_work_buffer(rndr, cells);
	return total ? total : size; }


/* parse_table • parsing of a whole table */
static size_t
parse_table(struct buf *ob, struct render *rndr, char *data, size_t size) {
	size_t i = 0, head_end, col;
	size_t align_size = 0;
	int *aligns = 0;
	struct buf *head = 0;
	struct buf *rows = new_work_buffer(rndr);

	/* skip the first (presumably header) line */
	while (i < size && data[i] != '\n')
		i += 1;
	head_end = i;

	/* fallback on end of input */
	if (i >= size) {
		parse_table_row(rows, rndr, data, size, 0, 0, 0);
		rndr->make.table(ob, 0, rows, rndr->make.opaque);
		release_work_buffer(rndr, rows);
		return i; }

	/* attempt to parse a table rule, i.e. blanks, dash, colons and sep */
	i += 1;
	col = 0;
	while (i < size && (data[i] == ' ' || data[i] == '\t' || data[i] == '-'
			 || data[i] == ':' || data[i] == '|')) {
		if (data[i] == '|') align_size += 1;
		if (data[i] == ':') col = 1;
		i += 1; }

	if (i < size && data[i] == '\n') {
		align_size += 1;

		/* render the header row */
		head = new_work_buffer(rndr);
		parse_table_row(head, rndr, data, head_end, 0, 0,
		    MKD_CELL_HEAD);

		/* parse alignments if provided */
		if (col && (aligns = malloc(align_size * sizeof *aligns)) != 0){
			for (i = 0; i < align_size; i += 1)
				aligns[i] = 0;
			col = 0;
			i = head_end + 1;

			/* skip initial white space and optional separator */
			while (i < size && (data[i] == ' ' || data[i] == '\t'))
				i += 1;
			if (data[i] == '|') i += 1;

			/* compute default alignment for each column */
			while (i < size && data[i] != '\n') {
				if (data[i] == ':')
					aligns[col] |= MKD_CELL_ALIGN_LEFT;
				while (i < size
				&& data[i] != '|' && data[i] != '\n')
					i += 1;
				if (data[i - 1] == ':')
					aligns[col] |= MKD_CELL_ALIGN_RIGHT;
				if (i < size && data[i] == '|')
					i += 1;
				col += 1; } }

		/* point i to the beginning of next line/row */
		i += 1; }

	else {
		/* there is no valid ruler, continuing without header */
		i = 0; }

	/* render the table body lines */
	while (i < size && is_tableline(data + i, size - i))
		i += parse_table_row(rows, rndr, data + i, size - i,
		    aligns, align_size, 0);

	/* render the full table */
	rndr->make.table(ob, head, rows, rndr->make.opaque);

	/* cleanup */
	if (head) release_work_buffer(rndr, head);
	release_work_buffer(rndr, rows);
	free(aligns);
	return i; }


/* parse_block • parsing of one block, returning next char to parse */
static void
parse_block(struct buf *ob, struct render *rndr,
			char *data, size_t size) {
	size_t beg, end, i;
	char *txt_data;
	int has_table = (rndr->make.table && rndr->make.table_row
	    && rndr->make.table_cell);

	if (rndr->work.size > rndr->make.max_work_stack) {
		if (size) bufput(ob, data, size);
		return; }

	beg = 0;
	while (beg < size) {
		txt_data = data + beg;
		end = size - beg;
		if (data[beg] == '#')
			beg += parse_atxheader(ob, rndr, txt_data, end);
		else if (data[beg] == '<' && rndr->make.blockhtml
			&& (i = parse_htmlblock(ob, rndr, txt_data, end)) != 0)
			beg += i;
		else if ((i = is_empty(txt_data, end)) != 0)
			beg += i;
		else if (is_hrule(txt_data, end)) {
			if (rndr->make.hrule)
				rndr->make.hrule(ob, rndr->make.opaque);
			while (beg < size && data[beg] != '\n') beg += 1;
			beg += 1; }
		else if (prefix_quote(txt_data, end))
			beg += parse_blockquote(ob, rndr, txt_data, end);
		else if (prefix_code(txt_data, end))
			beg += parse_blockcode(ob, rndr, txt_data, end);
		else if (prefix_uli(txt_data, end))
			beg += parse_list(ob, rndr, txt_data, end, 0);
		else if (prefix_oli(txt_data, end))
			beg += parse_list(ob, rndr, txt_data, end,
						MKD_LIST_ORDERED);
		else if (has_table && is_tableline(txt_data, end))
			beg += parse_table(ob, rndr, txt_data, end);
		else
			beg += parse_paragraph(ob, rndr, txt_data, end); } }



/*********************
 * REFERENCE PARSING *
 *********************/

/* is_ref • returns whether a line is a reference or not */
static int
is_ref(char *data, size_t beg, size_t end, size_t *last, struct array *refs) {
	size_t i = 0;
	size_t id_offset, id_end;
	size_t link_offset, link_end;
	size_t title_offset, title_end;
	size_t line_end;
	struct link_ref *lr;
	struct buf *id;

	/* up to 3 optional leading spaces */
	if (beg + 3 >= end) return 0;
	if (data[beg] == ' ') { i = 1;
	if (data[beg + 1] == ' ') { i = 2;
	if (data[beg + 2] == ' ') { i = 3;
	if (data[beg + 3] == ' ') return 0; } } }
	i += beg;

	/* id part: anything but a newline between brackets */
	if (data[i] != '[') return 0;
	i += 1;
	id_offset = i;
	while (i < end && data[i] != '\n' && data[i] != '\r' && data[i] != ']')
		i += 1;
	if (i >= end || data[i] != ']') return 0;
	id_end = i;

	/* spacer: colon (space | tab)* newline? (space | tab)* */
	i += 1;
	if (i >= end || data[i] != ':') return 0;
	i += 1;
	while (i < end && (data[i] == ' ' || data[i] == '\t')) i += 1;
	if (i < end && (data[i] == '\n' || data[i] == '\r')) {
		i += 1;
		if (i < end && data[i] == '\r' && data[i - 1] == '\n') i += 1; }
	while (i < end && (data[i] == ' ' || data[i] == '\t')) i += 1;
	if (i >= end) return 0;

	/* link: whitespace-free sequence, optionally between angle brackets */
	if (data[i] == '<') i += 1;
	link_offset = i;
	while (i < end && data[i] != ' ' && data[i] != '\t'
			&& data[i] != '\n' && data[i] != '\r') i += 1;
	if (data[i - 1] == '>') link_end = i - 1;
	else link_end = i;

	/* optional spacer: (space | tab)* (newline | '\'' | '"' | '(' ) */
	while (i < end && (data[i] == ' ' || data[i] == '\t')) i += 1;
	if (i < end && data[i] != '\n' && data[i] != '\r'
			&& data[i] != '\'' && data[i] != '"' && data[i] != '(')
		return 0;
	line_end = 0;
	/* computing end-of-line */
	if (i >= end || data[i] == '\r' || data[i] == '\n') line_end = i;
	if (i + 1 < end && data[i] == '\n' && data[i + 1] == '\r')
		line_end = i + 1;

	/* optional (space|tab)* spacer after a newline */
	if (line_end) {
		i = line_end + 1;
		while (i < end && (data[i] == ' ' || data[i] == '\t')) i += 1; }

	/* optional title: any non-newline sequence enclosed in '"()
					alone on its line */
	title_offset = title_end = 0;
	if (i + 1 < end
	&& (data[i] == '\'' || data[i] == '"' || data[i] == '(')) {
		i += 1;
		title_offset = i;
		/* looking for EOL */
		while (i < end && data[i] != '\n' && data[i] != '\r') i += 1;
		if (i + 1 < end && data[i] == '\n' && data[i + 1] == '\r')
			title_end = i + 1;
		else	title_end = i;
		/* stepping back */
		i -= 1;
		while (i > title_offset && (data[i] == ' ' || data[i] == '\t'))
			i -= 1;
		if (i > title_offset
		&& (data[i] == '\'' || data[i] == '"' || data[i] == ')')) {
			line_end = title_end;
			title_end = i; } }
	if (!line_end) return 0; /* garbage after the link */

	/* a valid ref has been found, filling-in return structures */
	if (last) *last = line_end;
	if (!refs) return 1;
	id = bufnew(WORK_UNIT);
	if (build_ref_id(id, data + id_offset, id_end - id_offset) < 0) {
		bufrelease(id);
		return 0; }
	lr = arr_item(refs, arr_newitem(refs));
	lr->id = id;
	lr->link = bufnew(link_end - link_offset);
	bufput(lr->link, data + link_offset, link_end - link_offset);
	if (title_end > title_offset) {
		lr->title = bufnew(title_end - title_offset);
		bufput(lr->title, data + title_offset,
					title_end - title_offset); }
	else lr->title = 0;
	return 1; }



/**********************
 * EXPORTED FUNCTIONS *
 **********************/

/* markdown • parses the input buffer and renders it into the output buffer */
void
markdown(struct buf *ob, struct buf *ib, const struct mkd_renderer *rndrer) {
	struct link_ref *lr;
	struct buf *text = bufnew(TEXT_UNIT);
	size_t i, beg, end;
	struct render rndr;

	/* filling the render structure */
	if (!rndrer) return;
	rndr.make = *rndrer;
	if (rndr.make.max_work_stack < 1)
		rndr.make.max_work_stack = 1;
	arr_init(&rndr.refs, sizeof (struct link_ref));
	parr_init(&rndr.work);
	for (i = 0; i < 256; i += 1) rndr.active_char[i] = 0;
	if ((rndr.make.emphasis || rndr.make.double_emphasis
						|| rndr.make.triple_emphasis)
	&& rndr.make.emph_chars)
		for (i = 0; rndr.make.emph_chars[i]; i += 1)
			rndr.active_char[(unsigned char)rndr.make.emph_chars[i]]
				= char_emphasis;
	if (rndr.make.codespan) rndr.active_char['`'] = char_codespan;
	if (rndr.make.linebreak) rndr.active_char['\n'] = char_linebreak;
	if (rndr.make.image || rndr.make.link)
		rndr.active_char['['] = char_link;
	rndr.active_char['<'] = char_langle_tag;
	rndr.active_char['\\'] = char_escape;
	rndr.active_char['&'] = char_entity;

	/* first pass: looking for references, copying everything else */
	beg = 0;
	while (beg < ib->size) /* iterating over lines */
		if (is_ref(ib->data, beg, ib->size, &end, &rndr.refs))
			beg = end;
		else { /* skipping to the next line */
			end = beg;
			while (end < ib->size
			&& ib->data[end] != '\n' && ib->data[end] != '\r')
				end += 1;
			/* adding the line body if present */
			if (end > beg) bufput(text, ib->data + beg, end - beg);
			while (end < ib->size
			&& (ib->data[end] == '\n' || ib->data[end] == '\r')) {
				/* add one \n per newline */
				if (ib->data[end] == '\n'
				|| (end + 1 < ib->size
						&& ib->data[end + 1] != '\n'))
					bufputc(text, '\n');
				end += 1; }
			beg = end; }

	/* sorting the reference array */
	if (rndr.refs.size)
		qsort(rndr.refs.base, rndr.refs.size, rndr.refs.unit,
					cmp_link_ref_sort);

	/* adding a final newline if not already present */
	if (text->size
	&&  text->data[text->size - 1] != '\n'
	&&  text->data[text->size - 1] != '\r')
		bufputc(text, '\n');

	/* second pass: actual rendering */
	if (rndr.make.prolog)
		rndr.make.prolog(ob, rndr.make.opaque);
	parse_block(ob, &rndr, text->data, text->size);
	if (rndr.make.epilog)
		rndr.make.epilog(ob, rndr.make.opaque);

	/* clean-up */
	bufrelease(text);
	lr = rndr.refs.base;
	for (i = 0; i < rndr.refs.size; i += 1) {
		bufrelease(lr[i].id);
		bufrelease(lr[i].link);
		bufrelease(lr[i].title); }
	arr_free(&rndr.refs);
	assert(rndr.work.size == 0);
	for (i = 0; i < rndr.work.asize; i += 1)
		bufrelease(rndr.work.item[i]);
	parr_free(&rndr.work); }

/* vim: set filetype=c: */
