
#ifndef AperatureCamera_h
#define AperatureCamera_h

#include "Camera.h"
#include "Point.h"
#include "Vector.h"
#include "PinholeCamera.h"

class AperatureCamera : public PinholeCamera {
public:
  AperatureCamera(const Point& eye, const Point& lookat, const Vector& up, double hfov, double lensRadius, double focalDistance);
  virtual ~AperatureCamera();
  virtual void makeRay(Ray& ray, const RenderContext& context, double x, double y, int n_ray) const;
	
private:
  AperatureCamera(const AperatureCamera&);
  AperatureCamera& operator=(const AperatureCamera&);
	
	double lensRadius;
	double focalDistance;
};

#endif
