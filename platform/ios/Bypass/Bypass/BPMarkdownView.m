//
//  BPMarkdownView.m
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

#import "BPAttributedStringConverter.h"
#import "BPDocument.h"
#import "BPMarkdownView.h"

void
BPContextFlipVertical(CGContextRef context, CGRect rect)
{
    CGFloat h = CGRectGetHeight(rect);
    CGAffineTransform flipTransformation = CGAffineTransformMake(1,  0,  // 0
                                                                 0, -1,  // 0
                                                                 0,  h); // 1
    CGContextConcatCTM(context, flipTransformation);
}

@implementation BPMarkdownView
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

@end
