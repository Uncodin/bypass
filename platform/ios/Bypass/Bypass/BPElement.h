//
//  BPElement.h
//  Bypass
//
//  Created by Damian Carrillo on 2/28/13.
//  Copyright (c) 2013 Uncodin. All rights reserved.
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

@interface BPElement : NSObject
@property (assign, nonatomic, readonly) BPElementType elementType;
@property (strong, nonatomic, readonly) NSString      *text;
@property (strong, nonatomic, readonly) NSDictionary  *attributes;
@property (strong, nonatomic, readonly) NSArray       *childElements;
@end
