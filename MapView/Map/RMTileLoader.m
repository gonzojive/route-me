//
//  RMTimeImageSet.m
//
// Copyright (c) 2008-2009, Route-Me Contributors
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
#import "RMGlobalConstants.h"
#import "RMTileLoader.h"

#import "RMTileImage.h"
#import "RMTileSource.h"
#import "RMPixel.h"
#import "RMMercatorToScreenProjection.h"
#import "RMFractalTileProjection.h"
#import "RMTileImageSet.h"

#import "RMTileCache.h"

@implementation RMTileLoader

#pragma mark -
#pragma mark Simple properties
@synthesize loadedBounds, loadedZoom;

#pragma mark -
#pragma mark Initialization and deallocation
-(id) init
{
	if (![self initWithContent: nil])
		return nil;
	
	return self;
}

-(id) initWithContent: (RMMapContents *)_contents
{
	if (![super init])
		return nil;
	
	content = _contents;
	
	[self clearLoadedBounds];
	loadedTileRect.origin.tile = RMTileDummy();
	
	suppressLoading = NO;
	
	return self;
}

-(void) dealloc
{
	[[NSNotificationCenter defaultCenter] removeObserver:self];
	[super dealloc];
}

#pragma mark -
#pragma mark Resetting the loaded tiles and bounds
-(void) clearLoadedBounds
{
	loadedBounds = CGRectZero;
}

- (void)reset
{
	loadedTileRect.origin.tile = RMTileDummy();
}

- (void)reload
{
	[self clearLoadedBounds];
	[self updateLoadedImages];
}

#pragma mark -
#pragma mark Loading
-(BOOL) screenIsLoaded
{
	//	RMTileRect targetRect = [content tileBounds];
	BOOL contained = CGRectContainsRect(loadedBounds, [content screenBounds]);
	
	int targetZoom = (int)([[content mercatorToTileProjection] calculateNormalisedZoomFromScale:[content scaledMetersPerPixel]]);
	if((targetZoom > content.maxZoom) || (targetZoom < content.minZoom))
          RMLog(@"target zoom %d is outside of RMMapContents limits %f to %f",
			  targetZoom, content.minZoom, content.maxZoom);
	if (contained == NO)
	{
		// RMLog(@"reassembling because its not contained");
	}
	
	if (targetZoom != loadedZoom)
	{
		// RMLog(@"reassembling because target zoom = %f, loaded zoom = %d", targetZoom, loadedZoom);
	}
	
	return contained && targetZoom == loadedZoom;
}


- (BOOL) shouldLoadImages
{
  if (suppressLoading)
		return NO;
	
	if ([content mercatorToTileProjection] == nil || [content  
													  mercatorToScreenProjection] == nil)
		return NO;
	
	// Delay display of new images until expensive operations are allowed.
	[[NSNotificationCenter defaultCenter] removeObserver:self  
													name:RMResumeExpensiveOperations object:nil];
	if ([RMMapContents performExpensiveOperations] == NO)
	{
        [[NSNotificationCenter defaultCenter] addObserver:self  
												 selector:@selector(updateLoadedImages)  
													 name:RMResumeExpensiveOperations object:nil];
        return NO;
	}
	
	if ([self screenIsLoaded])
		return NO;

    return YES;
}
-(void) updateLoadedImages
{
	if (![self shouldLoadImages])
        return;
    
    // Create a tile rectangle for the current region and zoom level of the map.
    RMTileRect newTileRect = [content tileBounds];
    
    // Ge the list of images that are already on the screen, and add new ones to canvas
    // the newTileRect with tile images.
    RMTileImageSet *images = [content imagesOnScreen];
    images.zoom = newTileRect.origin.tile.zoom;
    CGRect newLoadedBounds = [images addTiles:newTileRect ToDisplayIn: [content screenBounds]];
	
    // Get rid of images that are outside of the newTileRect.
    // TODO: explain-- for some reason, we also check to make sure loadedTiles.origin.tile is not a dummy tile
    // loadedTiles.origin.tile is set to a dummy when the loader is reset.  So resetting the loader
    // prevents tiles from being removed.
	if (!RMTileIsDummy(loadedTileRect.origin.tile))
	{
		[images removeTilesOutsideOf:newTileRect];
	}
	
    // Finally, update the state of this object to reflect the changes in bounds and tiles.
    loadedBounds = newLoadedBounds;
    loadedZoom = newTileRect.origin.tile.zoom;
    loadedTileRect = newTileRect;
	
    [content tilesUpdatedRegion:newLoadedBounds];
}

#pragma mark -
#pragma mark Manipulating the bounds and zoom
- (void)moveBy: (CGSize) delta
{
	loadedBounds = RMTranslateCGRectBy(loadedBounds, delta);
	[self updateLoadedImages];
}

- (void)zoomByFactor: (float) zoomFactor near:(CGPoint) center
{
	loadedBounds = RMScaleCGRectAboutPoint(loadedBounds, zoomFactor, center);
	[self updateLoadedImages];
}

#pragma mark -
#pragma mark Toggling the suppression of loading tiles
- (BOOL) suppressLoading
{
	return suppressLoading;
}

- (void) setSuppressLoading: (BOOL) suppress
{
	suppressLoading = suppress;
	
	if (suppress == NO)
		[self updateLoadedImages];
}

@end
