//
// Created by Audun Holm Ellertsen on 4/30/13.
// Copyright (c) 2013 Uncodin. All rights reserved.
//
// To change the template use AppCode | Preferences | File Templates.
//


#import "BPAttributedStringConverter.h"
#import <CoreText/CoreText.h>
#import "BPDisplaySettings.h"


@implementation BPDisplaySettings
{
  CTFontRef _defaultFont;
  CTFontRef _boldFont;
  CTFontRef _italicFont;
  CTFontRef _boldItalicFont;
  CTFontRef _monospaceFont;
  CTFontRef _quoteFont;
  CTFontRef _h1Font;
  CTFontRef _h2Font;
  CTFontRef _h3Font;
  CTFontRef _h4Font;
  CTFontRef _h5Font;
  CTFontRef _h6Font;
}

- (id)init
{
  self = [super init];
  if (self) {
    self.defaultColor = [UIColor blackColor];
    self.bulletIndentation = 13.0f;
    self.codeIndentation = 10.0f;
    self.quoteIndentation = 23.0f;
    self.paragraphSpacing = 20.0f;
    self.paragraphSpacingHeading = 10.0f;
    self.paragraphSpacingCode = 0.0f;

    self.paragraphLineSpacing = 1.2f;
    self.paragraphLineSpacingHeading = 1.2f;

  }
  return self;
}


- (void)dealloc
{
  if (_defaultFont != NULL) {CFRelease(_defaultFont);}
  if (_boldFont != NULL) {CFRelease(_boldFont);}
  if (_italicFont != NULL) {CFRelease(_italicFont);}
  if (_boldItalicFont != NULL) {CFRelease(_boldItalicFont);}
  if (_monospaceFont != NULL) {CFRelease(_monospaceFont);}
  if (_quoteFont != NULL) {CFRelease(_quoteFont);}
  if (_h1Font != NULL) {CFRelease(_h1Font);}
  if (_h2Font != NULL) {CFRelease(_h2Font);}
  if (_h3Font != NULL) {CFRelease(_h3Font);}
  if (_h4Font != NULL) {CFRelease(_h4Font);}
  if (_h5Font != NULL) {CFRelease(_h5Font);}
  if (_h6Font != NULL) {CFRelease(_h6Font);}
}

- (void)setDefaultFont:(CTFontRef)defaultFont
{
  if (_defaultFont) {CFRelease(_defaultFont);}
  _defaultFont = CFRetain(defaultFont);
}

- (void)setBoldFont:(CTFontRef)boldFont
{
  if (_boldFont) {CFRelease(_boldFont);}
  _boldFont = CFRetain(boldFont);
}

- (void)setItalicFont:(CTFontRef)italicFont
{
  if (_italicFont) {CFRelease(_italicFont);}
  _italicFont = CFRetain(italicFont);
}

- (void)setBoldItalicFont:(CTFontRef)boldItalicFont
{
  if (_boldItalicFont) {CFRelease(_boldItalicFont);}
  _boldItalicFont = CFRetain(boldItalicFont);
}

- (void)setMonospaceFont:(CTFontRef)monospaceFont
{
  if (_monospaceFont) {CFRelease(_monospaceFont);}
  _monospaceFont = CFRetain(monospaceFont);
}

- (void)setQuoteFont:(CTFontRef)quoteFont
{
  if (_quoteFont) {CFRelease(_quoteFont);}
  _quoteFont = CFRetain(quoteFont);
}

- (void)setH1Font:(CTFontRef)h1Font
{
  if (_h1Font) {CFRelease(_h1Font);}
  _h1Font = CFRetain(h1Font);
}

- (void)setH2Font:(CTFontRef)h2Font
{
  if (_h2Font) {CFRelease(_h2Font);}
  _h2Font = CFRetain(h2Font);
}

- (void)setH3Font:(CTFontRef)h3Font
{
  if (_h3Font) {CFRelease(_h3Font);}
  _h3Font = CFRetain(h3Font);
}

- (void)setH4Font:(CTFontRef)h4Font
{
  if (_h4Font) {CFRelease(_h4Font);}
  _h4Font = CFRetain(h4Font);
}

- (void)setH5Font:(CTFontRef)h5Font
{
  if (_h5Font) {CFRelease(_h5Font);}
  _h5Font = CFRetain(h5Font);
}

- (void)setH6Font:(CTFontRef)h6Font
{
  if (_h6Font) {CFRelease(_h6Font);}
  _h6Font = CFRetain(h6Font);
}

- (CTFontRef)defaultFont
{

  if (_defaultFont == NULL) {
    CGFloat systemFontSize = [UIFont systemFontSize];
    UIFont *systemFont = [UIFont systemFontOfSize:systemFontSize];
    CFStringRef systemFontName = (__bridge CFStringRef) [systemFont fontName];

    _defaultFont = CTFontCreateWithName(systemFontName, systemFontSize, NULL);
  }
  assert(_defaultFont != NULL);
  return _defaultFont;
}

- (CTFontRef)boldFont
{
  if (_boldFont == NULL) {
    _boldFont = CTFontCreateCopyWithSymbolicTraits([self defaultFont], 0.f, NULL, kCTFontBoldTrait, kCTFontBoldTrait);
  }
  assert(_boldFont != NULL);
  return _boldFont;
}

- (CTFontRef)italicFont
{
  if (_italicFont == NULL) {
    _italicFont = CTFontCreateCopyWithSymbolicTraits([self defaultFont], 0.f, NULL, kCTFontItalicTrait, kCTFontItalicTrait);
    if (_italicFont == NULL) {
      CGFloat systemFontSize = [UIFont systemFontSize];
      UIFont *systemFont = [UIFont italicSystemFontOfSize:systemFontSize];
      CFStringRef systemFontName = (__bridge CFStringRef) [systemFont fontName];

      _italicFont = CTFontCreateWithName(systemFontName, systemFontSize, NULL);
    }
    if (_italicFont == NULL) {
      CGAffineTransform matrix = {1, 0, 0.11/* skew angle */, 1, 0, 0};
      _italicFont = CTFontCreateCopyWithAttributes([self defaultFont], 0.0, &matrix, NULL);
      CFStringRef fontName = CTFontCopyFullName([self defaultFont]);
      NSLog(@"Warning: can't find an italic font variant for font %@. Fallback to matrix/skew used.",
            (__bridge NSString *) fontName);
      CFRelease(fontName);
    }
  }

  assert(_italicFont != NULL);
  return _italicFont;
}

- (CTFontRef)boldItalicFont
{
  if (_boldItalicFont == NULL) {
    CTFontSymbolicTraits traits = kCTFontBoldTrait | kCTFontItalicTrait;
    CTFontSymbolicTraits mask = kCTFontBoldTrait | kCTFontItalicTrait;
    _boldItalicFont = CTFontCreateCopyWithSymbolicTraits([self defaultFont], 0.f, NULL, traits, mask);
  }
  assert(_boldItalicFont != NULL);
  return _boldItalicFont;
}

- (CTFontRef)monospaceFont
{
  if (_monospaceFont == NULL) {
    _monospaceFont = CTFontCreateWithName(CFSTR("Courier"), CTFontGetSize([self defaultFont]) - 2, NULL);
  }
  assert(_monospaceFont != NULL);
  return _monospaceFont;
}

- (CTFontRef)quoteFont
{
  if (_quoteFont == NULL) {
    _quoteFont = CTFontCreateWithName(CFSTR("Marion-Italic"), CTFontGetSize([self defaultFont]) + 2, NULL);
  }

  return _quoteFont;
}

- (CTFontRef)h1Font
{
  if (_h1Font == NULL) {
    _h1Font = CTFontCreateWithName(CFSTR("HelveticaNeue-CondensedBold"), CTFontGetSize([self defaultFont]) * 2, NULL);
  }

  return _h1Font;
}

- (CTFontRef)h2Font
{
  if (_h2Font == NULL) {
    _h2Font = CTFontCreateWithName(CFSTR("HelveticaNeue-CondensedBold"), CTFontGetSize([self defaultFont]) * 1.8, NULL);
  }

  return _h2Font;
}

- (CTFontRef)h3Font
{
  if (_h3Font == NULL) {
    _h3Font = CTFontCreateWithName(CFSTR("HelveticaNeue-CondensedBold"), CTFontGetSize([self defaultFont]) * 1.6, NULL);
  }

  return _h3Font;
}

- (CTFontRef)h4Font
{
  if (_h4Font == NULL) {
    _h4Font = CTFontCreateWithName(CFSTR("HelveticaNeue-CondensedBold"), CTFontGetSize([self defaultFont]) * 1.4, NULL);
  }

  return _h4Font;
}

- (CTFontRef)h5Font
{
  if (_h5Font == NULL) {
    _h5Font = CTFontCreateWithName(CFSTR("HelveticaNeue-CondensedBold"), CTFontGetSize([self defaultFont]) * 1.2, NULL);
  }

  return _h5Font;
}

- (CTFontRef)h6Font
{
  if (_h6Font == NULL) {
    _h6Font = CTFontCreateWithName(CFSTR("HelveticaNeue-CondensedBold"), CTFontGetSize([self defaultFont]) * 1, NULL);
  }

  return _h6Font;
}
@end
