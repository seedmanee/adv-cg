
#include "Polygon.h"
#include "BoundingBox.h"
#include "HitRecord.h"
#include "Point.h"
#include "Ray.h"
#include "Vector.h"

#include <math.h>

Polygon::Polygon(Material* material, const Vector &velocity, Triangle *triangle_list, int triangle_number)
: Primitive(material), velocity(velocity), triangle_number(triangle_number)
{

}

Polygon::~Polygon()
{
}

void Polygon::getBounds(BoundingBox& bbox, double t) const
{
}

void Polygon::intersect(HitRecord& hit, const RenderContext&, const Ray& ray, double t) const
{
}

void Polygon::normal(Vector& normal, const RenderContext&, const Point& hitpos,
                    const Ray& ray, const HitRecord& hit, double t) const
{
}
