//
//  BPMarkdownViewController.m
//  BypassSample
//
//  Created by Damian Carrillo on 2/28/13.
//  Copyright (c) 2013 Uncodin. All rights reserved.
//

#import "BPMarkdownViewController.h"
#import <Bypass/Bypass.h>

@interface BPMarkdownViewController ()
@property (weak, nonatomic) IBOutlet UITextView *markdownView;

@end

@implementation BPMarkdownViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)viewWillAppear:(BOOL)animated
{
    NSString *example =
    @"Sundown\n"
    "=======\n"
    "\n"
    "`Sundown` is a Markdown parser based on the original code of the\n"
    "[Upskirt library](http://fossil.instinctive.eu/libupskirt/index) by Natacha Porté.\n"
    "\n"
    "Features\n"
    "--------\n"
    "\n"
    "*	**Fully standards compliant**\n"
    "\n"
    "	`Sundown` passes out of the box the official Markdown v1.0.0 and v1.0.3\n"
    "	test suites, and has been extensively tested with additional corner cases\n"
    "	to make sure its output is as sane as possible at all times.\n"
    "\n"
    "*	**Massive extension support**\n"
    "\n"
    "	`Sundown` has optional support for several (unofficial) Markdown extensions,\n"
    "	such as non-strict emphasis, fenced code blocks, tables, autolinks,\n"
    "	strikethrough and more.\n"
    "\n"
    "*	**UTF-8 aware**\n"
    "\n"
    "	`Sundown` is fully UTF-8 aware, both when parsing the source document and when\n"
    "	generating the resulting (X)HTML code.\n"
    "\n"
    "*	**Tested & Ready to be used on production**\n"
    "\n"
    "	`Sundown` has been extensively security audited, and includes protection against\n"
    "	all possible DOS attacks (stack overflows, out of memory situations, malformed\n"
    "	Markdown syntax...) and against client attacks through malicious embedded HTML.\n"
    "\n"
    "	We've worked very hard to make `Sundown` never crash or run out of memory\n"
    "	under *any* input. `Sundown` renders all the Markdown content in GitHub and so\n"
    "	far hasn't crashed a single time.\n"
    "\n"
    "*	**Customizable renderers**\n"
    "\n"
    "	`Sundown` is not stuck with XHTML output: the Markdown parser of the library\n"
    "	is decoupled from the renderer, so it's trivial to extend the library with\n"
    "	custom renderers. A fully functional (X)HTML renderer is included.\n"
    "\n"
    "*	**Optimized for speed**\n"
    "\n"
    "	`Sundown` is written in C, with a special emphasis on performance. When wrapped\n"
    "	on a dynamic language such as Python or Ruby, it has shown to be up to 40\n"
    "	times faster than other native alternatives.\n"
    "\n"
    "*	**Zero-dependency**\n"
    "\n"
    "	`Sundown` is a zero-dependency library composed of 3 `.c` files and their headers.\n"
    "	No dependencies, no bullshit. Only standard C99 that builds everywhere.\n"
    "\n"
    "Credits\n"
    "-------\n"
    "\n"
    "`Sundown` is based on the original Upskirt parser by Natacha Porté, with many additions\n"
    "by Vicent Marti (@tanoku) and contributions from the following authors:\n"
    "\n"
    "	Ben Noordhuis, Bruno Michel, Joseph Koshy, Krzysztof Kowalczyk, Samuel Bronson,\n"
    "	Shuhei Tanuma\n"
    "\n"
    "Bindings\n"
    "--------\n"
    "\n"
    "`Sundown` is available from other programming languages thanks to these bindings developed\n"
    "by our awesome contributors.\n"
    "\n"
    "- [Redcarpet](https://github.com/tanoku/redcarpet) (Ruby)\n"
    "- [RobotSkirt](https://github.com/benmills/robotskirt) (Node.js)\n"
    "- [Misaka](https://github.com/FSX/misaka) (Python)\n"
    "- [ffi-sundown](https://github.com/postmodern/ffi-sundown) (Ruby FFI)\n"
    "- [Sundown HS](https://github.com/rostayob/sundown) (Haskell)\n"
    "- [Goskirt](https://github.com/madari/goskirt) (Go)\n"
    "- [Upskirt.go](https://github.com/buu700/upskirt.go) (Go)\n"
    "- [MoonShine](https://github.com/brandonc/moonshine) (.NET)\n"
    "- [PHP-Sundown](https://github.com/chobie/php-sundown) (PHP)\n"
    "- [Sundown.net](https://github.com/txdv/sundown.net) (.NET)\n"
    "\n"
    "Help us\n"
    "-------\n"
    "\n"
    "`Sundown` is all about security. If you find a (potential) security vulnerability in the\n"
    "library, or a way to make it crash through malicious input, please report it to us,\n"
    "either directly via email or by opening an Issue on GitHub, and help make the web safer\n"
    "for everybody.\n"
    "\n"
    "Unicode character handling\n"
    "--------------------------\n"
    "\n"
    "Given that the Markdown spec makes no provision for Unicode character handling, `Sundown`\n"
    "takes a conservative approach towards deciding which extended characters trigger Markdown\n"
    "features:\n"
    "\n"
    "*	Punctuation characters outside of the U+007F codepoint are not handled as punctuation.\n"
    "	They are considered as normal, in-word characters for word-boundary checks.\n"
    "\n"
    "*	Whitespace characters outside of the U+007F codepoint are not considered as\n"
    "	whitespace. They are considered as normal, in-word characters for word-boundary checks.\n"
    "\n"
    "Install\n"
    "-------\n"
    "\n"
    "There is nothing to install. `Sundown` is composed of 3 `.c` files (`markdown.c`,\n"
    "`buffer.c` and `array.c`), so just throw them in your project. Zero-dependency means\n"
    "zero-dependency. You might want to include `render/html.c` if you want to use the\n"
    "included XHTML renderer, or write your own renderer. Either way, it's all fun and joy.\n"
    "\n"
    "If you are hardcore, you can use the included `Makefile` to build `Sundown` into a dynamic\n"
    "library, or to build the sample `sundown` executable, which is just a commandline\n"
    "Markdown to XHTML parser. (If gcc gives you grief about `-fPIC`, e.g. with MinGW, try\n"
    "`make MFLAGS=` instead of just `make`.)\n"
    "\n"
    "License\n"
    "-------\n"
    "\n"
    "Permission to use, copy, modify, and distribute this software for any\n"
    "purpose with or without fee is hereby granted, provided that the above\n"
    "copyright notice and this permission notice appear in all copies.\n"
    "\n"
    "THE SOFTWARE IS PROVIDED \"AS IS\" AND THE AUTHOR DISCLAIMS ALL WARRANTIES\n"
    "WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF\n"
    "MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR\n"
    "ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES\n"
    "WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN\n"
    "ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF\n"
    "OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.\n"
    "\n"
    "<!-- Local Variables: -->\n"
    "<!-- fill-column: 89 -->\n"
    "<!-- End: -->\n";
    
    BPParser *parser = [[BPParser alloc] init];
    BPDocument *document = [parser parse:example];
    
    BPAttributedStringRenderer *renderer = [[BPAttributedStringRenderer alloc] init];
    NSAttributedString *attributedText = [renderer renderDocument:document];
    
    [[self markdownView] setAttributedText:attributedText];
}

@end
