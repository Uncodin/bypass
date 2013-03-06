# Nanoc

[Nanoc](http://nanoc.stoneship.org/) is a useful utility for generating a static
website. It serves as a filter pipeline in which you specify a course of events
that should take place based on certain criteria. For example, if you would like
to generate HTML from Markdown, you can instruct Nanoc to run Markdown files
though a processor such as RDiscount or BlueCloth via its [rule specification
file](http://nanoc.stoneship.org/docs/4-basic-concepts/#rules). Nanoc offers a
great degree of flexibility and customizability. This was its primary selling
point next to having good documentation, and ultimately was the reason we chose
it over its alternatives.

Nanoc correctly makes few assumptions about its application. Because of this,
we had to roll up our sleeves and knock out some custom code in order to get some
of the features we wanted. One feature that we found particularly useful in
our day-to-day interaction with Github was its support for Fenced Code Blocks
within a page of Markdown. Eg:

## Fenced Code Block Support

I added support for this feature with the following custom Nanoc filter:

    class FencedCodeBlock < Nanoc3::Filter
      identifier :fenced_code_block

      def run(content, params={})
        content.gsub(/(^[:backtick:]{3}\s*(\S*)\s*$([^[:backtick:]]*)^[:backtick:]{3}\s*$)+?/m) {|match|
          lang_spec  = $2
          code_block = $3

          replacement = '<div class="code-container">'

          if lang_spec && lang_spec.length > 0
            replacement << '<div class="langspec">'
            replacement << lang_spec.capitalize
            replacement << '</div>'
          end

          replacement << '<pre class="highlight"><code class="language'

          if lang_spec && lang_spec.length > 0
            replacement << '-'
            replacement << lang_spec
          end

          replacement << '">'

          code_block.gsub!("[:-backtick-:]", "[:backtick:]")

          coder = HTMLEntities.new
          replacement << coder.encode(code_block)
          replacement << '</code></pre></div>'
        }
      end
    end

A fenced code block demarcated by three back ticks followed by optional whitespace,
and an optional language on a single line. The code block can then be flush against
the left  margin (which differs from typical Markdown). End the code block with three
more backticks on a single line.

### Description

The aforementioned filter interprets fenced code blocks with the following algorithm:

* Locate a fenced code block
* Isolate the specified programming language
* Isolate the actual block of code
* Escape html entities in the block of code
* Return the following snippet of HTML

	<div class="code-container">
	  <div class="langspec">objective-c</div>
	  <pre class="highlight">
	    <code class="language-objective-c">
	      ... pygmentized markup ...
	    </code>
	  </pre>
	</div>