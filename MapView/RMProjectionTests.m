//
//  RMProjectionTests.m
//  MapView
//
//  Created by Red Daly on 9/18/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "RMProjectionTests.h"


@implementation RMProjectionTests


- (void)setUp {
    [super setUp];
	
    CLLocationCoordinate2D initialCenter;
    initialCenter.latitude = 66.44;
	initialCenter.longitude = -178.0;
	
	rmMapView = [[RMMapView alloc] initWithFrame:CGRectMake(10,20,200,300)
								  WithLocation:initialCenter];
}

-(void)tearDown
{
    [rmMapView release];
    rmMapView = nil;
	[super tearDown];
}

-(void) testSetProjection
{
    RMProjection * proj = rmMapView.contents.projection;
    
    // When we set the projected area of the map, the getter should return the same
    // value that we just set.
    RMProjectedRect rmProjRect = RMMakeProjectedRect(-14157831.136609977,
                                                     5229238.1644575465,
                                                     1000000.0,
                                                     1000000.0 * rmMapView.bounds.size.height / rmMapView.bounds.size.width);
    //[[rmMapView contents] setProjectedBounds: rmProjRect];
    rmMapView.contents.projectedBounds = rmProjRect;
    RMProjectedRect getterValue = rmMapView.contents.projectedBounds;
    
    STAssertEqualsWithAccuracy(rmProjRect.size.width, getterValue.size.width,
                               .1, @"Setting projected region should be exact.");
    STAssertEqualsWithAccuracy(rmProjRect.size.height, getterValue.size.height,
                               .1, @"Setting projected region should be exact.");
    
    STAssertEqualsWithAccuracy(rmProjRect.origin.easting, getterValue.origin.easting,
                               .1, @"Setting projected region should be exact.");
    STAssertEqualsWithAccuracy(rmProjRect.origin.northing, getterValue.origin.northing,
                               .1, @"Setting projected region should be exact.");
}

@end
