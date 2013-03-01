//
//  BPDocument.m
//  Bypass
//
//  Created by Damian Carrillo on 2/28/13.
//  Copyright (c) 2013 Uncodin. All rights reserved.
//

#import "BPDocumentPrivate.h"

@implementation BPDocument
{
    Bypass::Document _document;
    NSArray         *_elements;
}

- (id)init
{
    Bypass::Document document;
    return [self initWithDocument:document];
}

- (id)initWithDocument:(Bypass::Document)document
{
    self = [super init];
    
    if (self != nil) {
        _document = document;
    }
    
    return self;
}

- (NSArray *)elements
{
    using namespace Bypass;
    
    if (_elements == nil) {
        size_t i, count = _document.size();
        
        NSMutableArray *elements = [NSMutableArray arrayWithCapacity:count];
        
        for (i = 0; i < count; ++i) {
            Element e = _document[i];

            BPElement *ee = [[BPElement alloc] initWithElement:e];
            elements[i] = ee;
        }
        
        _elements = [NSArray arrayWithArray:elements];
    }
    
    return _elements;
}

@end
