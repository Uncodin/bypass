//
//  BPMarkdownView.m
//  Bypass
//
//  Created by Damian Carrillo on 3/20/13.
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
#import "BPAttributedStringConverter.h"
#import "BPMarkdownView.h"
#import "BPMarkdownPageView.h"
#import "BPParser.h"
#import "BPDisplaySettings.h"

/*
 * The standard margin of the UIKit views. This value was based on human inspection.
 */
static const CGFloat kUIStandardMargin = 8.f;

/*
 * The duration of the view reorientation animation.
 */
static const NSTimeInterval kReorientationDuration = 0.3;

/*
 * Creates an array of CTFrameRefs (frames) from the given document. The frames are created 
 * at the given page size. This function will transfer the suggested content size back to the 
 * caller through `suggestedContentSizeOut`. This is the size that the framesetter recommends
 * the container should have, and is based on the length of the body of attribited markdown.
 *
 * Note that it is possible to supply a `pageSize` with an unbounded height by suppling a size
 * whose height is CGFLOAT_MAX. This will size a single view to fit around the attributed text,
 * but it is not recommended for long strings because the entire view will be rendered in the
 * `drawRect` method.
 * 
 * \param document a document tree
 * \pageSize the size for a given page frame
 *
 */
static CFArrayRef
BPCreatePageFrames(CGSize pageSize, CGSize *suggestedContentSizeOut, CFAttributedStringRef attributedText)
{
    CFMutableArrayRef frames = CFArrayCreateMutable(kCFAllocatorDefault,
                                                    0,
                                                    &kCFTypeArrayCallBacks);
    CTFramesetterRef framesetter;
    framesetter = CTFramesetterCreateWithAttributedString(attributedText);

    CGRect pageRect = CGRectMake(0.f, 0.f, pageSize.width, pageSize.height);
    CGSize constraints = CGSizeMake(CGRectGetWidth(pageRect), CGFLOAT_MAX);
    
    CFRange fitRange;
    CGSize suggestedSize = CTFramesetterSuggestFrameSizeWithConstraints(framesetter,
                                                                        CFRangeMake(0, 0),
                                                                        NULL,
                                                                        constraints,
                                                                        &fitRange);
    *suggestedContentSizeOut = suggestedSize; // TODO:ContentSize is too small
    
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
    CFRelease(framesetter);
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
    NSAttributedString *_attributedText;
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
    _asynchronous = NO;
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
        [_pageViews removeAllObjects];
        [self renderMarkdownWithDuration:kReorientationDuration
                              completion:^(BOOL finished) {
                                  for (UIView *pageView in _previousPageViews) {
                                      [pageView removeFromSuperview];
                                  }
                                  
                                  _previousPageViews = nil;
                              }];
    }
}

/*
 * Occurs during a device rotation.
 */
- (BOOL)viewHasBeenReoriented
{
    return !CGRectEqualToRect([self frame], _previousFrame);
}

- (void)setMarkdown:(NSString *)markdown
{
    _markdown = markdown;
    _document = nil;
    for (BPMarkdownPageView *view in _pageViews) {
      [view removeFromSuperview];
    }
    [_pageViews removeAllObjects];
}

/*
 * Renders markdown text into corresponding CTFrames and arranges for them to be  displayed.
 */
- (void)renderMarkdownWithDuration:(NSTimeInterval)duration
                        completion:(void (^)(BOOL finished))completion
{
    /*
     In order to be flexible in the way that the text is rendered, this method's implementation
     is a bit wonky. The intent is to support views that render immediately and also to support
     a two step process where the text is rendered into Core Text frames in the background and 
     then those frames fill views that are inserted.
     
            *** Please ensure that you thoroughly understand what is going ***
            ***            on before attempting to change this.            ***
     
     */
    
    _previousFrame = [self frame];

    void (^createPageFrames)(void) = ^{
        if (_document == nil) {
            _document = [_parser parse:_markdown];
            BPAttributedStringConverter *converter = [[BPAttributedStringConverter alloc] init];
            
            // Push display settings into the converter
            
            if (_displaySettings != nil) {
                converter.displaySettings = _displaySettings;
            }
            
           _attributedText = [converter convertDocument:_document];
        }

        CGSize pageSize = CGSizeMake(CGRectGetWidth([self frame]) - (self.contentInset.left + self.contentInset.right),
                                     CGRectGetHeight([self frame]));

        CGSize contentSize;
        CFArrayRef pageFrames = BPCreatePageFrames(pageSize, &contentSize, (__bridge CFAttributedStringRef) _attributedText);
        contentSize.width = MIN(contentSize.width, pageSize.width);

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

/*
 * Following from `renderMarkdownWithDuration:completion:`, this method will display the 
 * populate a view with a frame that it is to display and add it as a subview.
 *
 * There are three cases that this must support:
 * 
 *   - Rendering text immediately when this view loads for the first time
 *   - Rendering text that fades in when this view loads text asynchronously
 *   - Rendering text after a device reorientation which has an animation, but the duration
 *     is not configurable.
 */
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
        CGRect textViewFrame;
        
        if (contentSize.height < CGRectGetHeight([self bounds])) {
            textViewFrame = CGRectMake(0.f,
                                       0.f,
                                       CGRectGetWidth(pageRect),
                                       contentSize.height);
        } else {
            textViewFrame = CGRectOffset(pageRect, 0.f, i * CGRectGetHeight(pageRect));
        }
        
        BPMarkdownPageView *textView = [[BPMarkdownPageView alloc] initWithFrame:textViewFrame
                                                                       textFrame:textFrame];

        CFRelease(textFrame); // the textView took ownership, and the retain would be 2 at this point

        [textView setTag:i + 1];
        [textView setAlpha:0.f];
        
        [_pageViews addObject:textView];
        [self addSubview:textView];
        
        [textView setLinkDelegate:self];
    }
    
    // Schedule the fade in and fade out animations to occur at the same time
    
    [UIView animateWithDuration:duration animations:^{
        for (BPMarkdownPageView *pageView in _pageViews) {
            [pageView setAlpha:1.f];
        }
        
        for (BPMarkdownPageView *previousPageView in _previousPageViews) {
            [previousPageView setAlpha:0.f];
        }
    } completion:completion];
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
