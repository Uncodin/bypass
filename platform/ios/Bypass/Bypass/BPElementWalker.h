//
//  BPElementWalker.h
//  Bypass
//
//  Created by Damian Carrillo on 3/22/13.
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

#import <Foundation/Foundation.h>

@class    BPElement;
@class    BPDocument;
@protocol BPElementVisitor;

@interface BPElementWalker : NSObject

- (void)addElementVisitor:(id<BPElementVisitor>)elementVisitor;
- (void)walkDocument:(BPDocument *)document;

@end

@protocol BPElementVisitor <NSObject>
@required

- (void)elementWalker:(BPElementWalker *)elementWalker
     willVisitElement:(BPElement *)element
        withTextRange:(NSRange)textRange;

// Returns the number of characters added to the element's text. If characters were removed, the return value should be negative.
- (int)elementWalker:(BPElementWalker *)elementWalker
      didVisitElement:(BPElement *)element
        withTextRange:(NSRange)textRange;

@end