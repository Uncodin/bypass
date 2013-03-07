//
//  BPAttributedStringRenderer.m
//  Bypass
//
//  Created by Damian Carrillo on 3/1/13.
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

#import <CoreText/CoreText.h>
#import "BPAttributedStringRenderer.h"

@implementation BPAttributedStringRenderer
{
    CTFontRef _defaultFont;
    CTFontRef _boldFont;
    CTFontRef _italicFont;
    CTFontRef _boldItalicFont;
    CTFontRef _monospaceFont;
    CTFontRef _h1Font;
    CTFontRef _h2Font;
    CTFontRef _h3Font;
    CTFontRef _h4Font;
    CTFontRef _h5Font;
    CTFontRef _h6Font;
    CTFontRef _quoteFont;
}

- (id)init
{
    self = [super init];
    
    if (self != nil) {
        CGFloat systemFontSize = [UIFont systemFontSize];
        UIFont *systemFont = [UIFont systemFontOfSize:systemFontSize];
        CFStringRef systemFontName = (__bridge CFStringRef) [systemFont fontName];
        
        _defaultFont = CTFontCreateWithName(systemFontName, systemFontSize, NULL);
    }
    
    return self;
}

- (void)dealloc
{
    CFRelease(_defaultFont);
    if (_boldFont != NULL) CFRelease(_boldFont);
    if (_italicFont != NULL) CFRelease(_italicFont);
    if (_boldItalicFont != NULL) CFRelease(_boldItalicFont);
    if (_monospaceFont != NULL) CFRelease(_monospaceFont);
    if (_h1Font != NULL) CFRelease(_h1Font);
    if (_h2Font != NULL) CFRelease(_h2Font);
    if (_h3Font != NULL) CFRelease(_h3Font);
    if (_h4Font != NULL) CFRelease(_h4Font);
    if (_h5Font != NULL) CFRelease(_h5Font);
    if (_h6Font != NULL) CFRelease(_h6Font);
}

- (UIFont *)UIFontFromCTFont:(CTFontRef)ctFont
{
    NSString *fontName = (__bridge_transfer NSString *) CTFontCopyName(ctFont, kCTFontPostScriptNameKey);
    CGFloat fontSize = CTFontGetSize(ctFont);
    UIFont *font = [UIFont fontWithName:fontName size:fontSize];
    return font;
}

- (NSAttributedString *)renderDocument:(BPDocument *)document
{
    NSMutableAttributedString *target = [[NSMutableAttributedString alloc] init];

    for (BPElement *element in [document elements]) {
        [self renderElement:element toTarget:target];
    }
    
    return target;
}

- (void)renderElement:(BPElement *)element toTarget:(NSMutableAttributedString *)target
{
    // Capture the starting point of the effective range to apply attributes to
    
    NSRange effectiveRange;
    effectiveRange.location = [target length];
 
    BPElementType elementType = [element elementType];
    
    // Render span elements immediately, and for some block-level elements, insert special
    // characters
    
    if (elementType == BPListItem) {
        [self insertBulletIntoTarget:target];
    } else if (elementType == BPAutoLink) {
        [self renderLinkElement:element toTarget:target];
    } else if (elementType == BPCodeSpan) {
        [self renderCodeSpanElement:element toTarget:target];
    } else if (elementType == BPDoubleEmphasis) {
        [self renderBoldElement:element toTarget:target];
    } else if (elementType == BPEmphasis) {
        [self renderItalicElement:element toTarget:target];
    } else if (elementType == BPImage) {
        // Currently not supported
    } else if (elementType == BPLineBreak) {
        [self renderLineBreak:element toTarget:target];
    } else if (elementType == BPLink) {
        [self renderLinkElement:element toTarget:target];
    } else if (elementType == BPRawHTMLTag) {
        // Currently not supported
    } else if (elementType == BPTripleEmphasis) {
        [self renderBoldItalicElement:element toTarget:target];
    } else if (elementType == BPText) {
        [self renderTextElement:element toTarget:target];
    }
    
    // Render children of this particular element recursively
    
    for (BPElement *childElement in [element childElements]) {
        [self renderElement:childElement toTarget:target];
    }
    
    // Capture the end of the range
    
    effectiveRange.length = [target length] - effectiveRange.location;
    
    // Follow up with some types of block-level elements and apply properties en masse.
    
    if (elementType == BPParagraph) {
        [self renderParagraphElement:element inRange:effectiveRange toTarget:target];
    } else if (elementType == BPHeader) {
        [self renderHeaderElement:element inRange:effectiveRange toTarget:target];
    } else if (elementType == BPList) {
        [self renderListElement:element inRange:effectiveRange toTarget:target];
    } else if (elementType == BPListItem) {
        [self renderListItemElement:element inRange:effectiveRange toTarget:target];
    } else if (elementType == BPBlockCode) {
        [self renderBlockCodeElement:element inRange:effectiveRange toTarget:target];
    } else if (elementType == BPBlockQuote) {
        [self renderBlockQuoteElement:element inRange:effectiveRange toTarget:target];
    }
    
    if ([element isBlockElement] && [element elementType] != BPListItem) {
        [target appendAttributedString:[[NSAttributedString alloc] initWithString:@"\n"]];
    }
}

#pragma mark Block Element Rendering

- (void)renderBlockQuoteElement:(BPElement *)element
                        inRange:(NSRange)effectiveRange
                       toTarget:(NSMutableAttributedString *)target
{
    if (_quoteFont == NULL) {
        _quoteFont = CTFontCreateWithName(CFSTR("Marion-Italic"), CTFontGetSize(_defaultFont) + 2, NULL);
    }
    
    UIFont *quoteFont = [self UIFontFromCTFont:_quoteFont];
    
    NSMutableParagraphStyle *paragraphStyle = [[NSMutableParagraphStyle alloc] init];
    [paragraphStyle setParagraphSpacing:15.f];
    [paragraphStyle setFirstLineHeadIndent:23.f];
    [paragraphStyle setHeadIndent:23.f];
    [paragraphStyle setTailIndent:-23.f];
    
    NSDictionary *attributes = @{NSFontAttributeName : quoteFont, NSParagraphStyleAttributeName : paragraphStyle};
    
    [target addAttributes:attributes range:effectiveRange];
}

- (void)renderBlockCodeElement:(BPElement *)element
                       inRange:(NSRange)effectiveRange
                      toTarget:(NSMutableAttributedString *)target
{
    if (_monospaceFont == NULL) {
        _monospaceFont = CTFontCreateWithName(CFSTR("Courier"), CTFontGetSize(_defaultFont), NULL);
    }
    
    UIFont *monospaceFont = [self UIFontFromCTFont:_monospaceFont];
    NSDictionary *attributes = @{NSFontAttributeName : monospaceFont, NSForegroundColorAttributeName : [UIColor grayColor]};
    [target addAttributes:attributes range:effectiveRange];
    [self insertNewlineIntoTarget:target];
}

- (void)renderParagraphElement:(BPElement *)element
                       inRange:(NSRange)effectiveRange
                      toTarget:(NSMutableAttributedString *)target
{
    NSMutableParagraphStyle *paragraphStyle = [[NSMutableParagraphStyle alloc] init];
    [paragraphStyle setParagraphSpacing:20.f];
    
    NSDictionary *attributes = @{NSParagraphStyleAttributeName : paragraphStyle};
    [target addAttributes:attributes range:effectiveRange];
}

- (void)renderListElement:(BPElement *)element
                  inRange:(NSRange)effectiveRange
                 toTarget:(NSMutableAttributedString *)target
{
    // Currently do nothing
}

- (void)renderListItemElement:(BPElement *)element
                      inRange:(NSRange)effectiveRange
                     toTarget:(NSMutableAttributedString *)target
{
    NSMutableParagraphStyle *paragraphStyle = [[NSMutableParagraphStyle alloc] init];
    [paragraphStyle setParagraphSpacing:0.f];
    [paragraphStyle setFirstLineHeadIndent:6.f];
    [paragraphStyle setHeadIndent:23.f];

    NSDictionary *attributes = @{NSParagraphStyleAttributeName : paragraphStyle};
    [target addAttributes:attributes range:effectiveRange];
    
    [self insertNewlineIntoTarget:target];
}

- (void)renderHeaderElement:(BPElement *)element
                    inRange:(NSRange)effectiveRange
                    toTarget:(NSMutableAttributedString *)target
{
    NSMutableDictionary *attributes = [NSMutableDictionary dictionary];
    NSMutableParagraphStyle *paragraphStyle = [[NSMutableParagraphStyle alloc] init];
    [paragraphStyle setParagraphSpacing:15.f];
    
    attributes[NSParagraphStyleAttributeName] = paragraphStyle;
    
    CTFontRef font = _defaultFont;
    NSUInteger level = [element[@"level"] integerValue];
    
    // Override font weight and size attributes (but preserve all other attributes)
    
    switch (level) {
        case 1:
            if (_h1Font == NULL) {
                _h1Font = CTFontCreateCopyWithSymbolicTraits(font, CTFontGetSize(font) + 12, NULL, kCTFontBoldTrait, kCTFontBoldTrait);
            }
            
            attributes[NSFontAttributeName] = [self UIFontFromCTFont:_h1Font];
            
            break;
        case 2:
            if (_h2Font == NULL) {
                _h2Font = CTFontCreateCopyWithSymbolicTraits(font, CTFontGetSize(font) + 10, NULL, kCTFontBoldTrait, kCTFontBoldTrait);
            }
            
            attributes[NSFontAttributeName] = [self UIFontFromCTFont:_h2Font];
            
            break;
        case 3:
            if (_h3Font == NULL) {
                _h3Font = CTFontCreateCopyWithSymbolicTraits(font, CTFontGetSize(font) + 8, NULL, kCTFontBoldTrait, kCTFontBoldTrait);
            }
            
            attributes[NSFontAttributeName] = [self UIFontFromCTFont:_h3Font];
            
            break;
        case 4:
            if (_h4Font == NULL) {
                _h4Font = CTFontCreateCopyWithSymbolicTraits(font, CTFontGetSize(font) + 6, NULL, kCTFontBoldTrait, kCTFontBoldTrait);
            }
            
            attributes[NSFontAttributeName] = [self UIFontFromCTFont:_h4Font];
            
            break;
        case 5:
            if (_h5Font == NULL) {
                _h5Font = CTFontCreateCopyWithSymbolicTraits(font, CTFontGetSize(font) + 4, NULL, kCTFontBoldTrait, kCTFontBoldTrait);
            }
            
            attributes[NSFontAttributeName] = [self UIFontFromCTFont:_h5Font];
            
            break;
        case 6:
            if (_h6Font == NULL) {
                _h6Font = CTFontCreateCopyWithSymbolicTraits(font, CTFontGetSize(font) + 2, NULL, kCTFontBoldTrait, kCTFontBoldTrait);
            }
            
            attributes[NSFontAttributeName] = [self UIFontFromCTFont:_h6Font];
            
            break;
        default:
            attributes[NSFontAttributeName] = [self UIFontFromCTFont:_defaultFont];
            break;
    }
    
    [target addAttributes:attributes range:effectiveRange];
}

#pragma mark Span Element Rendering

- (void)renderSpanElement:(BPElement *)element
                 withFont:(CTFontRef)font
                 toTarget:(NSMutableAttributedString *)target
{
    [self renderSpanElement:element
                   withFont:font
                 attributes:[NSMutableDictionary dictionary]
                   toTarget:target];
}

- (void)renderSpanElement:(BPElement *)element
                 withFont:(CTFontRef)font
               attributes:(NSMutableDictionary *)attributes
                 toTarget:(NSMutableAttributedString *)target
{
    attributes[NSFontAttributeName] = [self UIFontFromCTFont:font];
    
    NSString *text;
    
    if ([[element parentElement] elementType] == BPBlockCode) {
        
        // Preserve whitespace within a code block
        
        text = [element text];
    } else {
        text = [[element text] stringByReplacingOccurrencesOfString:@"\n" withString:@" "];
    }
    
    NSAttributedString *attributedText = [[NSAttributedString alloc] initWithString:text attributes:attributes];
    [target appendAttributedString:attributedText];
}

- (void)renderTextElement:(BPElement *)element toTarget:(NSMutableAttributedString *)target
{
    [self renderSpanElement:element withFont:_defaultFont toTarget:target];
}

- (void)renderBoldItalicElement:(BPElement *)element toTarget:(NSMutableAttributedString *)target
{
    if (_boldItalicFont == NULL) {
        _boldItalicFont = CTFontCreateCopyWithSymbolicTraits(_defaultFont, 0.f, NULL, kCTFontBoldTrait | kCTFontItalicTrait, kCTFontBoldTrait | kCTFontItalicTrait);
    }
    
    [self renderSpanElement:element withFont:_boldItalicFont toTarget:target];
}

- (void)renderBoldElement:(BPElement *)element toTarget:(NSMutableAttributedString *)target
{
    if (_boldFont == NULL) {
        _boldFont = CTFontCreateCopyWithSymbolicTraits(_defaultFont, 0.f, NULL, kCTFontBoldTrait, kCTFontBoldTrait);
    }
    
    [self renderSpanElement:element withFont:_boldFont toTarget:target];
}

- (void)renderItalicElement:(BPElement *)element toTarget:(NSMutableAttributedString *)target
{
    if (_italicFont == NULL) {
        _italicFont = CTFontCreateCopyWithSymbolicTraits(_defaultFont, 0.f, NULL, kCTFontItalicTrait, kCTFontItalicTrait);
    }
    
    [self renderSpanElement:element withFont:_italicFont toTarget:target];
}

- (void)renderCodeSpanElement:(BPElement *)element toTarget:(NSMutableAttributedString *)target
{
    if (_monospaceFont == NULL) {
        _monospaceFont = CTFontCreateWithName(CFSTR("Courier"), CTFontGetSize(_defaultFont), NULL);
    }
    
    [self renderSpanElement:element withFont:_monospaceFont toTarget:target];
}

- (void)renderLinkElement:(BPElement *)element toTarget:(NSMutableAttributedString *)target
{
    NSMutableDictionary *attributes = [NSMutableDictionary dictionary];
    attributes[NSUnderlineStyleAttributeName] = @(NSUnderlineStyleSingle);
    attributes[NSForegroundColorAttributeName] = [UIColor blueColor];
    [self renderSpanElement:element withFont:_defaultFont attributes:attributes toTarget:target];
}

- (void)renderLineBreak:(BPElement *)element toTarget:(NSMutableAttributedString *)target
{
    [self insertNewlineIntoTarget:target];
}

#pragma mark Character Insertion

- (void)insertBulletIntoTarget:(NSMutableAttributedString *)target
{
    if (_monospaceFont == NULL) {
        _monospaceFont = CTFontCreateWithName(CFSTR("Courier"), CTFontGetSize(_defaultFont), NULL);
    }
    
    UIFont *bulletFont = [self UIFontFromCTFont:_monospaceFont];
    NSDictionary *attributes = @{NSFontAttributeName : bulletFont};
    [target appendAttributedString:[[NSMutableAttributedString alloc] initWithString:@"• " attributes:attributes]];
}

- (void)insertNewlineIntoTarget:(NSMutableAttributedString *)target
{
    [target appendAttributedString:[[NSMutableAttributedString alloc] initWithString:@"\n"]];
}

@end
