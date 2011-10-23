
#include "Polygon.h"
#include "BoundingBox.h"
#include "HitRecord.h"
#include "Point.h"
#include "Ray.h"
#include "Vector.h"

#include <iostream>
#include <math.h>
#include <cfloat>

Polygon::Polygon(Material* material, const Vector &velocity, Point *plist, int pn, int *flist, int fn)
: Primitive(material), velocity(velocity), plist(plist), pn(pn), flist(flist), fn(fn)
{
	for (int i=0; i<pn; i++) {
		std::cout << plist[i] << std::endl;
	}

	for (int i=0; i<fn; i++) {
		for (int j=0; j<3; j++) {
			std::cout << flist[3*i+j] << " ";
		}
		std::cout << std::endl;
	}
}

Polygon::~Polygon()
{
}

void Polygon::getBounds(BoundingBox& bbox, double t) const
{
	for (int i=0; i<pn; i++) {
		bbox.extend(plist[i] + t * velocity);
	}
}

void Polygon::BetaGammaT(Point &beta_gamma_t, Vector &c1, Vector &c2, Vector &c3){
	
}

void Polygon::intersect(HitRecord& hit, const RenderContext&, const Ray& ray, double t) const
{
	for (int z = 0; z < fn; z++) {
		Point pa = plist[flist[3*z  ]] + t*velocity;
		Point pb = plist[flist[3*z+1]] + t*velocity;
		Point pc = plist[flist[3*z+2]] + t*velocity;
		
		double a = pa.x() - pb.x();
		double b = pa.y() - pb.y();
		double c = pa.z() - pb.z();
		
		double d = pa.x() - pc.x();
		double e = pa.y() - pc.y();
		double f = pa.z() - pc.z();
		
		double g = ray.direction().x();
		double h = ray.direction().y();
		double i = ray.direction().z();
		
		double j = pa.x() - ray.origin().x();
		double k = pa.y() - ray.origin().y();
		double l = pa.z() - ray.origin().z();
		
		double M = a*(e*i - h*f) + b*(g*f - d*i) + c*(d*h - e*g);
		double beta = (j*(e*i-h*f)+k*(g*f-d*i)+l*(d*h-e*g))/M;
		double gamma = (i*(a*k-j*b)+h*(j*c-a*l)+g*(b*l-k*c))/M;
		double tt = -(f*(a*k-j*b)+e*(j*c-a*l)+d*(b*l-k*c))/M;
		
	  if (beta>=0.0 && gamma>=0.0 && beta+gamma<=1.0) {
			hit.hit(tt, this, matl);
		}
	}
}

void Polygon::normal(Vector& normal, const RenderContext&, const Point& hitpos,
                    const Ray& ray, const HitRecord& hit, double t) const
{
	for (int z=0; z<fn; z++) {
		Point pa = plist[flist[3*z  ]] + t*velocity;
		Point pb = plist[flist[3*z+1]] + t*velocity;
		Point pc = plist[flist[3*z+2]] + t*velocity;
		
		double a = pa.x() - pb.x();
		double b = pa.y() - pb.y();
		double c = pa.z() - pb.z();
		double d = pa.x() - pc.x();
		double e = pa.y() - pc.y();
		double f = pa.z() - pc.z();
		double g = ray.direction().x();
		double h = ray.direction().y();
		double i = ray.direction().z();
		double j = pa.x() - ray.origin().x();
		double k = pa.y() - ray.origin().y();
		double l = pa.z() - ray.origin().z();
		
		double M = a*(e*i - h*f) + b*(g*f - d*i) + c*(d*h - e*g);
		
		double beta = (j*(e*i-h*f)+k*(g*f-d*i)+l*(d*h-e*g))/M;
		double gamma = (i*(a*k-j*b)+h*(j*c-a*l)+g*(b*l-k*c))/M;

		
		Vector vba = pb - pa;
		Vector vca = pc - pa;
		
		Point pp = pa + beta * vba + gamma * vca;
		Vector dist = pp - hitpos;
		
		if (dist.length()<0.00001) {
			normal = Cross(vba, vca);
			
			if (Dot(normal, ray.direction()) >0) {
				normal = -normal;
			}
			
			normal.normalize();
		}
	}
}
