//
//  BPImageGetterSample.h
//  BypassSample
//
//  Created by Victor Oliveira on 17/07/17.
//  Copyright © 2017 Uncodin. All rights reserved.
//

#import "BPImageGetter.h"

@interface BPImageGetterSample : NSObject<BPImageGetter>

- (UIImage *) getUIImageWithSource:(NSString *)source;

@end
