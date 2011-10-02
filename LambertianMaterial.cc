
#include "LambertianMaterial.h"
#include "HitRecord.h"
#include "Light.h"
#include "Point.h"
#include "Primitive.h"
#include "Ray.h"
#include "RenderContext.h"
#include "Scene.h"
#include "Vector.h"
#include "Math.h"
using namespace std;

LambertianMaterial::LambertianMaterial(const Color& color, float Kd, float Ka)
  :color(color), Kd(Kd), Ka(Ka)
{
}

LambertianMaterial::~LambertianMaterial()
{
}

void LambertianMaterial::shade(Color& result, const RenderContext& context,
                               const Ray& ray, const HitRecord& hit, const Color&, int) const
{
  const Scene* scene = context.getScene();
  const vector<Light*>& lights = scene->getLights();
  Point hitpos = ray.origin()+ray.direction()*hit.minT();	// ray hit object at Point hitpos
  Vector normal;
  hit.getPrimitive()->normal(normal, context, hitpos, ray, hit);	// normal at hitpos
	
  double costheta = Dot(normal, ray.direction());
	
	// make sure the direction of normal facing the way we can see
  if(costheta > 0)
    normal = -normal;

  const Object* world = scene->getObject();

  Color light = scene->getAmbient()*Ka;				// ambient term Ia = La * Ka

	double Ks = 0.7;
	double alpha = 80;
	
	// each light contribute to color of hitpos
	Light*const* begin = &lights[0];
	Light*const* end = &lights[0]+lights.size();
	
	while(begin != end){
		
    Color light_color;
    Vector light_direction;
		
		// distance from light source to hitpos
    double dist = (*begin++)->getLight(light_color, light_direction, context, hitpos);  
    
		double cosphi = Dot(normal, light_direction);

		// if light is above horizon
    if(cosphi > 0){
      // Cast shadow rays...
      HitRecord shadowhit(dist);
      Ray shadowray(hitpos, light_direction);
      world->intersect(shadowhit, context, shadowray);	// light is not an object
      if(!shadowhit.getPrimitive())
			{
        // No shadows...		
				// diffuse term Id = Ld * Kd * dot(light, normal)
        light += light_color*(Kd*cosphi);
				
				// specular term Is = Ls * Ks * cos^alpha (phi)				
				Vector reflection = 2 * Dot(light_direction, normal) * normal - light_direction;
				light += light_color * Ks * pow( max(Dot(reflection, -ray.direction()), 0.0), alpha);
			}
    }
  } // end of while(begin != end)
  result = light*color;
}