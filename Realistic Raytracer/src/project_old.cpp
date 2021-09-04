/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2019.
 *
 * Do what you like with this code as long as you retain this comment.
 */

/* This is the entry point function for the program you need to create for lab two.
 * You should not need to modify this code.
 * It creates a framebuffer, loads an triangle mesh object, calls the drawing function to render the object and then outputs the framebuffer as a ppm file.
 *
 * On linux.bath.ac.uk:
 *
 * Compile the code using   g++ -o project project.cpp framebuffer.cpp polymesh.cpp directional_light.cpp phong.cpp scene.cpp sphere.cpp plane.cpp box.cpp -lm
 *                                      g++ -o project main_lab4.cpp framebuffer.cpp polymesh.cpp directional_light.cpp phong.cpp scene.cpp sphere.cpp plane.cpp box.cpp -lm
 *
 * Execute the code using ./lab2executable
 *
 * This will produce an image file called test.ppm. You can convert this a png file for viewing using
 *
 * pbmropng test.ppm > test.png
 *
 * You are expected to fill in the missing code in polymesh.cpp.
 */

#include "framebuffer.h"
#include "ray.h"
#include "hit.h"
#include "polymesh.h"
#include "sphere.h"
#include "light.h"
#include "directional_light.h"
#include "material.h"
#include "phong.h"
#include "plane.h"
#include "box.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

void object_test(Ray ray, Object *objects, Hit &best_hit)
{
  Object *obj = objects;

  best_hit.flag = false;


  while(obj != 0)
  {
    Hit obj_hit = Hit();
    obj_hit.flag=false;
	  
    obj->intersection(ray, obj_hit);

    
    if (obj_hit.flag)
    {
      if (obj_hit.t > 0.0f)
      {
        best_hit.override(obj_hit, false);
      }
    }
    
    obj = obj->next;
  }


  return;
}

void raytrace(Ray ray, Object *objects, Light *lights, Colour &colour, float &depth)
{
  // first step, find the closest primitive

  Hit shadow_hit;
  Hit best_hit;
  object_test(ray, objects, best_hit);


  // if we found a primitive then compute the colour we should see
  if(best_hit.flag)
  {
    best_hit.what->material->compute_base_colour(colour);
    depth = best_hit.t;
    Light *light = lights;


    while (light != (Light *)0)
    {
      Vector viewer;
      Vector ldir;

      viewer.x = -best_hit.position.x;
      viewer.y = -best_hit.position.y;
      viewer.z = -best_hit.position.z;
      viewer.normalise();

      bool lit;
      lit = light->get_direction(best_hit.position, ldir);

      if(ldir.dot(best_hit.normal)>0)
      {
	      lit=false;//light is facing wrong way.
      }

      if(lit)
      {
            
        Ray shadow_ray;

        shadow_ray.direction.x = -ldir.x;
        shadow_ray.direction.y = -ldir.y;
        shadow_ray.direction.z = -ldir.z;
        shadow_ray.position.x = best_hit.position.x + (0.0001f * shadow_ray.direction.x);
        shadow_ray.position.y = best_hit.position.y + (0.0001f * shadow_ray.direction.y);
        shadow_ray.position.z = best_hit.position.z + (0.0001f * shadow_ray.direction.z);



        object_test(shadow_ray, objects, shadow_hit);

        if(shadow_hit.flag==true)
        {
          if (shadow_hit.t < 1000000000.0f)
          {
            lit = false; //there's a shadow so no lighting, if realistically close
          }
        }
      }

      if (lit)
      {
        Colour intensity;
        Colour scaling;

        light->get_intensity(best_hit.position, scaling);

        best_hit.what->material->compute_light_colour(viewer, best_hit.normal, ldir, intensity);

              intensity.scale(scaling);

        colour.add(intensity);
      }

      light = light->next;
    }

    // TODO: compute reflection ray if material supports it.
    if(1)
    {
    }

    // TODO: compute refraction ray if material supports it.
    if(1)
    {
    }

  } else
  {
    depth = 7.0f;
    colour.r = 0.0f;
    colour.g = 0.0f;
    colour.b = 0.0f;
  }	

}

int main(int argc, char *argv[])
{



  int FRAME_BUFFER_SIZE;
  cout << "Enter Frame Buffer Size (default:512): ";
  cin >> FRAME_BUFFER_SIZE;
  cout << "Frame Buffer Size set to " << FRAME_BUFFER_SIZE << endl;

  int width = FRAME_BUFFER_SIZE;
  int height = FRAME_BUFFER_SIZE;
  // Create a framebuffer
  FrameBuffer *fb = new FrameBuffer(width,height);



  // The following transform allows 4D homogeneous coordinates to be transformed. It moves the supplied teapot model to somewhere visible.
  Transform *transform = new Transform(1.0f, 0.0f, 0.0f,  0.0f,
				       0.0f, 0.0f, 1.0f, -2.7f,
				       0.0f, 1.0f, 0.0f, 5.0f,
				       0.0f, 0.0f, 0.0f, 1.0f);

  //  Read in the teapot model.
  PolyMesh *pm = new PolyMesh((char *)"teapot_smaller.ply", transform);
  pm->name = "Teapot";


  Vertex v;
  v.x = 0.0f;
  v.y = 0.0f;
  v.z = 5.0f;
  Sphere *sphere = new Sphere(v, 1.0f);
  sphere->name = "Sphere 1";


  Vertex v2;
  v2.x = -1.0f;
  v2.y = 1.0f;
  v2.z = 3.0f;
  Sphere *sphere2 = new Sphere(v2,0.5f);
  sphere2->name = "Sphere 2";


  Vertex c0 = Vertex(4, -2.7, 6);
  Vertex c1 = Vertex(5, -0.5, 7.5);
  Box *box1 = new Box(c0, c1);
  box1->name = "Box 1";


  Plane *plane1 = new Plane(Vertex(0, -2.7, 0), Vector(0, 1, 0));
  plane1->name = "Plane 1";


  Ray ray;

  ray.position.x = 0.0001f;
  ray.position.y = 0.0f;
  ray.position.z = 0.0f;



  DirectionalLight *dl = new DirectionalLight(Vector(1.01f, -1.0f, 1.0f),Colour(1.0f, 1.0f, 1.0f, 0.0f));


  Phong bp1;
	bp1.ambient.r = 0.2f;
	bp1.ambient.g = 0.0f;
	bp1.ambient.b = 0.0f;
	bp1.diffuse.r = 0.4f;
	bp1.diffuse.g = 0.0f;
	bp1.diffuse.b = 0.0f;
	bp1.specular.r = 0.4f;
	bp1.specular.g = 0.4f;
	bp1.specular.b = 0.4f;
	bp1.power = 40.0f;


  Phong bp2;
  bp2.ambient.r = 0.0f;
	bp2.ambient.g = 0.2f;
	bp2.ambient.b = 0.0f;
	bp2.diffuse.r = 0.0f;
	bp2.diffuse.g = 0.4f;
	bp2.diffuse.b = 0.0f;
	bp2.specular.r = 0.4f;
	bp2.specular.g = 0.4f;
	bp2.specular.b = 0.4f;
	bp2.power = 40.0f;


  Phong bp3;
  bp3.ambient.r = 0.1f;
	bp3.ambient.g = 0.2f;
	bp3.ambient.b = 0.8f;
	bp3.diffuse.r = 0.1f;
	bp3.diffuse.g = 0.4f;
	bp3.diffuse.b = 0.8f;
	bp3.specular.r = 0.4f;
	bp3.specular.g = 0.4f;
	bp3.specular.b = 0.4f;
	bp3.power = 40.0f;


  Phong bp4;
  bp4.ambient.r = 0.5f;
	bp4.ambient.g = 0.2f;
	bp4.ambient.b = 0.3f;
	bp4.diffuse.r = 0.7f;
	bp4.diffuse.g = 0.4f;
	bp4.diffuse.b = 0.3f;
	bp4.specular.r = 0.4f;
	bp4.specular.g = 0.4f;
	bp4.specular.b = 0.4f;
	bp4.power = 40.0f;



  pm->setMaterial(bp1);
	sphere->setMaterial(bp2);
 	sphere2->setMaterial(bp2);
  box1->setMaterial(bp3);
  plane1->setMaterial(bp4);



	//pm->next = sphere2;
  //pm->next->next = plane1;
  //pm->next->next->next = box1;



  int PercentComplete = 0;
 
  for (int y = 0; y < height; y += 1)
  {
    for (int x = 0; x < width; x += 1)
    {
      float fx = (float)x/(float)width;
      float fy = (float)y/(float)height;

      Vector direction;

      ray.direction.x = (fx-0.5f);
      ray.direction.y = (0.5f-fy);
      ray.direction.z = 0.5f;
      ray.direction.normalise();

      Colour colour;
      float depth;

      raytrace(ray, pm, dl, colour, depth);

      fb->plotPixel(x, y, colour.r, colour.g, colour.b);
      fb->plotDepth(x,y, depth);

    }

    if ((100*y)/FRAME_BUFFER_SIZE > PercentComplete)
    {
      PercentComplete = (100*y)/FRAME_BUFFER_SIZE;
      cout << PercentComplete-1 << "% complete." << endl;
    }

  }
  
  cout << "Polymesh Hitbox misses: " << pm->hitbox_misses << endl;

  // Output the framebuffer.
  fb->writeRGBFile((char *)"test.ppm");
  //  fb->writeDepthFile((char *)"depth.ppm");
  return 0;
  
}
