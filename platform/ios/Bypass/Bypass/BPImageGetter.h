//
//  BPImageGetter.h
//  Bypass
//
//  Created by Victor Oliveira on 17/07/17.
//  Copyright © 2017 Uncodin. All rights reserved.
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
#ifndef BPImageGetter_h
#define BPImageGetter_h

@protocol BPImageGetter

@required
- (UIImage *) getUIImageWithSource:(NSString *)source;

@end

#endif /* BPImageGetter_h */
