//
//  BPMarkdownViewController.m
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

#import "BPMarkdownViewController.h"
#import <Bypass.h>

@interface BPMarkdownViewController ()
@property (weak, nonatomic) IBOutlet UITextView         *markdownView;
@property (copy, nonatomic)          NSAttributedString *attributedText;
@end

@implementation BPMarkdownViewController

- (IBAction)textViewWasTapped:(id)sender
{
    CGPoint position = [sender locationInView:[self markdownView]];
    position.y += [[self markdownView] contentOffset].y;

    UITextPosition *tapApproximation = [[self markdownView] closestPositionToPoint:position];
    UITextPosition *beginning = [[self markdownView] beginningOfDocument];
    NSUInteger characterIndex = [[self markdownView] offsetFromPosition:beginning toPosition:tapApproximation];
    
    NSRange effectiveRange;
    effectiveRange.location = characterIndex;
    effectiveRange.length = 0;
    
    NSDictionary *attributes = [[self  attributedText] attributesAtIndex:characterIndex effectiveRange:&effectiveRange];
    NSString *linkHREF = attributes[BPLinkStyleAttributeName];
    
    if (linkHREF != nil) {
        NSURL *linkURL = [NSURL URLWithString:linkHREF];
        [[UIApplication sharedApplication] openURL:linkURL];
    }
}

- (void)viewWillAppear:(BOOL)animated
{
    NSString *samplePath = [[NSBundle mainBundle] pathForResource:@"sample" ofType:@"markdown"];
    
    NSError *error = nil;
    NSString *sample = [NSString stringWithContentsOfFile:samplePath encoding:NSUTF8StringEncoding error:&error];
    
    if (error != nil) {
        NSLog(@"Error: %@", error);
        return;
    }
    
    BPParser *parser = [[BPParser alloc] init];
    BPDocument *document = [parser parse:sample];
    
    BPAttributedStringRenderer *renderer = [[BPAttributedStringRenderer alloc] init];
    NSAttributedString *attributedText = [renderer renderDocument:document];
    
    // Warning: The attributed text is being set on a simple UITextView out of convenience. After this has been done,
    //          Bypass' custom text attributes have been stripped. We save a copy to use as a point of reference for
    //          user taps.
    
    [self setAttributedText:attributedText];
    [[self markdownView] setAttributedText:attributedText];
}

@end
