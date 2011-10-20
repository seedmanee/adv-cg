
#include "Scene.h"
#include "Background.h"
#include "Camera.h"

#include "AperatureCamera.h"

#include "HitRecord.h"
#include "Image.h"
#include "Light.h"
#include "Material.h"
#include "Object.h"
#include "Ray.h"
#include "RenderContext.h"
#include "Primitive.h"

#include <float.h>
#include <iostream>
#include <stdlib.h>
using namespace std;

Scene::Scene()
{
  object = 0;
  background = 0;
  camera = 0;
  ambient = Color(0, 0, 0);
  image = 0;
	
  minAttenuation = 0.1;
	
	maxRayDepth = 10;
	
	SSAA = 1;  // super sampling anti aliasing
}

Scene::~Scene()
{
  delete object;
  delete background;
  delete camera;
  delete image;
  for(int i=0;i<static_cast<int>(lights.size());i++){
    Light* light = lights[i];
    delete light;
  }
}

void Scene::preprocess()
{
  background->preprocess();
  for(int i=0;i<static_cast<int>(lights.size());i++){
    Light* light = lights[i];
    light->preprocess();
  }
  double aspect_ratio = image->aspect_ratio();
  camera->preprocess(aspect_ratio);
  object->preprocess();
}

void Scene::render()
{
	if(!object || !background || !camera || !image){
		cerr << "Incomplete scene, cannot render!\n";
		exit(1);
	}
	int xres = image->getXresolution();
	int yres = image->getYresolution();
	RenderContext context(this);
	double dx = 2./xres;
	double xmin = -1. + dx/2.;
	double dy = 2./yres;
	double ymin = -1. + dy/2.;
	Color atten(1,1,1);
	
	double ddx = dx / (double)SSAA;  // super sampling grid step
	double ddy = dy / (double)SSAA;
	
	int DOF_ray = 1;
	
	double motion_time = -1.0;
	while (motion_time < 2.0){
		
		for(int i=0;i<yres;i++){
			double y = ymin + i*dy;
			
			for(int j=0;j<xres;j++){
				double x = xmin + j*dx;
				
				Ray ray;
				Color result_sum(0.0, 0.0, 0.0);

				for (int superx = 0; superx < SSAA; superx++) {
					for (int supery = 0; supery < SSAA; supery++) {

						// these is for anti-alaising
						double xp = x + (superx - SSAA/2) * ddx + ddx * drand48();
						double yp = y + (supery - SSAA/2) * ddy + ddy * drand48();
						
						Color result_dof(0.0, 0.0, 0.0);
						for (int r=0; r<DOF_ray; r++) {

							camera->makeRay(ray, context, xp, yp);
							HitRecord hit(DBL_MAX);
							object->intersect(hit, context, ray);
							Color result;
							if(hit.getPrimitive()){
								// Ray hit something...
								const Material* matl = hit.getMaterial();
								matl->shade(result, context, ray, hit, atten, 0);
							} else {
								background->getBackgroundColor(result, context, ray);
							}
							result_dof += result/(double)(DOF_ray);	
					
						}
						
						result_sum += result_dof/(double)(SSAA*SSAA);	

					}	
				}
				
				image->set(j, i, result_sum);
			}
		} // end of for (x,y)
	}
}

double Scene::traceRay(Color& result, const RenderContext& context, const Object* obj, const Ray& ray, const Color& atten, int depth) const
{
  if(depth >= maxRayDepth || atten.maxComponent() < minAttenuation){
    result = Color(0, 0, 0);
    return 0;
  } else {
    HitRecord hit(DBL_MAX);
    obj->intersect(hit, context, ray);
    if(hit.getPrimitive()){
      // Ray hit something...
      const Material* matl = hit.getMaterial();
      matl->shade(result, context, ray, hit, atten, depth);
      return hit.minT();
    } else {
      background->getBackgroundColor(result, context, ray);
      return DBL_MAX;
    }
  }
}

double Scene::traceRay(Color& result, const RenderContext& context, const Ray& ray, const Color& atten, int depth) const
{
	if(depth >= maxRayDepth || atten.maxComponent() < minAttenuation){
		result = Color(0, 0, 0);
		return 0;
	} else {
		HitRecord hit(DBL_MAX);
		object->intersect(hit, context, ray);
		if(hit.getPrimitive()){
				// Ray hit something...
				const Material* matl = hit.getMaterial();
				matl->shade(result, context, ray, hit, atten, depth);
				return hit.minT();
		} else {
				background->getBackgroundColor(result, context, ray);
				return DBL_MAX;
		}
	}
}