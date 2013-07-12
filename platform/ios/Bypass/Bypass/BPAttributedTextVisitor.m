//
//  BPAttributedTextVisitor.m
//  Bypass
//
//  Created by Damian Carrillo on 3/22/13.
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

#import "BPAttributedTextVisitor.h"

NSString *const BPLinkStyleAttributeName = @"NSLinkAttributeName";

static const CGFloat kBulletIndentation     = 13.0f;
static const CGFloat kCodeIndentation       = 10.0f;
static const CGFloat kQuoteIndentation      = 23.0f;
static const CGFloat kLineSpacingSmall      =  1.2f;
static const CGFloat kParagraphSpacingLarge = 20.0f;
static const CGFloat kParagraphSpacingSmall = 10.0f;
static const CGFloat kParagraphSpacingNone  =  0.0f;

@implementation BPAttributedTextVisitor
{
    CTFontRef _defaultFont;
    CTFontRef _boldFont;
    CTFontRef _italicFont;
    CTFontRef _boldItalicFont;
    CTFontRef _monospaceFont;
    CTFontRef _quoteFont;
    CTFontRef _h1Font;
    CTFontRef _h2Font;
    CTFontRef _h3Font;
    CTFontRef _h4Font;
    CTFontRef _h5Font;
    CTFontRef _h6Font;
}

- (id)init
{
    self = [super init];
    
    if (self != nil) {
        _attributedText = [[NSMutableAttributedString alloc] init];
    }
    
    return self;
}

- (void)elementWalker:(BPElementWalker *)elementWalker
     willVisitElement:(BPElement *)element
        withTextRange:(NSRange)textRange
{
    // do nothing
}

- (int)elementWalker:(BPElementWalker *)elementWalker
     didVisitElement:(BPElement *)element
       withTextRange:(NSRange)textRange
{
    return [self convertElement:element toTarget:_attributedText range:textRange];
}

- (void)dealloc
{
    if (_defaultFont != NULL)    CFRelease(_defaultFont);
    if (_boldFont != NULL)       CFRelease(_boldFont);
    if (_italicFont != NULL)     CFRelease(_italicFont);
    if (_boldItalicFont != NULL) CFRelease(_boldItalicFont);
    if (_monospaceFont != NULL)  CFRelease(_monospaceFont);
    if (_quoteFont != NULL)      CFRelease(_quoteFont);
    if (_h1Font != NULL)         CFRelease(_h1Font);
    if (_h2Font != NULL)         CFRelease(_h2Font);
    if (_h3Font != NULL)         CFRelease(_h3Font);
    if (_h4Font != NULL)         CFRelease(_h4Font);
    if (_h5Font != NULL)         CFRelease(_h5Font);
    if (_h6Font != NULL)         CFRelease(_h6Font);
}

#pragma mark Fonts

- (UIFont *)UIFontFromCTFont:(CTFontRef)ctFont
{
    NSString *fontName;
    fontName = (__bridge_transfer NSString *) CTFontCopyName(ctFont, kCTFontPostScriptNameKey);
    
    CGFloat fontSize = CTFontGetSize(ctFont);
    UIFont *font = [UIFont fontWithName:fontName size:fontSize];
    return font;
}

- (CTFontRef)defaultFont
{
    if (_defaultFont == NULL) {
        CGFloat systemFontSize = [UIFont systemFontSize];
        UIFont *systemFont = [UIFont systemFontOfSize:systemFontSize];
        CFStringRef systemFontName = (__bridge CFStringRef) [systemFont fontName];
        
        _defaultFont = CTFontCreateWithName(systemFontName, systemFontSize, NULL);
    }
    
    return _defaultFont;
}

- (CTFontRef)boldFont
{
    if (_boldFont == NULL) {
        _boldFont = CTFontCreateCopyWithSymbolicTraits([self defaultFont],
                                                       0.f,
                                                       NULL,
                                                       kCTFontBoldTrait,
                                                       kCTFontBoldTrait);
    }
    
    return _boldFont;
}

- (CTFontRef)italicFont
{
    if (_italicFont == NULL) {
        _italicFont = CTFontCreateCopyWithSymbolicTraits([self defaultFont],
                                                         0.f,
                                                         NULL,
                                                         kCTFontItalicTrait,
                                                         kCTFontItalicTrait);
    }
    
    return _italicFont;
}

- (CTFontRef)boldItalicFont
{
    if (_boldItalicFont == NULL) {
        CTFontSymbolicTraits traits = kCTFontBoldTrait | kCTFontItalicTrait;
        CTFontSymbolicTraits mask = kCTFontBoldTrait | kCTFontItalicTrait;
        _boldItalicFont = CTFontCreateCopyWithSymbolicTraits([self defaultFont],
                                                             0.f,
                                                             NULL,
                                                             traits,
                                                             mask);
    }
    
    return _boldItalicFont;
}

- (CTFontRef)monospaceFont
{
    if (_monospaceFont == NULL) {
        _monospaceFont = CTFontCreateWithName(CFSTR("Courier"),
                                              CTFontGetSize([self defaultFont]) - 2, NULL);
    }
    
    return _monospaceFont;
}

- (CTFontRef)quoteFont
{
    if (_quoteFont == NULL) {
        _quoteFont = CTFontCreateWithName(CFSTR("Marion-Italic"),
                                          CTFontGetSize([self defaultFont]) + 2, NULL);
    }
    
    return _quoteFont;
}

- (CTFontRef)h1Font
{
    if (_h1Font == NULL) {
        _h1Font = CTFontCreateWithName(CFSTR("HelveticaNeue-CondensedBold"),
                                       CTFontGetSize([self defaultFont]) * 2, NULL);
    }
    
    return _h1Font;
}

- (CTFontRef)h2Font
{
    if (_h2Font == NULL) {
        _h2Font = CTFontCreateWithName(CFSTR("HelveticaNeue-CondensedBold"),
                                       CTFontGetSize([self defaultFont]) * 1.8, NULL);
    }
    
    return _h2Font;
}

- (CTFontRef)h3Font
{
    if (_h3Font == NULL) {
        _h3Font = CTFontCreateWithName(CFSTR("HelveticaNeue-CondensedBold"),
                                       CTFontGetSize([self defaultFont]) * 1.6, NULL);
    }
    
    return _h3Font;
}

- (CTFontRef)h4Font
{
    if (_h4Font == NULL) {
        _h4Font = CTFontCreateWithName(CFSTR("HelveticaNeue-CondensedBold"),
                                       CTFontGetSize([self defaultFont]) * 1.4, NULL);
    }
    
    return _h4Font;
}

- (CTFontRef)h5Font
{
    if (_h5Font == NULL) {
        _h5Font = CTFontCreateWithName(CFSTR("HelveticaNeue-CondensedBold"),
                                       CTFontGetSize([self defaultFont]) * 1.2, NULL);
    }
    
    return _h5Font;
}

- (CTFontRef)h6Font
{
    if (_h6Font == NULL) {
        _h6Font = CTFontCreateWithName(CFSTR("HelveticaNeue-CondensedBold"),
                                       CTFontGetSize([self defaultFont]) * 1, NULL);
    }
    
    return _h6Font;
}

#pragma mark Rendering

- (int)convertElement:(BPElement *)element
             toTarget:(NSMutableAttributedString *)target
                range:(NSRange)effectiveRange
{
    int insertedCharacters = 0;
    
    BPElementType elementType = [element elementType];
    
    // Render span elements and insert special characters for block elements
    if (elementType == BPList) {
        if ([[element parentElement] elementType] == BPListItem) {
            insertedCharacters += [self insertNewlineIntoTarget:target atIndex:effectiveRange.location];
        }
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
    } else if (elementType == BPParagraph) {
        [self renderParagraphElement:element inRange:effectiveRange toTarget:target];
    } else if (elementType == BPHeader) {
        [self renderHeaderElement:element inRange:effectiveRange toTarget:target];
    } else if (elementType == BPListItem) {
        insertedCharacters += [self renderListItemElement:element inRange:effectiveRange toTarget:target];
    } else if (elementType == BPBlockCode) {
        insertedCharacters += [self renderBlockCodeElement:element inRange:effectiveRange toTarget:target];
    } else if (elementType == BPBlockQuote) {
        [self renderBlockQuoteElement:element inRange:effectiveRange toTarget:target];
    }
    
    if ([element isBlockElement]
        && ![[element parentElement] isBlockElement]
        && ![[[element parentElement] parentElement] isBlockElement]) {
        insertedCharacters += [self appendNewlineOntoTarget:target];
    }
    
    return insertedCharacters;
}

#pragma mark Character Insertion

- (int)insertNewlineIntoTarget:(NSMutableAttributedString*) target
                       atIndex:(int) index
{
    [target insertAttributedString:[[NSMutableAttributedString alloc] initWithString:@"\n"] atIndex:index];
    
    return 1;
}

- (int)appendNewlineOntoTarget:(NSMutableAttributedString *)target
{
    [target appendAttributedString:[[NSMutableAttributedString alloc] initWithString:@"\n"]];
    
    return 1;
}

- (int)insertBulletIntoTarget:(NSMutableAttributedString*) target
                        color:(UIColor*) bulletColor
                      atIndex:(int)index
{
    NSDictionary *bulletAttributes = @{
                                       NSFontAttributeName            : [self UIFontFromCTFont:[self monospaceFont]],
                                       NSForegroundColorAttributeName : bulletColor
                                       };
    
    NSAttributedString *attributedBullet;
    attributedBullet = [[NSAttributedString alloc] initWithString:@"• "
                                                       attributes:bulletAttributes];
    
    [target insertAttributedString:attributedBullet atIndex:index];
    
    return 2;
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
    
    NSAttributedString *attributedText = [[NSAttributedString alloc] initWithString:text
                                                                         attributes:attributes];
    [target appendAttributedString:attributedText];
}

- (void)renderTextElement:(BPElement *)element toTarget:(NSMutableAttributedString *)target
{
    [self renderSpanElement:element withFont:[self defaultFont] toTarget:target];
}

- (void)renderBoldItalicElement:(BPElement *)element
                       toTarget:(NSMutableAttributedString *)target
{
    [self renderSpanElement:element withFont:[self boldItalicFont] toTarget:target];
}

- (void)renderBoldElement:(BPElement *)element
                 toTarget:(NSMutableAttributedString *)target
{
    [self renderSpanElement:element withFont:[self boldFont] toTarget:target];
}

- (void)renderItalicElement:(BPElement *)element
                   toTarget:(NSMutableAttributedString *)target
{
    [self renderSpanElement:element withFont:[self italicFont] toTarget:target];
}

- (void)renderCodeSpanElement:(BPElement *)element
                     toTarget:(NSMutableAttributedString *)target
{
    [self renderSpanElement:element withFont:[self monospaceFont] toTarget:target];
}

- (void)renderLinkElement:(BPElement *)element
                 toTarget:(NSMutableAttributedString *)target
{
    NSMutableDictionary *attributes = [NSMutableDictionary dictionary];
    attributes[NSUnderlineStyleAttributeName] = @(NSUnderlineStyleSingle);
    attributes[NSForegroundColorAttributeName] = [UIColor blueColor];
    attributes[BPLinkStyleAttributeName] = element[@"link"];
    [self renderSpanElement:element
                   withFont:_defaultFont
                 attributes:attributes toTarget:target];
}

- (void)renderLineBreak:(BPElement *)element
               toTarget:(NSMutableAttributedString *)target
{
    [self appendNewlineOntoTarget:target];
}

#pragma mark Block Element Rendering

- (void)renderBlockQuoteElement:(BPElement *)element
                        inRange:(NSRange)effectiveRange
                       toTarget:(NSMutableAttributedString *)target
{
    NSMutableDictionary *attributes = [NSMutableDictionary dictionary];
    attributes[NSFontAttributeName] = [self UIFontFromCTFont:[self quoteFont]];
    
    NSMutableParagraphStyle *paragraphStyle = [[NSMutableParagraphStyle alloc] init];
    [paragraphStyle setParagraphSpacing:kParagraphSpacingSmall];
    [paragraphStyle setFirstLineHeadIndent:kQuoteIndentation];
    [paragraphStyle setHeadIndent:kQuoteIndentation];
    [paragraphStyle setTailIndent:-kQuoteIndentation];
    attributes[NSParagraphStyleAttributeName] = paragraphStyle;
    
    [target addAttributes:attributes range:effectiveRange];
}

- (int)renderBlockCodeElement:(BPElement *)element
                      inRange:(NSRange)effectiveRange
                     toTarget:(NSMutableAttributedString *)target
{
    int insertedCharacters = 0;
    
    NSMutableDictionary *attributes = [NSMutableDictionary dictionary];
    attributes[NSFontAttributeName] = [self UIFontFromCTFont:[self monospaceFont]];
    attributes[NSForegroundColorAttributeName] = [UIColor grayColor];
    
    NSMutableParagraphStyle *paragraphStyle = [[NSMutableParagraphStyle alloc] init];
    [paragraphStyle setParagraphSpacing:kParagraphSpacingNone];
    [paragraphStyle setFirstLineHeadIndent:kCodeIndentation];
    [paragraphStyle setHeadIndent:kCodeIndentation];
    [paragraphStyle setTailIndent:-kCodeIndentation];
    attributes[NSParagraphStyleAttributeName] = paragraphStyle;
    
    [target addAttributes:attributes range:effectiveRange];
    
    insertedCharacters += [self appendNewlineOntoTarget:target];
    
    return insertedCharacters;
}

- (void)renderParagraphElement:(BPElement *)element
                       inRange:(NSRange)effectiveRange
                      toTarget:(NSMutableAttributedString *)target
{
    NSMutableParagraphStyle *paragraphStyle = [[NSMutableParagraphStyle alloc] init];
    [paragraphStyle setParagraphSpacing:kParagraphSpacingLarge];
    [paragraphStyle setLineSpacing:1.1f];
    
    NSDictionary *attributes = @{NSParagraphStyleAttributeName : paragraphStyle};
    [target addAttributes:attributes range:effectiveRange];
}

- (int)renderListItemElement:(BPElement *)element
                     inRange:(NSRange)effectiveRange
                    toTarget:(NSMutableAttributedString *)target
{
    int insertedCharacters = 0;
    
    NSUInteger level = 0;
    BPElement *inspectedElement = [[element parentElement] parentElement];
    NSMutableString *indentation = [NSMutableString  string];
    
    while ([inspectedElement elementType] == BPList
           || [inspectedElement elementType] == BPListItem) {
        if ([inspectedElement elementType] == BPList) {
            [indentation appendString:@"\t"];
            ++level;
        }
        
        inspectedElement = [inspectedElement parentElement];
    }
    
    UIColor *bulletColor;
    
    switch (level % 3) {
        case 1:
            bulletColor = [UIColor grayColor];
            break;
        case 2:
            bulletColor = [UIColor lightGrayColor];
            break;
        default:
            bulletColor = [UIColor blackColor];
            break;
    }
    
    insertedCharacters += [self insertBulletIntoTarget:target color:bulletColor atIndex:effectiveRange.location];
    
    NSMutableParagraphStyle *paragraphStyle = [[NSMutableParagraphStyle alloc] init];
    [paragraphStyle setLineSpacing:kLineSpacingSmall];
    
    NSDictionary *indentationAttributes = @{
                                            NSFontAttributeName : [UIFont systemFontOfSize:kBulletIndentation],
                                            NSParagraphStyleAttributeName : paragraphStyle
                                            };
    
    NSAttributedString *attributedIndentation;
    attributedIndentation = [[NSAttributedString alloc] initWithString:indentation
                                                            attributes:indentationAttributes];
    [target insertAttributedString:attributedIndentation atIndex:effectiveRange.location];
    insertedCharacters += [attributedIndentation length];
    
    if (([[[element parentElement] parentElement] elementType] != BPListItem)
        || (element != [[[element parentElement] childElements] lastObject])) {
        insertedCharacters += [self appendNewlineOntoTarget:target];
    }
    
    return insertedCharacters;
}

- (void)renderHeaderElement:(BPElement *)element
                    inRange:(NSRange)effectiveRange
                   toTarget:(NSMutableAttributedString *)target
{
    NSMutableDictionary *attributes = [NSMutableDictionary dictionary];
    NSMutableParagraphStyle *paragraphStyle = [[NSMutableParagraphStyle alloc] init];
    [paragraphStyle setParagraphSpacing:kParagraphSpacingSmall];
    
    attributes[NSParagraphStyleAttributeName] = paragraphStyle;
    
    // Override font weight and size attributes (but preserve all other attributes)
    
    switch ([element[@"level"] integerValue]) {
        case 1:
            attributes[NSFontAttributeName] = [self UIFontFromCTFont:[self h1Font]];
            break;
        case 2:
            attributes[NSFontAttributeName] = [self UIFontFromCTFont:[self h2Font]];
            break;
        case 3:
            attributes[NSFontAttributeName] = [self UIFontFromCTFont:[self h3Font]];
            break;
        case 4:
            attributes[NSFontAttributeName] = [self UIFontFromCTFont:[self h4Font]];
            break;
        case 5:
            attributes[NSFontAttributeName] = [self UIFontFromCTFont:[self h5Font]];
            break;
        case 6:
            attributes[NSFontAttributeName] = [self UIFontFromCTFont:[self h6Font]];
            break;
        default:
            attributes[NSFontAttributeName] = [self UIFontFromCTFont:[self defaultFont]];
            break;
    }
    
    [target addAttributes:attributes range:effectiveRange];
}


@end
