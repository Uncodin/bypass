//
//  BPDocumentRenderer.m
//  Bypass
//
//  Created by Damian Carrillo on 3/14/13.
//  Copyright (c) 2013 Uncodin. All rights reserved.
//

#import <CoreText/CoreText.h>
#import <UIKit/UIKit.h>
#import "BPAttributedStringConverter.h"
#import "BPDocumentRenderer.h"
#import "BPMarkdownView.h"

@interface BPDocumentRenderer ()
@property (strong, nonatomic) NSArray *frameViews;
@end

@implementation BPDocumentRenderer
{
    CGSize                _pageSize;
}

- (id)initWithPageSize:(CGSize)pageSize
{
    self = [super init];
    
    if (self != nil) {
        _pageSize = pageSize;
    }
    
    return self;
}

- (NSArray *)renderDocument:(BPDocument *)document suggestedContainerSize:(CGSize *)suggestedContainerSizeOut
{
    BPAttributedStringConverter *converter = [[BPAttributedStringConverter alloc] init];
    CFAttributedStringRef attributedText = (__bridge CFAttributedStringRef) [converter convertDocument:document];
    
    CFIndex len = CFAttributedStringGetLength(attributedText);
    CFMutableAttributedStringRef mutableAttributedText = CFAttributedStringCreateMutableCopy(kCFAllocatorDefault, len, attributedText);
    
    CFMutableArrayRef frames = CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks);
    CTFramesetterRef framesetter = CTFramesetterCreateWithAttributedString(mutableAttributedText);
    CGRect pageRect = CGRectMake(0.f, 0.f, _pageSize.width, _pageSize.height);
    CGSize constraints = CGSizeMake(CGRectGetWidth(pageRect), CGFLOAT_MAX);
    
    CFRange fitRange;
    CGSize suggestedSize = CTFramesetterSuggestFrameSizeWithConstraints(framesetter, CFRangeMake(0, 0), NULL, constraints, &fitRange);
    *suggestedContainerSizeOut = suggestedSize;
    
    CFRange textRange = {0, 0};
    CGFloat y = CGRectGetMinY(pageRect);
    
    while (y < suggestedSize.height) {
        CGPathRef path = CGPathCreateWithRect(pageRect, &CGAffineTransformIdentity);
        CTFrameRef textFrame = CTFramesetterCreateFrame(framesetter, textRange, path, NULL);
        CFArrayAppendValue(frames, textFrame);
        
        CFRange visibleRange = CTFrameGetVisibleStringRange(textFrame);
        textRange.location += visibleRange.length;
        
        y += CGRectGetHeight(pageRect);
    }
    
    NSUInteger i, count = CFArrayGetCount(frames);
    NSMutableArray *frameViews = [NSMutableArray arrayWithCapacity:count];
    
    for (i = 0; i < count; i++) {
        CTFrameRef textFrame = CFArrayGetValueAtIndex(frames, i);
        CGRect textViewFrame = CGRectOffset(pageRect, 0.f, i * CGRectGetHeight(pageRect));
        BPMarkdownView *textView = [[BPMarkdownView alloc] initWithFrame:textViewFrame textFrame:textFrame];
        [frameViews addObject:textView];
    }
    
    return frameViews;
}

@end
