//
//  BPParser.h
//  Bypass
//
//  Created by Damian Carrillo on 2/28/13.
//  Copyright (c) 2013 Uncodin. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BPDocument.h"

@interface BPParser : NSObject

- (BPDocument *)parse:(NSString *)markdown;

@end
