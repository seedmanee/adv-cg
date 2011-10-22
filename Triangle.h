
#ifndef Triangle_h
#define Triangle_h

#include "Primitive.h"
#include "Point.h"
#include <vector>

class Ray;

class Triangle : public Primitive {
public:
  Triangle(Material* material, const Point (&points)[3], const Vector &velocity);
	
  virtual ~Triangle();
	
  virtual void getBounds(BoundingBox& bbox, double t) const;
  virtual void intersect(HitRecord& hit, const RenderContext& context, const Ray& ray, double t) const;
  virtual void normal(Vector& normal, const RenderContext& context,
                      const Point& hitpos, const Ray& ray, const HitRecord& hit, double t) const;
	virtual void getName()const {
		std::cout << "Triangle" << std::endl;
	}
protected:
	Point points[3];
	Vector velocity;
};

#endif
