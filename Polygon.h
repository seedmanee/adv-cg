
#ifndef Polygon_h
#define Polygon_h

#include "Primitive.h"
#include "Point.h"
#include "Triangle.h"
#include <vector>

class Ray;
class Triangle;

class Polygon : public Primitive {
public:
  Polygon(Material* material, const Vector &velocity, Triangle *triangle_list, int triangle_number);
  virtual ~Polygon();
	
  virtual void getBounds(BoundingBox& bbox, double t) const;
  virtual void intersect(HitRecord& hit, const RenderContext& context, const Ray& ray, double t) const;
  virtual void normal(Vector& normal, const RenderContext& context,
                      const Point& hitpos, const Ray& ray, const HitRecord& hit, double t) const;
	virtual void getName()const {
		std::cout << "Polygon" << std::endl;
	}
	
protected:
	int triangle_number;
	Vector velocity;
};

#endif
