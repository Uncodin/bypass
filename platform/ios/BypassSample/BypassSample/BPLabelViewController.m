//
//  BPLabelViewController.m
//  BypassSample
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

#import <Bypass/Bypass.h>
#import "BPLabelViewController.h"

const CGFloat kTextMargin   = 8.f;
const CGFloat kFooterMargin = 25.f;

@interface BPLabelViewController ()
@property (strong, nonatomic) UIScrollView       *scrollView;
@property (strong, nonatomic) UILabel            *label;
@property (strong, nonatomic) NSAttributedString *attributedText;
@end

@implementation BPLabelViewController

#pragma mark BPLabelViewController

- (void)viewDidLoad
{
    UIScrollView *scrollView = [[UIScrollView alloc] init];
    UIViewAutoresizing autoresizingMask = UIViewAutoresizingFlexibleTopMargin
                                        | UIViewAutoresizingFlexibleTopMargin
                                        | UIViewAutoresizingFlexibleBottomMargin
                                        | UIViewAutoresizingFlexibleLeftMargin;
    [scrollView setAutoresizingMask:autoresizingMask];
    [[self view] addSubview:scrollView];
    [self setScrollView:scrollView];

    UILabel *label = [[UILabel alloc] init];
    [label setAutoresizingMask:autoresizingMask];
    [label setNumberOfLines:0];
    [[self scrollView] addSubview:label];
    [self setLabel:label];
}

- (void)viewWillAppear:(BOOL)animated
{
    [[self scrollView] setFrame:[[self view] bounds]];
    [[self label] setFrame:[[self view] bounds]];
    
    NSString *sample = [self sampleMarkdown];
    
    BPParser *parser = [[BPParser alloc] init];
    BPDocument *document = [parser parse:sample];
    
    BPAttributedStringConverter *renderer = [[BPAttributedStringConverter alloc] init];
    NSAttributedString *attributedText = [renderer convertDocument:document];

    [self setAttributedText:attributedText];

    CGSize boundedSize = CGSizeMake(CGRectGetWidth([[self view] bounds]) - 2 * kTextMargin, CGFLOAT_MAX);
    NSStringDrawingOptions drawingOptions = NSStringDrawingUsesDeviceMetrics
                                          | NSStringDrawingUsesFontLeading
                                          | NSStringDrawingUsesLineFragmentOrigin;
    CGRect boundingRect = [attributedText boundingRectWithSize:boundedSize options:drawingOptions context:nil];
    boundingRect.origin.x = kTextMargin;

    [[self label] setFrame:boundingRect];
    [[self label] setAttributedText:attributedText];
    [[self scrollView] setContentSize:CGSizeMake(boundingRect.size.width,
                                                 boundingRect.size.height + kFooterMargin)];
}

- (void)viewDidAppear:(BOOL)animated
{
    [[self scrollView] flashScrollIndicators];
}

@end
