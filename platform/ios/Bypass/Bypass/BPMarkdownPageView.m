//
//  BPMarkdownPageView.m
//  Bypass
//
//  Created by Damian Carrillo on 3/13/13.
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
#import "BPDocument.h"
#import "BPMarkdownPageView.h"

#define SHOW_ATTRIBUTES_ON_TAP 0

void
BPContextFlipVertical(CGContextRef context, CGRect rect)
{
    CGFloat h = CGRectGetHeight(rect);
    CGAffineTransform flipTransformation = CGAffineTransformMake(1,  0,  // 0
                                                                 0, -1,  // 0
                                                                 0,  h); // 1
    CGContextConcatCTM(context, flipTransformation);
}

@implementation BPMarkdownPageView
{
    CTFrameRef _textFrame;
}

- (id)initWithFrame:(CGRect)frame textFrame:(CTFrameRef)textFrame
{
    self = [super initWithFrame:frame];
    
    if (self != nil) {
        [self setBackgroundColor:[UIColor whiteColor]];
        
        CFRetain(textFrame);
        _textFrame = textFrame;
    }
    
    return self;
}

- (void)dealloc
{
    CFRelease(_textFrame);
}

- (void)drawRect:(CGRect)rect
{
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextSetFillColorWithColor(context, [[self backgroundColor] CGColor]);
    CGContextFillRect(context, rect);

    CGContextSetStrokeColorWithColor(context, [[UIColor blackColor] CGColor]);    
    CGContextSetTextMatrix(context, CGAffineTransformIdentity);    
    BPContextFlipVertical(context, rect);
    
    CTFrameDraw(_textFrame, context);
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [touches anyObject];
    
    if ([touch tapCount] == 1 && [touch phase] == UITouchPhaseEnded) {
        CGPoint touchPoint = [touch locationInView:self];
        
        // Account for matrix flip
        
        touchPoint = CGPointMake(touchPoint.x, CGRectGetHeight([self frame]) - touchPoint.y);

        CFArrayRef lines = CTFrameGetLines(_textFrame);
        CFIndex lineCount = CFArrayGetCount(lines);
        CGPoint origins[lineCount];
        
        CTFrameGetLineOrigins(_textFrame, CFRangeMake(0, lineCount), origins);
        
        CFIndex i;
        for (i = 0; i < lineCount; i++) {
            CTLineRef line = CFArrayGetValueAtIndex(lines, i);
            CGRect lineBounds = CTLineGetBoundsWithOptions(line, kCTLineBoundsUseOpticalBounds);
            
            lineBounds.origin.x += origins[i].x;
            lineBounds.origin.y += origins[i].y;
            
            if (CGRectContainsPoint(lineBounds, touchPoint)) {
                CFIndex stringIndex = CTLineGetStringIndexForPosition(line, touchPoint);
                
                if (stringIndex > 0 &&
                    touchPoint.x < CTLineGetOffsetForStringIndex(line, stringIndex, NULL)) {
                    
                    // Account for caret snapping when a boundary glyph's outer half has
                    // been tapped
                    
                    --stringIndex;
                }
                
                CFArrayRef glyphRuns = CTLineGetGlyphRuns(line);
                CFIndex j, glyphRunCount = CFArrayGetCount(glyphRuns);
                
                for (j = 0; j < glyphRunCount; j++) {
                    CTRunRef glyphRun = CFArrayGetValueAtIndex(glyphRuns, j);
                    CFRange textRange = CTRunGetStringRange(glyphRun);
                    
                    if (textRange.location <= stringIndex &&
                        stringIndex < textRange.location + textRange.length) {
                        
                        CFDictionaryRef attributes = CTRunGetAttributes(glyphRun);
                        
#if SHOW_ATTRIBUTES_ON_TAP == 1
                        // Shows what attributes are available on the tap point
                        NSLog(@"%@", (__bridge NSDictionary *) attributes);
#endif
                        
                        const void *value;
                        value = CFDictionaryGetValue(attributes,
                                                     (const void *) BPLinkStyleAttributeName);
                        
                        NSString *link = (__bridge NSString *) value;
                        [[self linkDelegate] markdownPageView:self didHaveLinkTapped:link];
                        return;
                    }
                }
                
                break;
            }
        }
    }
    
    [super touchesEnded:touches withEvent:event];
}

@end
