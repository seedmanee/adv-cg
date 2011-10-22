
#include "Triangle.h"
#include "BoundingBox.h"
#include "HitRecord.h"
#include "Point.h"
#include "Ray.h"
#include "Vector.h"
#include <math.h>

Triangle::Triangle(Material* material, const Point (&points)[3], const Vector &velocity)
: Primitive(material), velocity(velocity)
{
	this->points[0] = points[0];
	this->points[1] = points[1];
	this->points[2] = points[2];
}

Triangle::~Triangle()
{
}

void Triangle::getBounds(BoundingBox& bbox, double t) const
{
}

void Triangle::intersect(HitRecord& hit, const RenderContext&, const Ray& ray, double t) const
{
}

void Triangle::normal(Vector& normal, const RenderContext&, const Point& hitpos,
                    const Ray& ray, const HitRecord& hit, double t) const
{
}
