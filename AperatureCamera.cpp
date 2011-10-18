/*
 *  AperatureCamera.cpp
 *  adv_cg_xcode
 *
 *  Created by Jui-Chung Wu on 10/18/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "AperatureCamera.h"
#include "Ray.h"
#include "Math.h"
#include <math.h>

/*
float PerspectiveCamera::GenerateRay(const CameraSample &sample, Ray *ray) const {
    // Generate raster and camera samples
    Point Pras(sample.imageX, sample.imageY, 0);
    Point Pcamera;
    RasterToCamera(Pras, &Pcamera);
    *ray = Ray(Point(0,0,0), Normalize(Vector(Pcamera)), 0.f, INFINITY);
    // Modify ray for depth of field
    if (lensRadius > 0.) {
        // Sample point on lens
        float lensU, lensV;
        ConcentricSampleDisk(sample.lensU, sample.lensV, &lensU, &lensV);
        lensU *= lensRadius;
        lensV *= lensRadius;

        // Compute point on plane of focus
        float ft = focalDistance / ray->d.z;
        Point Pfocus = (*ray)(ft);

				// Update ray for effect of lens
				ray->o = Point(lensU, lensV, 0.f);
				ray->d = Normalize(Pfocus - ray->o);
		}
		ray->time = Lerp(sample.time, shutterOpen, shutterClose);
		CameraToWorld(*ray, ray);
		return 1.f; 
}
*/