//
//  UIFont+Additions.m
//  Bypass
//
//  Created by Damian Carrillo on 3/6/13.
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

#import "UIFont+Additions.h"

CTFontRef BPCreateFontFromFont(UIFont *font)
{
    CFStringRef fontName = (__bridge CFStringRef) [font fontName];
    CGFloat fontSize = [font pointSize];
    CTFontRef result = CTFontCreateWithName(fontName, fontSize, NULL);
    
    // Note: the caller is in charge of calling CFRelease(...)
    
    return result;
}

@implementation UIFont (Additions)

+ (id)BP_fontFromFont:(CTFontRef)ctFont
{
    NSString *fontName = (__bridge_transfer NSString *) CTFontCopyName(ctFont, kCTFontPostScriptNameKey);
    CGFloat fontSize = CTFontGetSize(ctFont);
    UIFont *font = [UIFont fontWithName:fontName size:fontSize];
    return font;
}

+ (id)BP_fontFromFontDescriptor:(CTFontDescriptorRef)fontDescriptor
{
    CGFloat fontSize;
    CFNumberRef fontSizeObj = CTFontDescriptorCopyAttribute(fontDescriptor, kCTFontSizeAttribute);
    CFNumberGetValue(fontSizeObj, kCFNumberCGFloatType, &fontSize);
    CTFontRef font = CTFontCreateWithFontDescriptor(fontDescriptor, fontSize, NULL);
    
    UIFont *result = [self BP_fontFromFont:font];
    
    CFRelease(font);
    CFRelease(fontSizeObj);
    
    return result;
}

@end
