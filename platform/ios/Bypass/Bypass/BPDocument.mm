//
//  BPDocument.m
//  Bypass
//
//  Created by Damian Carrillo on 2/28/13.
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

#if __has_feature(objc_subscripting)

- (id)objectAtIndexedSubscript:(NSUInteger)idx
{
    return [[self elements] objectAtIndexedSubscript:idx];
}

#endif

- (NSString *)debugDescription
{
    NSMutableString *desc = [NSMutableString stringWithString:NSStringFromClass([self class])];
    [desc appendString:@"{ childElements:"];
    
    for (BPElement *element in [self elements]) {
        [desc appendFormat:@"%@\n", [element debugDescription]];
    }
    
    [desc appendString:@"}"];
    return desc;
}


@end
