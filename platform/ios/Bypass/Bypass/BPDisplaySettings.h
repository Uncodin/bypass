//
// Created by Audun Holm Ellertsen on 4/30/13.
// Copyright (c) 2013 Uncodin. All rights reserved.
//
// To change the template use AppCode | Preferences | File Templates.
//


#import <Foundation/Foundation.h>
#import <CoreText/CoreText.h>

@interface BPDisplaySettings : NSObject

@property(nonatomic) UIColor *defaultColor;

@property(nonatomic) CTFontRef defaultFont;
@property(nonatomic) CTFontRef boldFont;
@property(nonatomic) CTFontRef italicFont;
@property(nonatomic) CTFontRef boldItalicFont;
@property(nonatomic) CTFontRef monospaceFont;
@property(nonatomic) CTFontRef quoteFont;
@property(nonatomic) CTFontRef h1Font;
@property(nonatomic) CTFontRef h2Font;
@property(nonatomic) CTFontRef h3Font;
@property(nonatomic) CTFontRef h4Font;
@property(nonatomic) CTFontRef h5Font;
@property(nonatomic) CTFontRef h6Font;

@property(nonatomic) CGFloat bulletIndentation;
@property(nonatomic) CGFloat codeIndentation;
@property(nonatomic) CGFloat quoteIndentation;
@property(nonatomic) CGFloat lineSpacingSmall;

@property(nonatomic) CGFloat paragraphSpacing;
@property(nonatomic) CGFloat paragraphSpacingHeading;
@property(nonatomic) CGFloat paragraphSpacingH2;

@property(nonatomic) CGFloat paragraphSpacingCode;

@property(nonatomic) CGFloat paragraphLineSpacing;
@property(nonatomic) CGFloat paragraphLineSpacingHeading;

@property(nonatomic) CGFloat firstParagraphFirstLineHeadIndent;

@property(nonatomic) CGFloat paragraphFirstLineHeadIndent;
@property(nonatomic) CGFloat paragraphHeadIndent;

@property(nonatomic) CGFloat headerFirstLineHeadIndent;
@property(nonatomic) CGFloat headerHeadIndent;

@end
