
#include "Scene.h"
#include "Background.h"
#include "Camera.h"

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
	
	SSAA = 1;
	DOF_ray = 1;
	MotionBlurSample = 1;
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

inline double randMotionTimeGaussian(double start_time, double end_time){
	double sigma = (end_time - start_time) / 4;
	double mu = (end_time + 2 * start_time )/3.0;
	double u1 = drand48();
	double u2 = drand48();
	
	double z1 = sqrt(-2 * log(u1)) * sin(2 * M_PI * u2);
//	double z2 = sqrt(-2 * log(u1)) * cos(2 * M_PI * u2);
	
	return mu + z1 * sigma;
	//x2 = mu + z2 * sigma;
}

inline double randMotionTime(double start_time, double end_time){	
	return start_time + drand48() * (end_time - start_time);
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
	
	// loop over all image pixel
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
						Color result_motion(0.0, 0.0, 0.0);
						for (int mbt = 0; mbt < MotionBlurSample; mbt++) {

							double motion_time = randMotionTimeGaussian(camera->start_time, camera->end_time);

							camera->makeRay(ray, context, xp, yp);
							HitRecord hit(DBL_MAX);
							object->intersect(hit, context, ray, motion_time);
							Color result;
							if(hit.getPrimitive()){
								// Ray hit something...
								const Material* matl = hit.getMaterial();
								matl->shade(result, context, ray, hit, atten, 0, motion_time);
							} else {
								background->getBackgroundColor(result, context, ray);
							}
							result_motion += result/(double)(MotionBlurSample);
						}
						
            result_dof += result_motion/(double)(DOF_ray);

          }

          result_sum += result_dof/(double)(SSAA*SSAA);	

        }	
      }

      image->set(j, i, result_sum);
    }
  } // end of for (x,y)
}

double Scene::traceRay(Color& result, const RenderContext& context, const Object* obj, const Ray& ray, const Color& atten, int depth, double motion_time) const
{
  if(depth >= maxRayDepth || atten.maxComponent() < minAttenuation){
    result = Color(0, 0, 0);
    return 0;
  } else {
    HitRecord hit(DBL_MAX);
    obj->intersect(hit, context, ray, motion_time);
    if(hit.getPrimitive()){
      // Ray hit something...
      const Material* matl = hit.getMaterial();
      matl->shade(result, context, ray, hit, atten, depth, motion_time);
      return hit.minT();
    } else {
      background->getBackgroundColor(result, context, ray);
      return DBL_MAX;
    }
  }
}

double Scene::traceRay(Color& result, const RenderContext& context, const Ray& ray, const Color& atten, int depth, double motion_time) const
{
  if(depth >= maxRayDepth || atten.maxComponent() < minAttenuation){
    result = Color(0, 0, 0);
    return 0;
  } else {
    HitRecord hit(DBL_MAX);
    object->intersect(hit, context, ray, motion_time);
    if(hit.getPrimitive()){
      // Ray hit something...
      const Material* matl = hit.getMaterial();
      matl->shade(result, context, ray, hit, atten, depth, motion_time);
      return hit.minT();
    } else {
      background->getBackgroundColor(result, context, ray);
      return DBL_MAX;
    }
  }
}
