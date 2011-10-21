
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
#include <float.h>
using namespace std;

LambertianMaterial::LambertianMaterial(const Color& color, float Kd, float Ka, float Ks, float alpha)
  :color(color), Kd(Kd), Ka(Ka), Ks(Ks), alpha(alpha), DRT_number(1)
{
}

LambertianMaterial::~LambertianMaterial()
{
}

void LambertianMaterial::shade(Color& result, const RenderContext& context,
                               const Ray& ray, const HitRecord& hit, const Color& atten, int depth, double motion_time) const
{
  const Scene* scene = context.getScene();
  const vector<Light*>& lights = scene->getLights();
  Point hitpos = ray.origin()+ray.direction()*hit.minT(); // ray hit object at point hitpos
  Vector normal;
  hit.getPrimitive()->normal(normal, context, hitpos, ray, hit); // normal at hitpos
  double costheta = Dot(normal, ray.direction());
  // make sure the normal of surface you hit is facing outward
	if(costheta > 0)
    normal = -normal;
	
  const Object* world = scene->getObject();
	
	// ambient term Ia = La * Ka
  Color light = scene->getAmbient()*Ka;
	
	Color Is(0.0, 0.0, 0.0);
	
	const double atten_factor = 0.8;
	Vector reflection_ray_direction = 2 * Dot(-ray.direction(), normal) * normal + ray.direction();
	Ray reflection_ray(hitpos, reflection_ray_direction);

	Light*const* begin = &lights[0];
	Light*const* end = &lights[0]+lights.size();
	
	while(begin != end){   // loop over all light source

		Color light_color;
		Vector light_direction;
		double dist = (*begin++)->getLight(light_color, light_direction, context, hitpos);
		double cosphi = Dot(normal, light_direction);
		
		if(cosphi > 0){
			// Cast shadow rays...
			HitRecord shadowhit(dist);
			Ray shadowray(hitpos, light_direction);
			world->intersect(shadowhit, context, shadowray, motion_time);
			if(!shadowhit.getPrimitive())
			{
				// No shadows...
				// diffuse term Id = Ld * Kd * cos(theta)
				light += light_color*(Kd*cosphi);
				
				Vector reflection_light_direction = 2 * Dot(light_direction, normal) * normal - light_direction;

				// specular term Is = Ls * Ks * cos^alpha (phi)
				Is += light_color * Ks * pow( max(Dot(reflection_light_direction, -ray.direction()), 0.0), alpha);
			}
		}
	} // end of while(begin != end)
	
	// add recursive part
	Color reflection_result;
	context.getScene()->traceRay(reflection_result, context, reflection_ray, atten * atten_factor, depth + 1, motion_time);
	
	Color reflection_diffuse = reflection_result * atten_factor * Kd * Dot(normal, reflection_ray_direction) ;
	Color reflection_specular = reflection_result * atten_factor * Ks * pow( max(Dot(reflection_ray_direction, -ray.direction()), 0.0), alpha);
	
	result = (light + reflection_diffuse) *color + Is + reflection_specular;

}