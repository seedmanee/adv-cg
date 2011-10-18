
#ifndef AperatureCamera_h
#define AperatureCamera_h

#include "Camera.h"
#include "Point.h"
#include "Vector.h"

class AperatureCamera : public PinholeCamera {
public:
  AperatureCamera(const Point& eye, const Point& lookat, const Vector& up, double hfov);
  virtual ~AperatureCamera();
  
  virtual void preprocess(double aspect_ratio);
  virtual void makeRay(Ray& ray, const RenderContext& context, double x, double y) const;
	
private:
  AperatureCamera(const PinholeCamera&);
  AperatureCamera& operator=(const PinholeCamera&);
	
  Point eye;
  Point lookat;
  Vector up;
  double hfov;
	
  Vector u;
  Vector v;
  Vector lookdir;
};

#endif

