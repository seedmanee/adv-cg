
#ifndef Object_h
#define Object_h

#include <iostream>

class BoundingBox;
class Color;
class HitRecord;
class Ray;
class RenderContext;

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
};

#endif

