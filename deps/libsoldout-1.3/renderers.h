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

#ifndef MARKDOWN_RENDERERS_H
#define MARKDOWN_RENDERERS_H

#include "markdown.h"


/*****************************
 * EXPORTED HELPER FUNCTIONS *
 *****************************/

/* lus_attr_escape • copy the buffer entity-escaping '<', '>', '&' and '"' */
void
lus_attr_escape(struct buf *ob, char *src, size_t size);

/* lus_body_escape • copy the buffer entity-escaping '<', '>' and '&' */
void
lus_body_escape(struct buf *ob, char *src, size_t size);



/***********************
 * RENDERER STRUCTURES *
 ***********************/

/* original markdown renderers */
extern const struct mkd_renderer mkd_html;  /* HTML 4 renderer */
extern const struct mkd_renderer mkd_xhtml; /* XHTML 1.0 renderer */

/* renderers with some discount extensions */
extern const struct mkd_renderer discount_html;
extern const struct mkd_renderer discount_xhtml;

/* renderers with Natasha's own extensions */
extern const struct mkd_renderer nat_html;
extern const struct mkd_renderer nat_xhtml;

#endif /* ndef MARKDOWN_RENDERERS_H */
