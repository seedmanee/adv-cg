
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

void Polygon::BetaGammaT(Point &beta_gamma_t,const Vector &c1,const Vector &c2,const Vector &c3,const Vector &y) const{
	
	double a = c1.x();
	double b = c1.y();
	double c = c1.z();
	
	double d = c2.x();
	double e = c2.y();
	double f = c2.z();
	
	double g = c3.x();
	double h = c3.y();
	double i = c3.z();
	
	double j = y.x();
	double k = y.y();
	double l = y.z();
	
	double M = a*(e*i - h*f) + b*(g*f - d*i) + c*(d*h - e*g);
	double beta = (j*(e*i-h*f)+k*(g*f-d*i)+l*(d*h-e*g))/M;
	double gamma = (i*(a*k-j*b)+h*(j*c-a*l)+g*(b*l-k*c))/M;
	double tt = -(f*(a*k-j*b)+e*(j*c-a*l)+d*(b*l-k*c))/M;
	
	beta_gamma_t.set(beta, gamma, tt);
}

void Polygon::intersect(HitRecord& hit, const RenderContext&, const Ray& ray, double t) const
{
	for (int z = 0; z < fn; z++) {
		Point pa = plist[flist[3*z  ]] + t*velocity;
		Point pb = plist[flist[3*z+1]] + t*velocity;
		Point pc = plist[flist[3*z+2]] + t*velocity;
	
		Point result;
		
		BetaGammaT(result, pa-pb, pa-pc, ray.direction(), pa-ray.origin());
		
	  if (result.x()>=0.0 && result.y()>=0.0 && result.x()+result.y()<=1.0) {
			hit.hit(result.z(), this, matl);
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
		
		Vector vba = pb - pa;
		Vector vca = pc - pa;
		
		Point result;
		
		BetaGammaT(result, -vba, -vca, ray.direction(), pa-ray.origin());
		
		Point pp = pa + result.x() * vba + result.y() * vca;
		Vector dist = pp - hitpos;
		
		if (dist.length()<0.00001) {
			normal = Cross(vba, vca);
			
			if (Dot(normal, ray.direction()) > 0) {
				normal = -normal;
			}
			
			normal.normalize();
		}
	}
}
