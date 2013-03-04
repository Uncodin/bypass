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
        [self renderElement:element atLevel:0 toTarget:target];
    }
    
    return target;
}

- (void)renderElement:(BPElement *)element atLevel:(NSUInteger)level toTarget:(NSMutableAttributedString *)target
{
    if ([[element text] length] > 0) {
        BPElementType elementType = [element elementType];
        
        if (elementType == BPBlockCode) {
            
        } else if (elementType == BPBlockQuote) {
            
        } else if (elementType == BPBlockHTML) {
            
        } else if (elementType == BPHeader) {
            
        } else if (elementType == BPHRule) {
            
        } else if (elementType == BPList) {
            
        } else if (elementType == BPListItem) {
            
        } else if (elementType == BPParagraph) {
            
        } else if (elementType == BPTable) {
            
        } else if (elementType == BPTableRow) {
            
        } else if (elementType == BPTableCell) {
            
        } else if (elementType == BPTableRow) {
            
        } else if (elementType == BPAutoLink) {
            
        } else if (elementType == BPCodeSpan) {
            
        } else if (elementType == BPDoubleEmphasis) {
            [self renderBoldElement:element toTarget:target];
        } else if (elementType == BPEmphasis) {
            [self renderItalicElement:element toTarget:target];
        } else if (elementType == BPImage) {
            // Currently not supported
        } else if (elementType == BPLineBreak) {
            
        } else if (elementType == BPLink) {
            
        } else if (elementType == BPRawHTMLTag) {
            // Currently not supported
        } else if (elementType == BPTripleEmphasis) {
            [self renderBoldItalicElement:element toTarget:target];
        } else if (elementType == BPText) {
            [self renderTextElement:element toTarget:target];
        }
    }
    
    for (BPElement *childElement in [element childElements]) {
        [self renderElement:childElement atLevel:(level + 1) toTarget:target];
    }
    
    if ([element isBlockElement]) {
        [target appendAttributedString:[[NSAttributedString alloc] initWithString:@"\n"]];
    }
}

- (void)renderSpanElement:(BPElement *)element withAttributes:(NSDictionary *)attributes toTarget:(NSMutableAttributedString *)target
{
    NSAttributedString *text = [[NSAttributedString alloc] initWithString:[element text] attributes:attributes];
    [target appendAttributedString:text];
}

- (void)renderTextElement:(BPElement *)element toTarget:(NSMutableAttributedString *)target
{
    [self renderSpanElement:element
             withAttributes:@{NSFontAttributeName : [UIFont fontWithName:@"Helvetica" size:15.f]}
                   toTarget:target];
}

- (void)renderBoldItalicElement:(BPElement *)element toTarget:(NSMutableAttributedString *)target
{
    [self renderSpanElement:element
             withAttributes:@{NSFontAttributeName : [UIFont fontWithName:@"Helvetica-BoldOblique" size:15.f]}
                   toTarget:target];
}

- (void)renderBoldElement:(BPElement *)element toTarget:(NSMutableAttributedString *)target
{
    [self renderSpanElement:element
             withAttributes:@{NSFontAttributeName : [UIFont fontWithName:@"Helvetica-Bold" size:15.f]}
                   toTarget:target];
}

- (void)renderItalicElement:(BPElement *)element toTarget:(NSMutableAttributedString *)target
{
    [self renderSpanElement:element
             withAttributes:@{NSFontAttributeName : [UIFont fontWithName:@"Helvetica-Oblique" size:15.f]}
                   toTarget:target];
}

- (void)renderCodeSpan:(BPElement *)element toTarget:(NSMutableAttributedString *)target
{
    [self renderSpanElement:element
             withAttributes:@{NSFontAttributeName : [UIFont fontWithName:@"Courier" size:14.f]}
                   toTarget:target];
}

@end
