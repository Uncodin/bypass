//
//  BPAttributedStringRenderer.m
//  Bypass
//
//  Created by Damian Carrillo on 3/1/13.
//  Copyright (c) 2013 Uncodin. All rights reserved.
//

#import "BPAttributedStringRenderer.h"

@implementation BPAttributedStringRenderer

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
    if ([[element text] length] > 0) {
        BPElementType elementType = [element elementType];
        
        if (elementType == BPAutoLink) {
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
    }
    
    for (BPElement *childElement in [element childElements]) {
        [self renderElement:childElement toTarget:target];
    }
    
    if ([element isBlockElement]) {
        [target appendAttributedString:[[NSAttributedString alloc] initWithString:@"\n"]];
    }
}

- (void)renderSpanElement:(BPElement *)element
           withAttributes:(NSMutableDictionary *)attributes
                 toTarget:(NSMutableAttributedString *)target
{
    // Override some attributes based on the parent header element
    
    if ([element parentElement] != nil) {
        if ([[element parentElement] elementType] == BPHeader) {
            if ([target length] > 0) {
                [target appendAttributedString:[[NSAttributedString alloc] initWithString:@"\n"]];
            }
            
            NSUInteger level = [[[element parentElement] attributes][@"level"] integerValue];
            NSString *fontName = [attributes[NSFontAttributeName] fontName];
            
            if ([fontName hasSuffix:@"-Italic"]) {
                fontName = [fontName stringByReplacingOccurrencesOfString:@"-Italic" withString:@"-BoldItalic"];
            } else if ([fontName hasSuffix:@"-Oblique"]) {
                fontName = [fontName stringByReplacingOccurrencesOfString:@"-Italic" withString:@"-BoldOblique"];
            } else if ([fontName hasSuffix:@"-ItalicMT"]) {
                fontName = [fontName stringByReplacingOccurrencesOfString:@"-ItalicMT" withString:@"-BoldItalicMT"];
            } else if (![fontName hasSuffix:@"-Bold"]) {
                fontName = [fontName stringByAppendingString:@"-Bold"];
            }
            
            switch (level) {
                case 1:
                    attributes[NSFontAttributeName] = [UIFont fontWithName:fontName size:27.f];
                    break;
                case 2:
                    attributes[NSFontAttributeName] = [UIFont fontWithName:fontName size:25.f];
                    break;
                case 3:
                    attributes[NSFontAttributeName] = [UIFont fontWithName:fontName size:23.f];
                    break;
                case 4:
                    attributes[NSFontAttributeName] = [UIFont fontWithName:fontName size:21.f];
                    break;
                case 5:
                    attributes[NSFontAttributeName] = [UIFont fontWithName:fontName size:19.f];
                    break;
                case 6:
                    attributes[NSFontAttributeName] = [UIFont fontWithName:fontName size:17.f];
                    break;
                default:
                    attributes[NSFontAttributeName] = [UIFont fontWithName:fontName size:15.f];
                    break;
            }
        } else if ([[element parentElement] elementType] == BPHeader) {
            attributes[NSFontAttributeName] = [UIFont fontWithName:@"Courier" size:16.f];
        } else if ([[element parentElement] elementType] == BPListItem) {
            if ([[element parentElement] childElements][0] == element) {
                if ([[[element parentElement] parentElement] childElements][0] == [element parentElement]) {
                    [target appendAttributedString:[[NSAttributedString alloc] initWithString:@"\n"]];
                }
                
                [target appendAttributedString:[[NSAttributedString alloc] initWithString:@"• "]];
            }
        }
    }
    
    // Rewrite newlines
    
    NSString *text = [[element text] stringByReplacingOccurrencesOfString:@"\n" withString:@" "];
    
    // Append an attributed text segment (glyph run)
    
    NSAttributedString *attributedText = [[NSAttributedString alloc] initWithString:text attributes:attributes];
    [target appendAttributedString:attributedText];
    
    if ([element parentElement] != nil && [[element parentElement] elementType] == BPHeader) {
        [target appendAttributedString:[[NSAttributedString alloc] initWithString:@"\n"]];
    }
}

- (void)renderTextElement:(BPElement *)element toTarget:(NSMutableAttributedString *)target
{
    NSMutableDictionary *attributes = [NSMutableDictionary dictionary];
    attributes[NSFontAttributeName] = [UIFont fontWithName:@"Helvetica" size:15.f];
    [self renderSpanElement:element withAttributes:attributes toTarget:target];
}

- (void)renderBoldItalicElement:(BPElement *)element toTarget:(NSMutableAttributedString *)target
{
    NSMutableDictionary *attributes = [NSMutableDictionary dictionary];
    attributes[NSFontAttributeName] = [UIFont fontWithName:@"Helvetica-BoldOblique" size:15.f];
    [self renderSpanElement:element withAttributes:attributes toTarget:target];
}

- (void)renderBoldElement:(BPElement *)element toTarget:(NSMutableAttributedString *)target
{
    NSMutableDictionary *attributes = [NSMutableDictionary dictionary];
    attributes[NSFontAttributeName] = [UIFont fontWithName:@"Helvetica-Bold" size:15.f];
    [self renderSpanElement:element withAttributes:attributes toTarget:target];
}

- (void)renderItalicElement:(BPElement *)element toTarget:(NSMutableAttributedString *)target
{
    NSMutableDictionary *attributes = [NSMutableDictionary dictionary];
    attributes[NSFontAttributeName] = [UIFont fontWithName:@"Helvetica-Oblique" size:15.f];
    [self renderSpanElement:element withAttributes:attributes toTarget:target];
}

- (void)renderCodeSpanElement:(BPElement *)element toTarget:(NSMutableAttributedString *)target
{
    NSMutableDictionary *attributes = [NSMutableDictionary dictionary];
    attributes[NSFontAttributeName] = [UIFont fontWithName:@"Courier" size:16.f];
    [self renderSpanElement:element withAttributes:attributes toTarget:target];
}

- (void)renderLinkElement:(BPElement *)element toTarget:(NSMutableAttributedString *)target
{
    NSMutableDictionary *attributes = [NSMutableDictionary dictionary];
    attributes[NSUnderlineStyleAttributeName] = @(1);
    attributes[NSForegroundColorAttributeName] = [UIColor blueColor];
    attributes[NSFontAttributeName] = [UIFont fontWithName:@"Helvetica" size:15.f];
    [self renderSpanElement:element withAttributes:attributes toTarget:target];
}

- (void)renderLineBreak:(BPElement *)element toTarget:(NSMutableAttributedString *)target
{
    NSAttributedString *newline = [[NSAttributedString alloc] initWithString:@"\n"];
    [target appendAttributedString:newline];
}

@end
