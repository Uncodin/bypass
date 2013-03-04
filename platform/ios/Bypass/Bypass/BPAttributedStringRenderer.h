//
//  BPAttributedStringRenderer.h
//  Bypass
//
//  Created by Damian Carrillo on 3/1/13.
//  Copyright (c) 2013 Uncodin. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "BPDocument.h"

@interface BPAttributedStringRenderer : NSObject

- (NSAttributedString *)renderDocument:(BPDocument *)document;

@end
