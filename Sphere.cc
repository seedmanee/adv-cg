
#include "Sphere.h"
#include "BoundingBox.h"
#include "HitRecord.h"
#include "Point.h"
#include "Ray.h"
#include "Vector.h"
#include <math.h>

Sphere::Sphere(Material* material, const Point& center, double radius, const Vector& velocity, double start_time, double end_time)
  : Primitive(material), center(center), radius(radius), velocity(velocity)
{
  inv_radius = 1./radius;
	this->start_time = start_time;
	this->end_time = end_time;
}

Sphere::~Sphere()
{
}

void Sphere::getBounds(BoundingBox& bbox, double t) const
{
	Point t_center = center + (inRange(t) - start_time) * velocity;
  Vector diag(radius, radius, radius);
  bbox.extend(t_center+diag);
  bbox.extend(t_center-diag);
}

void Sphere::intersect(HitRecord& hit, const RenderContext&, const Ray& ray, double t) const
{
	Point t_center = center + (inRange(t) - start_time) * velocity;
  Vector O(ray.origin()-t_center);
  const Vector& V(ray.direction());
  double b = Dot(O, V);
  double c = Dot(O, O)-radius*radius;
  double disc = b*b-c;
  if(disc > 0){
    double sdisc = sqrt(disc);
    double root1 = (-b - sdisc);
		
		// if nearer point hit then return
		// else test farer point hit
    if(!hit.hit(root1, this, matl)){
      double root2 = (-b + sdisc);
      hit.hit(root2, this, matl);
    }
  }
}

void Sphere::normal(Vector& normal, const RenderContext&, const Point& hitpos,
                    const Ray& ray, const HitRecord& hit, double t) const
{
	Point t_center = center + (inRange(t) - start_time) * velocity;
  normal = (hitpos-t_center)*inv_radius;
}
