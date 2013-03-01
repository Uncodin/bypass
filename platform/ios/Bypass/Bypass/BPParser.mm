//
//  BPParser.m
//  Bypass
//
//  Created by Damian Carrillo on 2/28/13.
//  Copyright (c) 2013 Uncodin. All rights reserved.
//

#import "BPParser.h"
#import "BPDocumentPrivate.h"
#import "parser.h"

@implementation BPParser {
    Bypass::Parser _parser;
}

- (id)init
{
    self = [super init];
    
    if (self != nil) {
        _parser = Bypass::Parser();
    }
    
    return self;
}

- (BPDocument *)parse:(NSString *)markdown
{
    using namespace Bypass;
    
    const char *mstr = [markdown cStringUsingEncoding:NSUTF8StringEncoding];
    Document doc = _parser.parse(mstr);
    BPDocument *document = [[BPDocument alloc] initWithDocument:doc];
    
    return document;
}

@end
