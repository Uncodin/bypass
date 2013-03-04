//
//  BPMarkdownViewController.m
//  BypassSample
//
//  Created by Damian Carrillo on 2/28/13.
//  Copyright (c) 2013 Uncodin. All rights reserved.
//

#import "BPMarkdownViewController.h"
#import <Bypass.h>

@interface BPMarkdownViewController ()
@property (weak, nonatomic) IBOutlet UITextView *markdownView;

@end

@implementation BPMarkdownViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)viewWillAppear:(BOOL)animated
{
    NSString *samplePath = [[NSBundle mainBundle] pathForResource:@"sample" ofType:@"markdown"];
    
    NSError *error = nil;
    NSString *sample = [NSString stringWithContentsOfFile:samplePath encoding:NSUTF8StringEncoding error:&error];
    
    if (error != nil) {
        NSLog(@"Error: %@", error);
    }
    
    BPParser *parser = [[BPParser alloc] init];
    BPDocument *document = [parser parse:sample];
    
    BPAttributedStringRenderer *renderer = [[BPAttributedStringRenderer alloc] init];
    NSAttributedString *attributedText = [renderer renderDocument:document];
    
    [[self markdownView] setAttributedText:attributedText];
}

@end
