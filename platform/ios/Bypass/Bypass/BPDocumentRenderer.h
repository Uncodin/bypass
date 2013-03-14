//
//  BPDocumentRenderer.h
//  Bypass
//
//  Created by Damian Carrillo on 3/14/13.
//  Copyright (c) 2013 Uncodin. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface BPDocumentRenderer : NSObject

- (id)initWithPageSize:(CGSize)pageSize;
- (NSArray *)renderDocument:(BPDocument *)document
     suggestedContainerSize:(CGSize *)suggestedContainerSizeOut;

@end
