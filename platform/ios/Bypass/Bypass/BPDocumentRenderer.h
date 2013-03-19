//
//  BPDocumentRenderer.h
//  Bypass
//
//  Created by Damian Carrillo on 3/14/13.
//  Copyright (c) 2013 Uncodin. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface BPDocumentRenderer : NSObject

/*!
 \brief Initializes a document renderer that will render attributed strings into a view. 
 
 This method will initialize the renderer with a particular size that pages should be drawn
 at. This is configurable because the intent is that you know an appropriate size for your 
 rendered documents. 
 
 You _can_ use something likeCGSizeMake(CGRectGetWidth([[self view] frame], CGFLOAT_MAX)) 
 to get an unbounded height, however, doing so will cause this renderer to output a single 
 view that is the exact height that Core Text prescribes based on the attributes of your
 string. If the view is larger than the screen, there will be unnecessary drawing happening 
 off screen. If the view is smaller than the screen, supplying an unbounded height or width
 is perfectly feasible.
 
 \param pageSize the suggested page size
 */
- (id)initWithPageSize:(CGSize)pageSize;

- (NSArray *)renderDocument:(BPDocument *)document
     suggestedContainerSize:(CGSize *)suggestedContainerSizeOut;

@end
