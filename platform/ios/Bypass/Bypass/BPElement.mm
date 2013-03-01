//
//  BPElement.m
//  Bypass
//
//  Created by Damian Carrillo on 2/28/13.
//  Copyright (c) 2013 Uncodin. All rights reserved.
//

#import "BPElement.h"
#import "element.h"

// Block Element Types

const BPElementType BPBlockCode      = Bypass::BLOCK_CODE;
const BPElementType BPBlockQuote     = Bypass::BLOCK_QUOTE;
const BPElementType BPBlockHTML      = Bypass::BLOCK_HTML;
const BPElementType BPHeader         = Bypass::HEADER;
const BPElementType BPHRule          = Bypass::HRULE;
const BPElementType BPList           = Bypass::LIST;
const BPElementType BPListItem       = Bypass::LIST_ITEM;
const BPElementType BPParagraph      = Bypass::PARAGRAPH;
const BPElementType BPTable          = Bypass::TABLE;
const BPElementType BPTableCell      = Bypass::TABLE_CELL;
const BPElementType BPTableRow       = Bypass::TABLE_ROW;

// Span Element Types

const BPElementType BPAutoLink       = Bypass::AUTOLINK;
const BPElementType BPCodeSpan       = Bypass::CODE_SPAN;
const BPElementType BPDoubleEmphasis = Bypass::DOUBLE_EMPHASIS;
const BPElementType BPEmphasis       = Bypass::EMPHASIS;
const BPElementType BPImage          = Bypass::IMAGE;
const BPElementType BPLineBreak      = Bypass::LINEBREAK;
const BPElementType BPLink           = Bypass::LINK;
const BPElementType BPRawHTMLTag     = Bypass::RAW_HTML_TAG;
const BPElementType BPTripleEmphasis = Bypass::TRIPLE_EMPHASIS;
const BPElementType BPText           = Bypass::TEXT;

@interface BPElement ()

@end

@implementation BPElement {
    Bypass::Element element;
}

- (BPElementType)type
{
    
}

@end
