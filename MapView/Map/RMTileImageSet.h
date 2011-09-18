//
//  RMTileImageSet.h
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

#import <TargetConditionals.h>
#if TARGET_OS_IPHONE
#	import <UIKit/UIKit.h>
#else
#	import <Cocoa/Cocoa.h>
#endif
#import "RMTile.h"

@class RMTileImage;
@protocol RMTileSource;

@protocol RMTileImageSetDelegate<NSObject>

@optional

-(void) tileRemoved: (RMTile) tile;
-(void) tileAdded: (RMTile) tile WithImage: (RMTileImage*) image;

@end

@interface RMTileImageSet : NSObject {
	IBOutlet id delegate;
	id<RMTileSource> tileSource;
	NSMutableSet *images;
	short zoom, tileDepth;
}
/// The delegate object that is informed when tiles are added and removed.
@property (assign, nonatomic, readwrite) id delegate;

/// tileDepth defaults to zero. If tiles have no alpha, set this higher, 3 or so, to make zooming smoother.
@property (assign, readwrite) short tileDepth;

/// The zoom level of the current region.
@property (assign, readwrite) short zoom;

/// Returns true if every image in the set has loaded.
@property (readonly) BOOL fullyLoaded;

/// Initializes the object.
-(id) initWithDelegate: (id) _delegate;

/// Add a tile from an already loaded image.
-(void) addTile: (RMTile) tile WithImage: (RMTileImage *)image At: (CGRect) screenLocation;
/// Addd a tile a particular screen location without specifying an image.
-(void) addTile: (RMTile) tile At: (CGRect) screenLocation;
/*! Add tile images to canvas the entire tile rect passed.
 
 Return screen rectangle containing bounds extended to full tile loading area.  This rectangle
 will fit tightly around the map tiles that were added, because the tiles that are added will
 form a rectangle and not some jagged shape.
 */
-(CGRect) addTiles: (RMTileRect)rect ToDisplayIn:(CGRect)bounds;

-(RMTileImage*) imageWithTile: (RMTile) tile;

/// Removes the tile image at the given point in tile space.
-(void) removeTile: (RMTile) tile;
/// Removes all tile images.
-(void) removeAllTiles;
/// Removes any tile that is determined to be lower quality (too stretched out usually) than a provided image.
-(void) removeTilesWorseThan: (RMTileImage *)newImage;
/// Removes tiles that are outside of some geographical boundary in tile space.
-(void) removeTilesOutsideOf: (RMTileRect)rect;

/// Changes the source of the tile set and clears any loaded tile images.
- (void) setTileSource: (id<RMTileSource>)newTileSource;

/// The number of images in the image set.
-(NSUInteger) count;

- (void)moveBy: (CGSize) delta;
- (void)zoomByFactor: (float) zoomFactor near:(CGPoint) center;

- (void)cancelLoading;

-(void) tileImageLoaded:(NSNotification *)notification;
-(BOOL) isTile: (RMTile)subject worseThanTile: (RMTile)object;
-(RMTileImage *) anyTileImage;

- (void) printDebuggingInformation;
@end
