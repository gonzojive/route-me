//
//  RMMapContents.h
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
#import <UIKit/UIKit.h>

#import "RMFoundation.h"
#import "RMLatLong.h"
#import "RMTile.h"

#import "RMTilesUpdateDelegate.h"


// constants for boundingMask
enum {
	// Map can be zoomed out past view limits
	RMMapNoMinBound			= 0,
	// Minimum map height when zooming out restricted to view height
	RMMapMinHeightBound		= 1,
	// Minimum map width when zooming out restricted to view width ( default )
	RMMapMinWidthBound		= 2
};

#define kDefaultInitialLatitude -33.858771
#define kDefaultInitialLongitude 151.201596

#define kDefaultMinimumZoomLevel 0.0
#define kDefaultMaximumZoomLevel 25.0
#define kDefaultInitialZoomLevel 13.0

@class RMMarkerManager;
@class RMProjection;
@class RMMercatorToScreenProjection;
@class RMTileImageSet;
@class RMTileLoader;
@class RMMapRenderer;
@class RMMapLayer;
@class RMLayerCollection;
@class RMMarker;
@protocol RMMercatorToTileProjection;
@protocol RMTileSource;


@protocol RMMapContentsAnimationCallback <NSObject>
@optional
- (void)animationFinishedWithZoomFactor:(float)zoomFactor near:(CGPoint)p;
- (void)animationStepped;
@end


/*! \brief The cartographic and data components of a map. Do not retain.
 
 There is exactly one RMMapContents instance for each RMMapView instance.
 
 \warning Do not retain an RMMapContents instance. Instead, ask the RMMapView for its contents 
 when you need it. It is an error for an RMMapContents instance to exist without a view, and 
 if you retain the RMMapContents, it can't go away when the RMMapView is released.
 
 At some point, it's likely that RMMapContents and RMMapView will be merged into one class.
 
 */
@interface RMMapContents : NSObject
{
	/// This is the underlying UIView's layer.
	CALayer *layer;
	
	RMMarkerManager *markerManager;

	/// The background subview displays while tiles are loading. Set its contents by providing your own "loading.png".
	RMMapLayer *background;

	/// The overlay subview displays markers and paths.
	RMLayerCollection *overlay;
	
	/// The projection object converts between latitude/longitude and RMProjectedPoint and friends.
	RMProjection *projection;
	
	id<RMMercatorToTileProjection> mercatorToTileProjection;
	
	/// This projection object converts between projected meters and screen pixel coordinates.
	RMMercatorToScreenProjection *mercatorToScreenProjection;
	
	/// The tile source controls what images are used.
    /// It can be changed while the view is visible, but see http://code.google.com/p/route-me/issues/detail?id=12
	id<RMTileSource> tileSource;
	
	RMTileImageSet *imagesOnScreen;
	RMTileLoader *tileLoader;
	
	RMMapRenderer *renderer;
	NSUInteger boundingMask;
	
	/*! The minimum zoom number allowed for the view prevents the map from zooming in too closely.
    
        #minZoom and #maxZoom must be within the limits of #tileSource but can be stricter.  They 
        are clamped to tilesource limits if needed.
     */
	float minZoom;
	/*! The maximum zoom number allowed for the view prevents the map from zooming out too far.
     
     #minZoom and #maxZoom must be within the limits of #tileSource but can be stricter.  They 
     are clamped to tilesource limits if needed.
     */
	float maxZoom;

    float screenScale;

	id<RMTilesUpdateDelegate> tilesUpdateDelegate;
}

/// The center coordinate of the displayed region of the map.
@property (readwrite) CLLocationCoordinate2D mapCenter;
/// The center of the displayed region of the map in projected space.
@property (readwrite) RMProjectedPoint centerProjectedPoint;
/// The bounds of the displayed portion of the map in projected space.
@property (readwrite) RMProjectedRect projectedBounds;
/// ???
@property (readonly)  RMTileRect tileBounds;
/// The screen coordinates of the displayed region of the map.
@property (readonly)  CGRect screenBounds;
/// The approximate number of meters displayed for every pixel.
@property (readwrite) float metersPerPixel;
/// ???
@property (readonly)  float scaledMetersPerPixel;
/// The current zoom level.  Note that the zoom level is clamped to [minZoom, maxZoom].
@property (readwrite) float zoom;

/*! The minimum zoom number allowed for the view prevents the map from zooming in too closely.
 
 #minZoom and #maxZoom must be within the limits of #tileSource but can be stricter.  They 
 are clamped to tilesource limits if needed.
 */
@property (nonatomic, readwrite) float minZoom;
/*! The maximum zoom number allowed for the view prevents the map from zooming out too far.
 
 #minZoom and #maxZoom must be within the limits of #tileSource but can be stricter.  They 
 are clamped to tilesource limits if needed.
 */
@property (nonatomic, readwrite) float maxZoom;

@property (nonatomic, assign) float screenScale;

@property (readonly)  RMTileImageSet *imagesOnScreen;
@property (readonly)  RMTileLoader *tileLoader;


/// The projection object converts between latitude/longitude and RMProjectedPoint and friends.
@property (readonly)  RMProjection *projection;
/// The projection used to map from RMProjectedPoint space to the tile space.
@property (readonly)  id<RMMercatorToTileProjection> mercatorToTileProjection;
/// This projection object converts between projected meters and screen pixel coordinates.
@property (readonly)  RMMercatorToScreenProjection *mercatorToScreenProjection;

/// The source of map tiles.  By default this is set to an instance of RMOpenStreetMapSource.
@property (retain, readwrite) id<RMTileSource> tileSource;
/// The object responsible for rendering map tiles (should probably be private).
@property (retain, readwrite) RMMapRenderer *renderer;

@property (readonly)  CALayer *layer;

/// The background subview displays while tiles are loading. Set its contents by providing your own "loading.png".
@property (retain, readwrite) RMMapLayer *background;
/// The overlay subview displays markers and paths.
@property (retain, readwrite) RMLayerCollection *overlay;
@property (retain, readonly)  RMMarkerManager *markerManager;
/// \bug probably shouldn't be retaining this delegate
@property (nonatomic, retain) id<RMTilesUpdateDelegate> tilesUpdateDelegate;
@property (readwrite) NSUInteger boundingMask;
/// The denominator in a cartographic scale like 1/24000, 1/50000, 1/2000000.
@property (readonly)double scaleDenominator;

// tileDepth defaults to zero. if tiles have no alpha, set this higher, 3 or so, to make zooming smoother
@property (readwrite, assign) short tileDepth;
@property (readonly, assign) BOOL fullyLoaded;

- (id)initWithView: (UIView*) view;
- (id)initWithView: (UIView*) view
		tilesource:(id<RMTileSource>)newTilesource;
/// designated initializer
- (id)initWithView:(UIView*)view
		tilesource:(id<RMTileSource>)tilesource
	  centerLatLon:(CLLocationCoordinate2D)initialCenter
		 zoomLevel:(float)initialZoomLevel
	  maxZoomLevel:(float)maxZoomLevel
	  minZoomLevel:(float)minZoomLevel
   backgroundImage:(UIImage *)backgroundImage;

/// \deprecated subject to removal at any moment after 0.5 is released
- (id) initForView: (UIView*) view;
/// \deprecated subject to removal at any moment after 0.5 is released
- (id) initForView: (UIView*) view WithLocation:(CLLocationCoordinate2D)latlong;
/// \deprecated subject to removal at any moment after 0.5 is released
- (id)initForView:(UIView*)view WithTileSource:(id<RMTileSource>)tileSource WithRenderer:(RMMapRenderer*)renderer LookingAt:(CLLocationCoordinate2D)latlong;

- (void)setFrame:(CGRect)frame;

- (void)handleMemoryWarningNotification:(NSNotification *)notification;
- (void)didReceiveMemoryWarning;

- (void)moveToLatLong: (CLLocationCoordinate2D)latlong;
/// \deprecate Use setCenterProjectedPoint: instead.
- (void)moveToProjectedPoint: (RMProjectedPoint)aPoint;

/// Move the map in screen coordinates.
- (void)moveBy: (CGSize) delta;
/// Zooms the map in or out by a particular factor. ??? What specificially
- (void)zoomByFactor: (float) zoomFactor near:(CGPoint) center;
- (void)zoomInToNextNativeZoomAt:(CGPoint) pivot animated:(BOOL) animated;
- (void)zoomOutToNextNativeZoomAt:(CGPoint) pivot animated:(BOOL) animated; 
- (void)zoomByFactor: (float) zoomFactor near:(CGPoint) center animated:(BOOL) animated;
- (void)zoomByFactor: (float) zoomFactor near:(CGPoint) center animated:(BOOL) animated withCallback:(id<RMMapContentsAnimationCallback>)callback;

- (void)zoomInToNextNativeZoomAt:(CGPoint) pivot;
- (void)zoomOutToNextNativeZoomAt:(CGPoint) pivot; 
- (float)adjustZoomForBoundingMask:(float)zoomFactor;
- (void)adjustMapPlacementWithScale:(float)aScale;
- (float)nextNativeZoomFactor;
- (float)prevNativeZoomFactor;

/// Zoom in on the region with opposite corners located on the provided GPS coordinates.
- (void)zoomWithLatLngBoundsNorthEast:(CLLocationCoordinate2D)ne SouthWest:(CLLocationCoordinate2D)se;
/// Sets the projected bounds of the displayed portion of the map, and updates the display.
- (void)zoomWithRMMercatorRectBounds:(RMProjectedRect)bounds;

/// Renders at least some portion the map.
- (void) drawRect: (CGRect) rect;

// During touch and move operations on the iphone its good practice to
// hold off on any particularly expensive operations so the user's exerience
// is not interrupted.
+ (BOOL) performExpensiveOperations;
+ (void) setPerformExpensiveOperations: (BOOL)p;

/// Returns the screen coordinate of the given latitude/longitude.
- (CGPoint)latLongToPixel:(CLLocationCoordinate2D)latlong;
- (CGPoint)latLongToPixel:(CLLocationCoordinate2D)latlong withMetersPerPixel:(float)aScale;

/// Returns an object that encodes both the location in tile space of the tile, and its offset in screen coordinates.
- (RMTilePoint)latLongToTilePoint:(CLLocationCoordinate2D)latlong withMetersPerPixel:(float)aScale;

/// Converts a pixel location to a longitude/latitude.
- (CLLocationCoordinate2D)pixelToLatLong:(CGPoint)aPixel;
- (CLLocationCoordinate2D)pixelToLatLong:(CGPoint)aPixel withMetersPerPixel:(float)aScale;

/// returns the smallest bounding box containing the entire screen
- (RMSphericalTrapezium) latitudeLongitudeBoundingBoxForScreen;
/// returns the smallest bounding box containing a rectangular region of the screen
- (RMSphericalTrapezium) latitudeLongitudeBoundingBoxFor:(CGRect) rect;

- (void)setRotation:(float)angle;

- (void) tilesUpdatedRegion:(CGRect)region;

/*! \brief Clear all images from the #tileSource's caching system.
 
 All of the existing RMTileSource implementations load tile images via NSURLRequest. It's possible that some images will remain in your
 application's shared URL cache. If you need to clear this out too, use this call:
 \code
 [[NSURLCache sharedURLCache] removeAllCachedResponses];
 \endcode
 */
-(void)removeAllCachedImages;

@end

/// Appears to be the methods actually implemented by RMMapContents, but generally invoked on RMMapView, and forwarded to the contents object.
@protocol RMMapContentsFacade

@optional
- (void)moveToLatLong: (CLLocationCoordinate2D)latlong;
- (void)moveToProjectedPoint: (RMProjectedPoint)aPoint;

- (void)moveBy: (CGSize) delta;
- (void)zoomByFactor: (float) zoomFactor near:(CGPoint) center;
- (void)zoomInToNextNativeZoomAt:(CGPoint) pivot animated:(BOOL) animated;
- (void)zoomOutToNextNativeZoomAt:(CGPoint) pivot animated:(BOOL) animated; 
- (void)zoomByFactor: (float) zoomFactor near:(CGPoint) center animated:(BOOL) animated;

- (void)zoomInToNextNativeZoomAt:(CGPoint) pivot;
- (void)zoomOutToNextNativeZoomAt:(CGPoint) pivot; 
- (float)adjustZoomForBoundingMask:(float)zoomFactor;
- (void)adjustMapPlacementWithScale:(float)aScale;

- (CGPoint)latLongToPixel:(CLLocationCoordinate2D)latlong;
- (CGPoint)latLongToPixel:(CLLocationCoordinate2D)latlong withMetersPerPixel:(float)aScale;
- (CLLocationCoordinate2D)pixelToLatLong:(CGPoint)aPixel;
- (CLLocationCoordinate2D)pixelToLatLong:(CGPoint)aPixel withMetersPerPixel:(float)aScale;

- (void)zoomWithLatLngBoundsNorthEast:(CLLocationCoordinate2D)ne SouthWest:(CLLocationCoordinate2D)se;
- (void)zoomWithRMMercatorRectBounds:(RMProjectedRect)bounds;

/// \deprecated name change pending after 0.5
- (RMSphericalTrapezium) latitudeLongitudeBoundingBoxForScreen;
/// \deprecated name change pending after 0.5
- (RMSphericalTrapezium) latitudeLongitudeBoundingBoxFor:(CGRect) rect;

- (void) tilesUpdatedRegion:(CGRect)region;


@end

