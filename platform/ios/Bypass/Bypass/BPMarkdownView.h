//
//  BPMarkdownView.h
//  Bypass
//
//  Created by Damian Carrillo on 3/20/13.
//  Copyright (c) 2013 Uncodin. All rights reserved.
//

#import <UIKit/UIKit.h>

@protocol BPMarkdownViewLinkDelegate;

@interface BPMarkdownView : UIScrollView
@property (strong, nonatomic) NSString *markdown;
@property (assign, nonatomic, getter = isAsynchronous) BOOL asynchronous;
@property (assign, nonatomic) NSTimeInterval asynchronousRevealDuration;
@property (weak, nonatomic) id<BPMarkdownViewLinkDelegate> linkDelegate;

- (id)initWithFrame:(CGRect)frame markdown:(NSString *)markdown;

@end

@protocol BPMarkdownViewLinkDelegate <NSObject>

- (void)markdownView:(BPMarkdownView *)markdownView didHaveLinkTapped:(NSString *)link;

@end