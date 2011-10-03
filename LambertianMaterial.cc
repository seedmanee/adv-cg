// modifed

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
  :color(color), Kd(Kd), Ka(Ka), Ks(Ks), alpha(alpha)
{
}

LambertianMaterial::~LambertianMaterial()
{
}

void LambertianMaterial::shade(Color& result, const RenderContext& context,
                               const Ray& ray, const HitRecord& hit, const Color& atten, int depth) const
{
  const Scene* scene = context.getScene();
	
	if ( depth >= scene->getMaxRayDepth() || atten.maxComponent() < scene->getMinAttenuation()){
		result = Color(0.0, 0.0, 0.0);
	} else {

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
		Color Is(0.0, 0.0, 0.0);
		Color bounce(0.0, 0.0, 0.0);
		
		// each light directly contribute to color of hitpos
		Light*const* begin = &lights[0];
		Light*const* end = &lights[0]+lights.size();
		Color recursive_result(0.0, 0.0, 0.0);
		
		while(begin != end){
			
			Color light_color;
			Vector light_direction;
			Vector reflection_direction;
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
					reflection_direction = 2 * Dot(light_direction, normal) * normal - light_direction;
					Is += light_color * Ks * pow( max(Dot(reflection_direction, -ray.direction()), 0.0), alpha);
				}
			
				Ray reflection_ray(hitpos, reflection_direction);
				HitRecord reflection_hit(DBL_MAX);
				scene->getObject()->intersect(reflection_hit, context, reflection_ray);
	
				if (reflection_hit.getPrimitive()) {
					shade(recursive_result, context, reflection_ray, reflection_hit, atten * 0.75, depth + 1);
				}
			}
		} // end of while(begin != end)
		result = light * color + Is + recursive_result * 0.75;
	}
}