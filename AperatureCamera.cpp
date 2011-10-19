
#include "AperatureCamera.h"
#include "Ray.h"
#include "Math.h"
#include <cstdlib>
#include <math.h>

AperatureCamera::AperatureCamera(const Point& eye, const Point& lookat, const Vector& up,
                             double hfov, double lensRadius, double focalDistance)
: PinholeCamera(eye, lookat, up, hfov), lensRadius(lensRadius), focalDistance(focalDistance)
{
}

AperatureCamera::~AperatureCamera()
{
}


void AperatureCamera::makeRay(Ray& ray, const RenderContext& context, double x, double y, int n_ray) const
{	
  Vector direction = lookdir+u*x+v*y;
	direction.normalize();
	
//	Point new_origin;
//	Vector new_direction;
	double imageDistance = 16;
	
	if (n_ray != 0) {
		double r = sqrt(drand48());
		double theta = 2 * M_PI * drand48();
		
		double lensU = lensRadius * r * cos(theta);
		double lensV = lensRadius * r * sin(theta);
		
		// Compute point on plane of focus
		// image plane to lens distance
		// project plane to lens distance
		double projectDistance = imageDistance*focalDistance / (imageDistance - focalDistance);
		double ft = (projectDistance+imageDistance) / direction.length();
		Point Pfocus = ray.origin() + ray.direction() * ft;
		
		// Update ray for effect of lens
		Point new_origin = ray.origin()+direction + u * lensU + v * lensV;
		Vector new_direction = Pfocus - new_origin;
		new_direction.normalize();
		
		ray = Ray(new_origin, new_direction);
		
	} else {
		ray = Ray(eye, direction);
	}

}