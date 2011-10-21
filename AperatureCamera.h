
#ifndef AperatureCamera_h
#define AperatureCamera_h

#include "Camera.h"
#include "Point.h"
#include "Vector.h"
#include "PinholeCamera.h"

class AperatureCamera : public PinholeCamera {
public:
  AperatureCamera(const Point& eye, const Point& lookat, const Vector& up, double hfov, double lensRadius, double focalDistance, double start_time, double end_time);
  virtual ~AperatureCamera();
  virtual void makeRay(Ray& ray, const RenderContext& context, double x, double y) const;

	double lensRadius;
	double focalDistance;

private:
  AperatureCamera(const AperatureCamera&);
  AperatureCamera& operator=(const AperatureCamera&);
	
};

#endif

