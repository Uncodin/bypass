//
// Created by Audun Holm Ellertsen on 4/30/13.
// Copyright (c) 2013 Uncodin. All rights reserved.
//
// To change the template use AppCode | Preferences | File Templates.
//

#import <UIKit/UIKit.h>
#import <CoreText/CoreText.h>

@interface BPDisplaySettings : NSObject

@property(nonatomic) UIColor *defaultColor;
@property(nonatomic) UIColor *quoteColor;
@property(nonatomic) UIColor *codeColor;
@property(nonatomic) UIColor *linkColor;

@property(nonatomic) UIFont *defaultFont;
@property(nonatomic) UIFont *boldFont;
@property(nonatomic) UIFont *italicFont;
@property(nonatomic) UIFont *boldItalicFont;
@property(nonatomic) UIFont *monospaceFont;
@property(nonatomic) UIFont *quoteFont;
@property(nonatomic) UIFont *h1Font;
@property(nonatomic) UIFont *h2Font;
@property(nonatomic) UIFont *h3Font;
@property(nonatomic) UIFont *h4Font;
@property(nonatomic) UIFont *h5Font;
@property(nonatomic) UIFont *h6Font;

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