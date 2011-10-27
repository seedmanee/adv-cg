
#ifndef Object_h
#define Object_h

#include <iostream>

class BoundingBox;
class Color;
class HitRecord;
class Ray;
class RenderContext;

inline double max(double a, double b){
	return (a>b)?a : b;
}

inline double min(double a, double b){
	return (a<b)?a : b;
}

class Object {
 public:
  Object();
  virtual ~Object();

  virtual void preprocess();
  virtual void getBounds(BoundingBox& bbox, double t) const = 0;
  virtual void intersect(HitRecord& hit, const RenderContext& context, const Ray& ray, double t) const = 0;
	virtual void getName() const = 0;
 private:
  Object(const Object&);
  Object& operator=(const Object&);
	
protected:
	double inRange(const double t) const{
		return min(max(t, start_time), end_time);
	}
	double start_time;
	double end_time;
};

#endif

