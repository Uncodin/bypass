//
//  BPElement.m
//  Bypass
//
//  Created by Damian Carrillo on 2/28/13.
//  Copyright 2013 Uncodin, Inc.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//

#import "BPElementPrivate.h"

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
const BPElementType BPStrikethrough  = Bypass::STRIKETHROUGH;

@interface BPElement ()

@end

@implementation BPElement
{
           Bypass::Element  _element;
           NSString        *_text;
           NSDictionary    *_attributes;
    __weak BPElement       *_parentElement;
           NSArray         *_childElements;
}

- (id)init
{
    Bypass::Element element;
    return [self initWithElement:element];
}

- (id)initWithElement:(Bypass::Element)element
{
    return [self initWithElement:element parentElement:nil];
}

- (id)initWithElement:(Bypass::Element)element parentElement:(BPElement *)parentElement
{
    self = [super init];
    
    if (self != nil) {
        _element = element;
        _parentElement = parentElement;
    }
    
    return self;
}

- (BPElementType)elementType
{
    return _element.getType();
}

- (BOOL)isBlockElement
{
    return _element.isBlockElement();
}

- (NSString *)text
{
    using namespace std;
    
    if (_text == nil) {
        string t = _element.getText();
        
        if (t.length() > 0) {
            _text = [NSString stringWithCString:t.c_str() encoding:NSUTF8StringEncoding];
        }
    }
    
    return _text;
}

- (NSDictionary *)attributes
{
    using namespace Bypass;
    
    if (_attributes == nil) {
        Element::AttributeMap::iterator it = _element.attrBegin();
        NSMutableDictionary *attributes;
        attributes = [NSMutableDictionary dictionaryWithCapacity:_element.attrSize()];

		for (; it != _element.attrEnd(); ++it) {
			if (!it->first.empty() && !it->second.empty()) {
				NSString *nn = [NSString stringWithUTF8String:it->first.c_str()];
				NSString *vv = [NSString stringWithUTF8String:it->second.c_str()];

				[attributes setObject:vv forKey:nn];
			}
		}

        _attributes = [NSDictionary dictionaryWithDictionary:attributes];
    }
    
    return _attributes;
}

- (NSArray *)childElements
{
    using namespace Bypass;
    
    if (_childElements == nil) {
        size_t i, count = _element.size();
        
        NSMutableArray *childElements = [NSMutableArray arrayWithCapacity:count];
        
        for (i = 0; i < count; ++i) {
            Element c = _element[i];
            BPElement *cc = [[BPElement alloc] initWithElement:c parentElement:self];
            childElements[i] = cc;
        }
        
        _childElements = [NSArray arrayWithArray:childElements];
    }
    
    return _childElements;
}

#if __has_feature(objc_subscripting)

- (id)objectAtIndexedSubscript:(NSUInteger)idx
{
    return [[self childElements] objectAtIndexedSubscript:idx];
}

- (id)objectForKeyedSubscript:(id)key
{
    return [[self attributes] objectForKeyedSubscript:key];
}

#endif

- (NSString *)debugDescription
{
    NSString *elementType;
    
    if ([self elementType] == BPBlockCode) {
        elementType = @"BPBlockCode";
    } else if ([self elementType] == BPBlockQuote) {
        elementType = @"BPBlockQuote";
    } else if ([self elementType] == BPBlockHTML) {
        elementType = @"BPBlockHTML";
    } else if ([self elementType] == BPHeader) {
        elementType = @"BPHeader";
    } else if ([self elementType] == BPHRule) {
        elementType = @"BPHRule";
    } else if ([self elementType] == BPList) {
        elementType = @"BPList";
    } else if ([self elementType] == BPListItem) {
        elementType = @"BPListItem";
    } else if ([self elementType] == BPParagraph) {
        elementType = @"BPParagraph";
    } else if ([self elementType] == BPTable) {
        elementType = @"BPTable";
    } else if ([self elementType] == BPTableCell) {
        elementType = @"BPTableCell";
    } else if ([self elementType] == BPTableRow) {
        elementType = @"BPTableRow";
    } else if ([self elementType] == BPAutoLink) {
        elementType = @"BPAutoLink";
    } else if ([self elementType] == BPCodeSpan) {
        elementType = @"BPCodeSpan";
    } else if ([self elementType] == BPDoubleEmphasis) {
        elementType = @"BPDoubleEmphasis";
    } else if ([self elementType] == BPEmphasis) {
        elementType = @"BPEmphasis";
    } else if ([self elementType] == BPImage) {
        elementType = @"BPImage";
    } else if ([self elementType] == BPLineBreak) {
        elementType = @"BPLineBreak";
    } else if ([self elementType] == BPLink) {
        elementType = @"BPLink";
    } else if ([self elementType] == BPRawHTMLTag) {
        elementType = @"BPRawHTMLTag";
    } else if ([self elementType] == BPTripleEmphasis) {
        elementType = @"BPTripleEmphasis";
    } else if ([self elementType] == BPText) {
        elementType = @"BPText";
    } else if ([self elementType] == BPStrikethrough) {
        elementType = @"BPStrikethrough";
    }
    
    NSMutableString *desc = [NSMutableString stringWithString:NSStringFromClass([self class])];
    [desc appendFormat:@"{ type: %@", elementType];
    [desc appendFormat:@", text: %@", [self text]];
    [desc appendFormat:@", attributes: %@", [[self attributes] debugDescription]];
    [desc appendString:@", childElements: {"];
    
    for (BPElement *childElement in [self childElements]) {
        [desc appendFormat:@"%@\n", [childElement debugDescription]];
    }
    
    [desc appendString:@"}"];
    [desc appendString:@"}"];
    return desc;
}

@end
