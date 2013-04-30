//
// Created by Audun Holm Ellertsen on 4/30/13.
// Copyright (c) 2013 Uncodin. All rights reserved.
//
// To change the template use AppCode | Preferences | File Templates.
//


#import <Foundation/Foundation.h>
#import <CoreText/CoreText.h>

@interface BPDisplaySettings : NSObject

- (CTFontRef)defaultFont;
- (CTFontRef)boldFont;
- (CTFontRef)italicFont;
- (CTFontRef)boldItalicFont;
- (CTFontRef)monospaceFont;
- (CTFontRef)quoteFont;
- (CTFontRef)h1Font;
- (CTFontRef)h2Font;
- (CTFontRef)h3Font;
- (CTFontRef)h4Font;
- (CTFontRef)h5Font;
- (CTFontRef)h6Font;

@property(nonatomic) CGFloat bulletIndentation;
@property(nonatomic) CGFloat codeIndentation;
@property(nonatomic) CGFloat quoteIndentation;
@property(nonatomic) CGFloat lineSpacingSmall;
@property(nonatomic) CGFloat paragraphSpacingLarge;
@property(nonatomic) CGFloat paragraphSpacingSmall;
@property(nonatomic) CGFloat paragraphSpacingNone;

@end
