//
//  BPElementPrivate.h
//  Bypass
//
//  Created by Damian Carrillo on 3/1/13.
//  Copyright (c) 2013 Uncodin. All rights reserved.
//

#import "BPElement.h"
#import "element.h"

@interface BPElement (Private)

- (id)init;
- (id)initWithElement:(Bypass::Element)element;

@end