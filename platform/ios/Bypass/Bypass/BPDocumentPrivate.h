//
//  BPDocumentPrivate.h
//  Bypass
//
//  Created by Damian Carrillo on 3/1/13.
//  Copyright (c) 2013 Uncodin. All rights reserved.
//

#import "BPDocument.h"
#import "BPElementPrivate.h"
#import "document.h"

@interface BPDocument (Private)

- (id)init;
- (id)initWithDocument:(Bypass::Document)document;

@end