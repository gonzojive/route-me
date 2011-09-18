//
//  RMTileImage.h
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
	#import <UIKit/UIKit.h>
#else
	#import <Cocoa/Cocoa.h>
typedef NSImage UIImage;
#endif

#import "RMFoundation.h"
#import "RMNotifications.h"
#import "RMTile.h"
#import "RMTileProxy.h"
#import "FMDatabase.h"

@class RMTileImage;
@class NSData;

/*! RMTileImage loads and displays a particular map tile in the context of some view.
 
 At the moment, this object is responsible for both loading the image and rendering it.  The rendering
 scheme is ugly because it consists of a single CALayer object.  This implicitly assumes that a particular
 RMTileImage will be used in only one UIView. Might see some interesting crashes if you have two RMMapViews
 using the same tile source.
 
 Every RMTileImage will observe for an RMMapImageLoadingCancelledNotification to prevent loading.
 
 Upon the image being loaded, RMTileImage will also post an RMMapImageLoadedNotification to the default
 notification center.
 */
@interface RMTileImage : NSObject
{
	// I know this is a bit nasty.
	RMTile tile;
	CGRect screenLocation;
	
	/// Used by cache
	NSDate *lastUsedTime;
	
	/// \bug placing the "layer" on the RMTileImage implicitly assumes that a particular RMTileImage will be used in only 
	/// one UIView. Might see some interesting crashes if you have two RMMapViews using the same tile source.
	// Only used when appropriate
	CALayer *layer;
}

/// Creates an RMTileImage given an RMTile point, but does not load an image.
- (id) initWithTile: (RMTile)tile;

/// Returns a new RMTileImage that has an RMTileDummy() as the tile.  The tile will not have a loaded image.
+ (RMTileImage*) dummyTile: (RMTile)tile;

/// GET RID OF THIS
- (void)draw;

/// Creates a tile image given an RMTile point and a URL of the iamge to load.
+ (RMTileImage*)imageForTile: (RMTile) tile withURL: (NSString*)url;
/// Creates a tile image given an RMTile point and a file of an image to load.
+ (RMTileImage*)imageForTile: (RMTile) tile fromFile: (NSString*)filename;
/// Creates a tile image given an RMTile point and a data representation of the image.
+ (RMTileImage*)imageForTile: (RMTile) tile withData: (NSData*)data;
/// Creates a tile image given an RMTile point and an FMDatabase.
/// The RMTile point is enough to identify the image to be loaded from the database.
+ (RMTileImage*)imageForTile: (RMTile) tile fromDB: (FMDatabase*)db;

/// Adjusts the screen location to account for a translation.  Moves the offset of the tile
/// to screenLocation.origin + delta
- (void)moveBy: (CGSize) delta;

/// Adjusts the screen rectangle to account for a zoom action.
/// Uses [Code]RMScaleCGRectAboutPoint(screenLocation, zoomFactor, center)[/Code] to update screen location.
- (void)zoomByFactor: (float) zoomFactor near:(CGPoint) center;

/// Creates the CALayer accessible with #layer.  See #layer for details.
- (void)makeLayer;

/// Prevents the tile from loading further.
- (void)cancelLoading;

/// Updates the object with image data, and posts a notification that the image tile was loaded.
- (void)updateImageUsingData: (NSData*) data;
/// Updates the object with an image, setting the layer contents.
- (void)updateImageUsingImage: (UIImage*) image;

/// Updates the lastUsedTime for this tile to help with caching.
- (void)touch;

/// Returns true if the image for this RMTileImage is available.
- (BOOL)isLoaded;

/// Displays an image in the place of the real tile image.
- (void) displayProxy:(UIImage*)img;

/// The location inside some view of this particular tile.
@property (readwrite, assign) CGRect screenLocation;
/// The tile identifier of this tile image.
/// For each tile source, each tile image has a unique RMTile representations
@property (readonly, assign) RMTile tile;
/// The core animation layer that is used to render this particular tile.
/// Note that the current architecture allows a tile to be loaded in only one
/// view at a time (as a single CALayer object cannot reside in multiple views).
@property (readonly) CALayer *layer;
/// The last time at which #touch was called.
@property (readonly) NSDate *lastUsedTime;

@end
