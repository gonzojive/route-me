//
//  RMTimeImageSet.h
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

#import <Foundation/Foundation.h>
#import "RMTile.h"
#import "RMTileImageSet.h"
#import "RMMapContents.h"

@class RMTileImage;
@class RMTileImageSet;
@class RMMercatorToScreenProjection;

@protocol RMTileSource;

/*! This object is responsible for ensuring that a particular region of the map,
 at a particular zoom level, has loaded tiles.
 
 This object does not actually store any RMTileImage instances, but relies instead
 on RMMapContents to keep track of the images that are currently loaded for display.
 */
@interface RMTileLoader : NSObject
{
	RMMapContents *content;

	CGRect loadedBounds;
	NSUInteger loadedZoom;
	RMTileRect loadedTiles;
	
	BOOL suppressLoading;
}

/// Designated initialiser.
- (id)initWithContent: (RMMapContents *)contents;

/// Do not use this initializer.
- (id)init;

/*! The main function of the class, responsible for syncrhonizing the set of loaded tiles and the map view region.
 
 This function will ensure that it is allowed to do anything with #shouldLoadImages and then proceed to
 load any tiles that are in the visible region of the map.
 */
- (void)updateLoadedImages;

/// Returns whether updateLoadedImages should even attempt to load more images.
/// There are a few ways of preventing images from being loaded:
///   - setting suppressLoading to TRUE
///   - if [RMMapContents performExpensiveOperations] == NO
///   - if the RMMapContents object does not have a projection suitable for mapping
///     coordinates to tiles.
- (BOOL) shouldLoadImages;

/// Changes the screen coordinate bounds of the tiles to be displayed.
- (void)moveBy: (CGSize) delta;
/// Changes the zoom level, and also the bounds, of the tiles to be displayed.  Updates 
- (void)zoomByFactor: (float) zoomFactor near:(CGPoint) center;

/// Clears the bounds that designate which area of the screen is guaranteed to have tiles that are loaded.
- (void)clearLoadedBounds;

/*! Reloads all map tiles in the image set, though not necessarily by fetching them anew from their original data source.
 
 This function will set the current #loadedBounds to CGRectZero() and then update the loaded images.
 
 This has the effect of getting rid of anything that is not in the current loadedBounds of the map contents,
 but it does not necessarily to out and download the tile images anew.
 */
- (void)reload;
/// Resets the loaded tiles rectangle.
- (void)reset;

/// The screen rectangle that is entirely filled in, at the current zoom level, with RMTileImage instances.
@property (readonly, nonatomic) CGRect loadedBounds;
/// The zoom level of the loaded image set.
@property (readonly, nonatomic) NSUInteger loadedZoom;
/// Set to false to prevent any new images from being loaded.  Set to false to allow loading again.
@property (readwrite, assign) BOOL suppressLoading;

//-(BOOL) containsRect: (CGRect)bounds;

@end
