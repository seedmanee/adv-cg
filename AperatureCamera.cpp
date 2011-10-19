
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

void AperatureCamera::makeRay(Ray& ray, const RenderContext& context, double x, double y) const
{
	double r = sqrt(drand48());
	double theta = 2 * M_PI * drand48();
	
	double lensU = lensRadius * r * cos(theta);
	double lensV = lensRadius * r * sin(theta);
	
	Vector direction = lookdir + u * x + v * y;
  Vector refraction_direction = lookdir + u * (lensU + x) + v * (lensV + y);
	Point pointOnLens = eye + refraction_direction;
	
	direction.normalize();
	refraction_direction.normalize();
	
	Vector straight = lookdir * Dot(direction, lookat);
	
	double imageDistance = 10.0;

	double projectDistance = imageDistance * focalDistance / (imageDistance - focalDistance);
	double ft = (projectDistance + imageDistance) / straight.length();

	Point Pfocus = eye + direction * ft;
	
	// Update ray for effect of lens
	Vector new_direction = Pfocus - pointOnLens;
	new_direction.normalize();
	
	ray = Ray(pointOnLens, new_direction);
	
}