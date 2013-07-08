//
//  BPElement.h
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

#import <Foundation/Foundation.h>

typedef NSUInteger BPElementType;

// Block Element Types

FOUNDATION_EXPORT const BPElementType BPBlockCode;
FOUNDATION_EXPORT const BPElementType BPBlockQuote;
FOUNDATION_EXPORT const BPElementType BPBlockHTML;
FOUNDATION_EXPORT const BPElementType BPHeader;
FOUNDATION_EXPORT const BPElementType BPHRule;
FOUNDATION_EXPORT const BPElementType BPList;
FOUNDATION_EXPORT const BPElementType BPListItem;
FOUNDATION_EXPORT const BPElementType BPParagraph;
FOUNDATION_EXPORT const BPElementType BPTable;
FOUNDATION_EXPORT const BPElementType BPTableCell;
FOUNDATION_EXPORT const BPElementType BPTableRow;

// Span Element Types

FOUNDATION_EXPORT const BPElementType BPAutoLink;
FOUNDATION_EXPORT const BPElementType BPCodeSpan;
FOUNDATION_EXPORT const BPElementType BPDoubleEmphasis;
FOUNDATION_EXPORT const BPElementType BPEmphasis;
FOUNDATION_EXPORT const BPElementType BPImage;
FOUNDATION_EXPORT const BPElementType BPLineBreak;
FOUNDATION_EXPORT const BPElementType BPLink;
FOUNDATION_EXPORT const BPElementType BPRawHTMLTag;
FOUNDATION_EXPORT const BPElementType BPTripleEmphasis;
FOUNDATION_EXPORT const BPElementType BPText;
FOUNDATION_EXPORT const BPElementType BPStrikethrough;

@interface BPElement : NSObject
@property (assign, nonatomic, readonly) BPElementType elementType;
@property (strong, nonatomic, readonly) NSString      *text;
@property (strong, nonatomic, readonly) NSDictionary  *attributes;
@property (weak,   nonatomic, readonly) BPElement     *parentElement;
@property (strong, nonatomic, readonly) NSArray       *childElements;

- (BOOL)isBlockElement;

#if __has_feature(objc_subscripting)

/*!
 \brief Convenience accessor for retrieving child elements
 */
- (id)objectAtIndexedSubscript:(NSUInteger)idx;

/*!
 \brief Convenience accessor for retrieving attributes
 */
- (id)objectForKeyedSubscript:(id)key;

#endif

@end
