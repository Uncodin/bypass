//
//  BPImageGetterSample.m
//  BypassSample
//
//  Created by Victor Oliveira on 17/07/17.
//  Copyright © 2017 Uncodin. All rights reserved.
//

#import "BPImageGetterSample.h"

@implementation BPImageGetterSample

- (UIImage *) getUIImageWithSource:(NSString *)source {
	
	return [UIImage imageNamed:source];
}

@end
