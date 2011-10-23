
#ifndef Polygon_h
#define Polygon_h

#include "Primitive.h"
#include "Point.h"
#include <vector>

class Ray;
//class Vector;
class Point;

class Polygon : public Primitive {
public:
  Polygon(Material* material, const Vector &velocity, std::vector<Point *>plist, int *flist, int fn);
  virtual ~Polygon();
	
  virtual void getBounds(BoundingBox& bbox, double t) const;
  virtual void intersect(HitRecord& hit, const RenderContext& context, const Ray& ray, double t) const;
  virtual void normal(Vector& normal, const RenderContext& context,
                      const Point& hitpos, const Ray& ray, const HitRecord& hit, double t) const;
	virtual void getName()const {
		std::cout << "Polygon" << std::endl;
	}
	
protected:
	std::vector<Point *>plist;
	int fn;
	int *flist;
	Vector velocity;
private:
	void BetaGammaT(Point &,const Vector&,const Vector&,const Vector&,const Vector&) const;
};

#endif
