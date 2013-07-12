//
//  BPTextViewController.m
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

#import "BPTextViewController.h"
#import <Bypass.h>

@interface BPTextViewController ()
@property (weak, nonatomic) IBOutlet UITextView         *markdownView;
@property (copy, nonatomic)          NSAttributedString *attributedText;
@end

@implementation BPTextViewController

/*
 Responds to a user's tap, and if necessary opens the embedded URL.
 */
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
    
    // Seek out a bypass link attribute. The attributed string renderer will embed the URL
    // as a string into the attributes dictionary of a link.
    
    if ([self attributedText].length != 0 ) {
        
        NSDictionary *attributes = [[self  attributedText] attributesAtIndex:characterIndex effectiveRange:&effectiveRange];
        NSString *linkHREF = attributes[BPLinkStyleAttributeName];
        
        if (linkHREF != nil) {
            NSURL *linkURL = [NSURL URLWithString:linkHREF];
            [[UIApplication sharedApplication] openURL:linkURL];
        }
    }
}

- (void)viewWillAppear:(BOOL)animated
{
    NSString *sample = [self sampleMarkdown];
    
    BPParser *parser = [[BPParser alloc] init];
    BPDocument *document = [parser parse:sample];
    
    BPAttributedTextVisitor* textVisitor = [BPAttributedTextVisitor new];

    BPElementWalker* walker = [BPElementWalker new];
    [walker addElementVisitor:textVisitor];
    [walker walkDocument:document];
    
    NSAttributedString *attributedText = textVisitor.attributedText;
    
    // Warning: The attributed text is being set on a simple UITextView out of convenience. After this has been done,
    //          Bypass' custom text attributes have been stripped. We save a copy to use as a point of reference for
    //          user taps.
    
    [self setAttributedText:attributedText];
    [[self markdownView] setAttributedText:attributedText];
}

- (void)viewDidAppear:(BOOL)animated
{
    [[self markdownView] flashScrollIndicators];
}

@end
