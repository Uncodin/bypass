//
//  BPMarkdownViewController.m
//  BypassSample
//
//  Created by Damian Carrillo on 3/13/13.
//  Copyright (c) 2013 Uncodin. All rights reserved.
//

#import "BPMarkdownViewController.h"

@interface BPMarkdownViewController ()
@property (strong, nonatomic) IBOutlet BPMarkdownView *markdownView;
@end

@implementation BPMarkdownViewController

- (void)viewWillAppear:(BOOL)animated
{
    NSString *sampleMarkdown = [self sampleMarkdown];
    [[self markdownView] setMarkdown:sampleMarkdown];
}

- (void)viewDidAppear:(BOOL)animated
{
    [[self markdownView] flashScrollIndicators];
}

@end
