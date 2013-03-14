//
//  BPAbstractViewController.m
//  BypassSample
//
//  Created by Damian Carrillo on 3/13/13.
//  Copyright (c) 2013 Uncodin. All rights reserved.
//

#import "BPAbstractViewController.h"

@interface BPAbstractViewController ()

@end

@implementation BPAbstractViewController

- (NSString *)sampleMarkdown
{
    NSString *samplePath = [[NSBundle mainBundle] pathForResource:@"sample" ofType:@"markdown"];
    
    NSError *error = nil;
    NSString *sample = [NSString stringWithContentsOfFile:samplePath encoding:NSUTF8StringEncoding error:&error];
    
    if (error != nil) {
        NSLog(@"Error: %@", error);
        return nil;
    }
    
    return sample;
}

@end
