//
//  BPMarkdownView.m
//  Bypass
//
//  Created by Damian Carrillo on 3/20/13.
//  Copyright (c) 2013 Uncodin. All rights reserved.
//

#import <CoreText/CoreText.h>
#import "BPAttributedStringConverter.h"
#import "BPMarkdownView.h"
#import "BPMarkdownPageView.h"
#import "BPParser.h"

static const CGFloat kUIStandardMargin = 8.f; // UIKit widgets seem to use 8 pt margins

static CFArrayRef
BPCreatePageFrames(BPDocument *document, CGSize pageSize, CGSize *suggestedContentSizeOut) {
    BPAttributedStringConverter *converter = [[BPAttributedStringConverter alloc] init];
    CFAttributedStringRef attributedText = (__bridge CFAttributedStringRef) [converter convertDocument:document];
    
    CFIndex len = CFAttributedStringGetLength(attributedText);
    CFMutableAttributedStringRef mutableAttributedText = CFAttributedStringCreateMutableCopy(kCFAllocatorDefault, len, attributedText);
    
    CFMutableArrayRef frames = CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks);
    CTFramesetterRef framesetter = CTFramesetterCreateWithAttributedString(mutableAttributedText);
    CFRelease(mutableAttributedText);
    
    CGRect pageRect = CGRectMake(0.f, 0.f, pageSize.width, pageSize.height);
    CGSize constraints = CGSizeMake(CGRectGetWidth(pageRect), CGFLOAT_MAX);
    
    CFRange fitRange;
    CGSize suggestedSize = CTFramesetterSuggestFrameSizeWithConstraints(framesetter, CFRangeMake(0, 0), NULL, constraints, &fitRange);
    *suggestedContentSizeOut = suggestedSize;
    
    pageRect.size.height = MIN(pageSize.height, suggestedSize.height);
    
    CFRange textRange = {0, 0};
    CGFloat y = CGRectGetMinY(pageRect);
    
    while (y < suggestedSize.height) {
        CGPathRef path = CGPathCreateWithRect(pageRect, &CGAffineTransformIdentity);
        CTFrameRef textFrame = CTFramesetterCreateFrame(framesetter, textRange, path, NULL);
        CGPathRelease(path);
        
        CFArrayAppendValue(frames, textFrame);
        
        CFRange visibleRange = CTFrameGetVisibleStringRange(textFrame);
        textRange.location += visibleRange.length;
        
        y += CGRectGetHeight(pageRect);
    }
    
    return frames;
}

@interface BPMarkdownView () <BPMarkdownViewLinkDelegate, BPMarkdownPageViewLinkDelegate>
@end

@implementation BPMarkdownView
{
    BPParser                    *_parser;
    BPAttributedStringConverter *_converter;
    NSMutableArray              *_pageViews;
}

- (id)initWithFrame:(CGRect)frame
{
    return [self initWithFrame:frame markdown:nil];
}

- (id)initWithFrame:(CGRect)frame markdown:(NSString *)markdown
{
    self = [super initWithFrame:frame];
    
    if (self != nil) {
        [self initializeView];
        [self setMarkdown:markdown];
    }
    
    return self;
}

- (id)initWithCoder:(NSCoder *)aDecoder
{
    self = [super initWithCoder:aDecoder];
    
    if (self != nil) {
        [self initializeView];
    }
    
    return self;
}

- (void)initializeView
{
    [self setLinkDelegate:self];
    
    _parser = [[BPParser alloc] init];
    _converter = [[BPAttributedStringConverter alloc] init];
    _pageViews = [[NSMutableArray alloc] init];
    _asynchronous = NO;
    _asynchronousRevealDuration = 0.25;
    
    [self setContentInset:UIEdgeInsetsMake(1.5 * kUIStandardMargin,
                                           kUIStandardMargin,
                                           4 * kUIStandardMargin,
                                           0)];
}

- (void)layoutSubviews
{
    if ([self markdown] != nil && [_pageViews count] == 0) {
        [self renderMarkdown];
    }
    
    [super layoutSubviews];
}

- (void)renderMarkdown
{
    void (^createPageFrames)(void) = ^{
        BPDocument *document = [_parser parse:_markdown];
        
        CGSize pageSize = CGSizeMake(CGRectGetWidth([self frame]) - 2 * kUIStandardMargin, CGRectGetHeight([self frame]));
        CGRect pageRect = CGRectZero;
        pageRect.size = pageSize;
        
        CGSize contentSize;
        CFArrayRef pageFrames = BPCreatePageFrames(document, pageSize, &contentSize);
        
        if ([self isAsynchronous]) {
            dispatch_sync(dispatch_get_main_queue(), ^{
                [self createAndDisplayFrameViewsFromPageFrames:pageFrames pageSize:pageSize contentSize:contentSize];
            });
        } else {
            [self createAndDisplayFrameViewsFromPageFrames:pageFrames pageSize:pageSize contentSize:contentSize];
        }
        
        CFRelease(pageFrames);
    };
    
    if ([self isAsynchronous]) {
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), createPageFrames);
    } else {
        createPageFrames();
    }
}

- (void)createAndDisplayFrameViewsFromPageFrames:(CFArrayRef)pageFrames
                                        pageSize:(CGSize)pageSize
                                     contentSize:(CGSize)contentSize {
    [self setContentSize:contentSize];
    
    CGRect pageRect = CGRectZero;
    pageRect.size = pageSize;
    
    NSUInteger i, count = CFArrayGetCount(pageFrames);
    
    for (i = 0; i < count; i++) {
        CTFrameRef textFrame = CFArrayGetValueAtIndex(pageFrames, i);
        CGRect textViewFrame = CGRectOffset(pageRect, 0.f, i * CGRectGetHeight(pageRect));
        BPMarkdownPageView *textView = [[BPMarkdownPageView alloc] initWithFrame:textViewFrame textFrame:textFrame];
        
        if ([self isAsynchronous]) {
            [textView setAlpha:0.f];
        }
        
        [_pageViews addObject:textView];
        [self addSubview:textView];

        [UIView animateWithDuration:[self asynchronousRevealDuration] animations:^{
            [textView setAlpha:1.f];
        }];
    }
}

#pragma mark BPMarkdownViewLinkDelegate

- (void)markdownView:(BPMarkdownView *)markdownView didHaveLinkTapped:(NSString *)link
{
    // Open URLs by default
    
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:link]];
}

#pragma mark BPMarkdownPageViewLinkDelegate

- (void)markdownPageView:(BPMarkdownPageView *)markdownView didHaveLinkTapped:(NSString *)link
{
    [[self linkDelegate] markdownView:self didHaveLinkTapped:link];
}

@end