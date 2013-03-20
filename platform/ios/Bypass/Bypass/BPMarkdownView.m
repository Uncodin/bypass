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

static const CGFloat        kUIStandardMargin      = 8.f;
static const NSTimeInterval kReorientationDuration = 0.1;

static CFArrayRef
BPCreatePageFrames(BPDocument *document, CGSize pageSize, CGSize *suggestedContentSizeOut) {
    BPAttributedStringConverter *converter = [[BPAttributedStringConverter alloc] init];
    
    CFAttributedStringRef attributedText;
    attributedText = (__bridge CFAttributedStringRef) [converter convertDocument:document];
    
    CFIndex len = CFAttributedStringGetLength(attributedText);
    CFMutableAttributedStringRef mutableAttributedText;
    mutableAttributedText = CFAttributedStringCreateMutableCopy(kCFAllocatorDefault,
                                                                len,
                                                                attributedText);
    
    CFMutableArrayRef frames = CFArrayCreateMutable(kCFAllocatorDefault,
                                                    0,
                                                    &kCFTypeArrayCallBacks);
    CTFramesetterRef framesetter;
    framesetter = CTFramesetterCreateWithAttributedString(mutableAttributedText);
    CFRelease(mutableAttributedText);
    
    CGRect pageRect = CGRectMake(0.f, 0.f, pageSize.width, pageSize.height);
    CGSize constraints = CGSizeMake(CGRectGetWidth(pageRect), CGFLOAT_MAX);
    
    CFRange fitRange;
    CGSize suggestedSize = CTFramesetterSuggestFrameSizeWithConstraints(framesetter,
                                                                        CFRangeMake(0, 0),
                                                                        NULL,
                                                                        constraints,
                                                                        &fitRange);
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
    BPParser       *_parser;
    BPDocument     *_document;
    NSMutableArray *_pageViews;
    NSArray        *_previousPageViews;
    CGRect         _previousFrame;
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
    _pageViews = [[NSMutableArray alloc] init];
    _asynchronous = YES;
    _asynchronousRevealDuration = 0.25;
    
    [self setContentInset:UIEdgeInsetsMake(1.5 * kUIStandardMargin,
                                           kUIStandardMargin,
                                           4 * kUIStandardMargin,
                                           0)];
    
    [self setAutoresizesSubviews:YES];
    [self setAutoresizingMask: UIViewAutoresizingFlexibleTopMargin
                             | UIViewAutoresizingFlexibleRightMargin
                             | UIViewAutoresizingFlexibleBottomMargin
                             | UIViewAutoresizingFlexibleLeftMargin
                             | UIViewAutoresizingFlexibleWidth
                             | UIViewAutoresizingFlexibleHeight];
}

- (void)layoutSubviews
{
    [super layoutSubviews];
    
    if (([self markdown] != nil && [_pageViews count] == 0)) {
        [self renderMarkdownWithDuration:_asynchronous ? _asynchronousRevealDuration : 0
                              completion:nil];
    } else if ([self viewHasBeenReoriented]) {
        _previousPageViews = [NSArray arrayWithArray:_pageViews];
        
        [self renderMarkdownWithDuration:kReorientationDuration
                              completion:^(BOOL finished) {
                                  for (UIView *pageView in _previousPageViews) {
                                      [pageView removeFromSuperview];
                                  }
                                  
                                  _previousPageViews = nil;
                              }];
    }
}

- (BOOL)viewHasBeenReoriented
{
    return !CGRectEqualToRect([self frame], _previousFrame);
}

- (void)setMarkdown:(NSString *)markdown
{
    _markdown = markdown;
    _document = nil;
}

- (void)renderMarkdownWithDuration:(NSTimeInterval)duration
                        completion:(void (^)(BOOL finished))completion
{
    _previousFrame = [self frame];
    
    void (^createPageFrames)(void) = ^{
        [_pageViews removeAllObjects];

        if (_document == nil) {
            _document = [_parser parse:_markdown];
        }
 
        [_parser parse:_markdown];
        CGSize pageSize = CGSizeMake(CGRectGetWidth([self frame]) - 2 * kUIStandardMargin,
                                     CGRectGetHeight([self frame]));
        CGRect pageRect = CGRectZero;
        pageRect.size = pageSize;
        
        CGSize contentSize;
        CFArrayRef pageFrames = BPCreatePageFrames(_document, pageSize, &contentSize);
        
        if ([self isAsynchronous]) {
            dispatch_sync(dispatch_get_main_queue(), ^{
                [self createAndDisplayViewsFromPageFrames:pageFrames
                                                 pageSize:pageSize
                                              contentSize:contentSize
                                                 duration:duration
                                               completion:completion];
            });
        } else {
            [self createAndDisplayViewsFromPageFrames:pageFrames
                                             pageSize:pageSize
                                          contentSize:contentSize
                                             duration:duration
                                           completion:completion];
        }
        
        CFRelease(pageFrames);
    };
    
    if ([self isAsynchronous]) {
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0),
                       createPageFrames);
    } else {
        createPageFrames();
    }
}

- (void)createAndDisplayViewsFromPageFrames:(CFArrayRef)pageFrames
                                   pageSize:(CGSize)pageSize
                                contentSize:(CGSize)contentSize
                                   duration:(NSTimeInterval)duration
                                 completion:(void (^)(BOOL finished))completion {
    [self setContentSize:contentSize];
    
    CGRect pageRect = CGRectZero;
    pageRect.size = pageSize;
    
    NSUInteger i, count = CFArrayGetCount(pageFrames);
    
    for (i = 0; i < count; i++) {
        CTFrameRef textFrame = CFArrayGetValueAtIndex(pageFrames, i);
        CGRect textViewFrame = CGRectOffset(pageRect, 0.f, i * CGRectGetHeight(pageRect));
        BPMarkdownPageView *textView = [[BPMarkdownPageView alloc] initWithFrame:textViewFrame
                                                                       textFrame:textFrame];
        
        [textView setTag:i + 1];
        
        if ([self isAsynchronous]) {
            [textView setAlpha:0.f];
        }
        
        [_pageViews addObject:textView];
        [self addSubview:textView];

        [UIView animateWithDuration:duration animations:^{
            [textView setAlpha:1.f];
        } completion:completion];
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