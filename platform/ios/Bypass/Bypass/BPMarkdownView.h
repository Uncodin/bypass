//
//  BPMarkdownView.h
//  Bypass
//
//  Created by Damian Carrillo on 3/20/13.
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

#import <UIKit/UIKit.h>

@protocol BPMarkdownViewLinkDelegate;
@class BPDisplaySettings;

@interface BPMarkdownView : UIScrollView

/*!
 * The markdown that this view will show.
 */
@property (strong, nonatomic) NSString *markdown;

@property (strong, nonatomic) BPDisplaySettings *displaySettings;

/*!
 * Whether or not to render the markdown asynchronously. If you are concerned with loading long
 * bodies of text, this may be of use to you. You can schedule the view to immediately load,
 * render markdown to Core Text frames in the background, and then show the results when it 
 * has finished.
 *
 * The default is to load synchronously.
 */
@property (assign, nonatomic, getter = isAsynchronous) BOOL asynchronous;

/*!
 * The duration at which an asynchronous load operation will fade the text in at. This property
 * only has an effect when this view loads asynchronously.
 */
@property (assign, nonatomic) NSTimeInterval asynchronousRevealDuration;

/*!
 * The delegate that should handle link taps. By default this view will use the system to open
 * links based on whatever handler is most appropriate.
 */
@property (weak, nonatomic) id<BPMarkdownViewLinkDelegate> linkDelegate;

- (id)initWithFrame:(CGRect)frame markdown:(NSString *)markdown;

@end

@protocol BPMarkdownViewLinkDelegate <NSObject>

- (void)markdownView:(BPMarkdownView *)markdownView didHaveLinkTapped:(NSString *)link;

@end