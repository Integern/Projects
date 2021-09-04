/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2019.
 *
 * Do what you like with this code as long as you retain this comment.
 */

/* This is the entry point function for the program you need to create for lab two.
 * You should not need to modify this code.
 * It creates a framebuffer, loads an triangle mesh object, calls the drawing function to render the object and then outputs the framebuffer as a pteapot file.
 *
 * On linux.bath.ac.uk:
 *
 * Compile the code using       g++ -o project project.cpp framebuffer.cpp photon_map.cpp polymesh.cpp phong.cpp scene.cpp sphere.cpp plane.cpp reflectrefract.cpp -lm
 *
 * Execute the code using project.exe
 *
 * This will produce an image file called test.ppm. You can convert this a png file for viewing using
 *
 * pbmropng test.ppm > test.png
 *
 * You are expected to fill in the missing code in polymesh.cpp.
 */

/*

    Joel Broad 2020

    added reflection/refraction
    added CSG (an object can be union'd or negated with/by another object)
    added photon mapping

*/

#include "framebuffer.h"
#include "ray.h"
#include "hit.h"
#include "polymesh.h"
#include "sphere.h"
#include "light.h"
#include "directional_light.h"
#include "point_light.h"
#include "material.h"
#include "phong.h"
#include "box.h"
#include "reflectrefract.h"
#include "photon.h"
#include "photon_map.h"
#include "photon_list.h"
#include "settings.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;


int max_reflection_depth = 5; //how many times to bounce rays around before giving up
int max_refractive_depth = 5;



void object_test(Ray ray, Object *objects, Hit &best_hit, bool doing_shadow_checks)
{
  float epsilon = 0.0001f;
  Object *obj = objects;

  best_hit.flag = false;

  while(obj != 0)
  {
    Hit obj_hit = Hit();
    obj->intersection(ray, obj_hit);



    //now check if we're hitting it from the inside (will have to nullify in certain situations)
    Hit obj_hit_inside = Hit();
    Ray ray_inverse = Ray(ray.position, ray.direction);
    ray_inverse.direction.negate();
    obj->intersection(ray_inverse, obj_hit_inside);


    if (doing_shadow_checks) // we don't want to give hits inside objects when we're checking for shadows!
    {
      if (obj_hit_inside.flag && obj_hit.flag && obj_hit.t > 0 && obj_hit_inside.t > 0)
      {
        obj_hit.flag = false;
      }
    }
  


    if (obj->unionObject != 0) //if we hit our union object, and can override our object hit, do so and make sure to update the what field
    {
      Hit union_obj_hit = Hit();
      obj->unionObject->intersection(ray, union_obj_hit);

      obj_hit.override(union_obj_hit, false);
      if (obj_hit.what == obj->unionObject) obj_hit.what = obj;
    }


    if ( (obj->negateObject != 0) && (obj_hit.flag == true) ) //if we hit our object, and have a negate object.
    {
      Ray temp_ray = Ray();

      temp_ray.direction = ray.direction;
      temp_ray.position = ray.getPosition(0.0f);


      Hit obj_hit_in = Hit();
      obj_hit_in.override(obj_hit, true);
    
      Hit obj_hit_out = Hit();
      temp_ray.position = ray.getPosition(obj_hit_in.t + epsilon);
      object_test(temp_ray, obj, obj_hit_out, doing_shadow_checks);
      if (obj_hit_out.flag) obj_hit_out.t = obj_hit_out.t + (obj_hit_in.t + epsilon);



      Hit negate_hit_in = Hit();
      temp_ray.position = ray.getPosition(0.0f);
      object_test(temp_ray, obj->negateObject, negate_hit_in, doing_shadow_checks);

      Hit negate_hit_out = Hit();
      if (negate_hit_in.flag == true) //if we've hit a negate object, see where the ray exits.
      {
        temp_ray.position = ray.getPosition(negate_hit_in.t + epsilon);
        object_test(temp_ray, obj->negateObject, negate_hit_out, doing_shadow_checks);
        if (negate_hit_out.flag) negate_hit_out.t = negate_hit_out.t + (negate_hit_in.t + epsilon);
      }




      if (obj_hit_in.flag && obj_hit_out.flag && negate_hit_in.flag && negate_hit_out.flag)
      {
        /// primitive is closer OR object finishes before negat starts OR object starts after negate finishes
        if ( (obj_hit_in.t < negate_hit_in.t) || (obj_hit_out.t < negate_hit_in.t) || (obj_hit_in.t > negate_hit_out.t) ) { //no conflict
          //take primitive
        } else if (obj_hit_in.t < negate_hit_out.t) { // collide, object further away

          if (obj_hit_out.t < negate_hit_out.t) { // negate goes all the way through object

            temp_ray.position = ray.getPosition(obj_hit_out.t + epsilon);
            obj_hit.flag = false;

          } else { // stops inside object (normal from negate object!)

            obj_hit.override(negate_hit_out, true);
            obj_hit.normal.negate(); //we're hitting the inside of the negation object, want to treat it as outside
          }

        } else if (obj_hit_out.t > negate_hit_in.t) { // negate further away aka primative is closer, should be illegal
          cout << "REEEEE ILLEGAL" << endl;
        }

      }

      obj_hit.what = obj;

    }



    //override if it is a better hit and 1) we're ignoring occlusions 2) object occludes light
    if (obj_hit.flag && (doing_shadow_checks == false || (doing_shadow_checks == true && obj_hit.what->occlude_light == true)) )
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




void raytrace_shadowcheck(Vector ldir, Vertex lpos, Hit last_hit, Object *objects, Hit &shadow_hit, Colour &lit_colour, bool &lit)
{

  Ray shadow_ray;

  shadow_ray.direction.x = -ldir.x;
  shadow_ray.direction.y = -ldir.y;
  shadow_ray.direction.z = -ldir.z;
  shadow_ray.position.x = last_hit.position.x + (0.0001f * shadow_ray.direction.x);
  shadow_ray.position.y = last_hit.position.y + (0.0001f * shadow_ray.direction.y);
  shadow_ray.position.z = last_hit.position.z + (0.0001f * shadow_ray.direction.z);

  shadow_hit.flag = false;
  object_test(shadow_ray, objects, shadow_hit, true);

  //is shadowed and object its shadowed by is closer than the position of the light
  if((shadow_hit.flag==true))
  {
    if ( (shadow_hit.position - last_hit.position).len() <= (lpos - last_hit.position).len() )
    {
      if (shadow_hit.t < 1000000000.0f)
      {

        //there's a shadow so no lighting, if realistically close
        if (shadow_hit.what->reflectrefract->refraction.rgbSum() == 0.0f)
        {
          lit = false; //material it hit is opaque so blocks all light
        } else {
          lit_colour.r = lit_colour.r * shadow_hit.what->reflectrefract->refraction.r; //if refraction value is 1, is transparent
          lit_colour.g = lit_colour.g * shadow_hit.what->reflectrefract->refraction.g;
          lit_colour.b = lit_colour.b * shadow_hit.what->reflectrefract->refraction.b;
        }
        
      }
    } else shadow_hit.flag = true; //light is closer, so definitely no shadow.
  }

}


void raytrace(Ray ray, Object *objects, Light *lights, Colour &colour, float &depth, int reflection_depth, int refractive_depth, PhotonMap *caustic, PhotonMap *global, Settings settings)
{
  // first step, find the closest primitive

  Hit shadow_hit;
  Hit best_hit;
  object_test(ray, objects, best_hit, false);


  // if we found a primitive then compute the colour we should see
  if(best_hit.flag)
  {

    best_hit.what->material->compute_base_colour(colour);
    if (true) // reduce ambient contribution, let photons contribute more to emphasise bleeding.
    {
      colour.r = colour.r * settings.ambient_contribution;
      colour.g = colour.g * settings.ambient_contribution;
      colour.b = colour.b * settings.ambient_contribution;
    }


    depth = best_hit.t;
    Light *light = lights;


    while (light != (Light *)0)
    {
      Vector viewer;
      Vector ldir;
      Vertex lpos;

      viewer.x = -best_hit.position.x;
      viewer.y = -best_hit.position.y;
      viewer.z = -best_hit.position.z;
      viewer.normalise();

      bool lit;
      lit = light->get_direction(best_hit.position, ldir);
      light->get_position(best_hit, lpos);

      Colour lit_colour = Colour(1.0f, 1.0f, 1.0f, 1.0f);

      if(ldir.dot(best_hit.normal)>0)
      {
	      lit=false;//light is facing wrong way.
      }



      if(lit) //shadow checks
      {

        raytrace_shadowcheck(ldir, lpos, best_hit, objects, shadow_hit, lit_colour, lit);
        if (lit)
        {
          while (true) //we might have got a shadow, but through a transluscent object.
          {
            if (shadow_hit.flag == true)
            {
              raytrace_shadowcheck(ldir, lpos, shadow_hit, objects, shadow_hit, lit_colour, lit);
            } else {
              break;
            }
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

        intensity.r = intensity.r * lit_colour.r;
        intensity.g = intensity.g * lit_colour.g;
        intensity.b = intensity.b * lit_colour.b;

        colour.add(intensity); //add to original colour
      }

      light = light->next;
    }


    
    // TODO: compute reflection ray if material supports it.
    if( (best_hit.what->reflectrefract->reflection.rgbSum() > 0.0f) && (reflection_depth <= max_reflection_depth) ) //if we have any kind of reflection in any colour
    {
      reflection_depth++;


      Vector reflection = Vector();
      best_hit.what->reflectrefract->compute_reflection_direction(ray, best_hit.normal, reflection);

      Ray reflected_ray = Ray();
      reflected_ray.position = ray.getPosition(best_hit.t - 0.0001f); //ensures we dont reflect inside the object
      reflected_ray.direction = reflection;



      Colour reflection_colour = Colour();
      raytrace(reflected_ray, objects, lights, reflection_colour, depth, reflection_depth+1, refractive_depth, caustic, global, settings);
      reflection_colour.scale(best_hit.what->reflectrefract->reflection);


      colour.scale(best_hit.what->reflectrefract->reflection_inverse);
      colour.add(reflection_colour);
    }


    
    // TODO: compute refraction ray if material supports it.
    if( (best_hit.what->reflectrefract->refraction.rgbSum() > 0.0f) && (refractive_depth <= max_refractive_depth) )
    {
      refractive_depth++;

      float n_out = 1.0f; //air
      float n_in = best_hit.what->reflectrefract->refractive_index;

      Ray refract_ray = Ray();
      refract_ray.position = ray.getPosition(best_hit.t + 0.00001f);
      best_hit.normal.refraction(ray.direction, n_in/n_out, refract_ray.direction);
      if (true)
      {
        // Got ray inside object, now to find exit point.
        Hit refract_exit = Hit();
        best_hit.what->intersection(refract_ray, refract_exit);

        if (refract_exit.flag)
        {
            
          Colour refracted_colour = Colour();
          Ray ray_new = Ray();
          best_hit.normal.refraction(refract_ray.direction, n_out/n_in, ray_new.direction);
          ray_new.position = refract_ray.getPosition(refract_exit.t + 0.00001f);

          raytrace(ray_new, objects, lights, refracted_colour, depth, reflection_depth, refractive_depth, caustic, global, settings);

          refracted_colour.scale(best_hit.what->reflectrefract->refraction);
          colour.scale(best_hit.what->reflectrefract->refraction_inverse);

          colour.add(refracted_colour);


        } else cout << "Refraction ray didn't exit object" << endl;

      } else cout << "Total internal reflection" << endl;


    }


    //photon calculations

    Colour col_GPC = Colour(); //global_photon_contributions
    Colour col_CPC = Colour(); //caustic_photon_contributions

    PhotonList *photon_list = new PhotonList();
    Photon *current;


    if (settings.search_using_radius)
    {

      if (settings.search_using_radius_fast) 
      {
        global->search(photon_list, best_hit.position, settings.search_radius);
      } else {
        global->search_slow(photon_list, best_hit.position, settings.search_radius);
      }
      

      float global_flux_f = float(settings.photon_global_flux) / (3.141*pow(settings.search_radius, 2)); //divide by area of sphere

      current = photon_list->head;
      while (current != 0 )
      {

        //weight photon contributions differently based on distance from origin, and if to compensate if there is more/less photon than expected if we want
        float weight = 1 - (photon_list->dist(best_hit.position, current) / settings.search_radius);
        weight = pow(weight, settings.search_radius_closeness_scaling);
        weight = weight + settings.search_radius_neighbour_baseline / photon_list->length;

        if ((photon_list->shadow_count > 0)) weight = weight/settings.shadow_divide_effect; // Temporary
        

        col_GPC.r = col_GPC.r + current->intensity.r * global_flux_f * weight;
        col_GPC.g = col_GPC.g + current->intensity.g * global_flux_f * weight;
        col_GPC.b = col_GPC.b + current->intensity.b * global_flux_f * weight;

        current = current->next;
      }





      photon_list->clear();


      if (settings.search_using_radius_fast) 
      {
        caustic->search(photon_list, best_hit.position, settings.search_radius);
      } else {
        caustic->search_slow(photon_list, best_hit.position, settings.search_radius);
      }

      float caustic_flux_f = float(settings.photon_caustic_flux) / (3.141*pow(settings.search_radius, 2));

      current = photon_list->head;
      while (current != 0 )
      {

        float weight = 1 - (photon_list->dist(best_hit.position, current) / settings.search_radius);
        weight = pow(weight, settings.search_radius_closeness_scaling);
        weight = weight + settings.search_radius_neighbour_baseline / photon_list->length;

        if ((photon_list->shadow_count > 0)) weight = weight/settings.shadow_divide_effect; // Temporary


        //cout << "photon " << current->position << "intensity: " << current->intensity << endl;
        col_CPC.r = col_CPC.r + current->intensity.r * caustic_flux_f * weight;
        col_CPC.g = col_CPC.g + current->intensity.g * caustic_flux_f * weight;
        col_CPC.b = col_CPC.b + current->intensity.b * caustic_flux_f * weight;

        current = current->next;
      }

      

    } else {

      cerr << "not updated" << endl;
/*
      global->search_nearest(photon_list, best_hit.position, settings.search_neighbour_count);
      float global_flux_f = float(settings.photon_global_flux) / photon_list->getBoundingSphereArea();
      //cout << "global sphere area: " << photon_list->getBoundingSphereArea() << endl;

      current = photon_list->head;
      while (current != 0 )
      {
        //cout << "photon " << current->position << "intensity: " << current->intensity << endl;
        col_GPC.r = col_GPC.r + current->intensity.r * global_flux_f;
        col_GPC.g = col_GPC.b + current->intensity.r * global_flux_f;
        col_GPC.b = col_GPC.r + current->intensity.r * global_flux_f;

        current = current->next;
      }


      photon_list->clear();
      caustic->search_nearest(photon_list, best_hit.position, settings.search_neighbour_count);
      float caustic_flux_f = float(settings.photon_caustic_flux) / photon_list->getBoundingSphereArea();
      //cout << "caustic sphere area: " << photon_list->getBoundingSphereArea() << endl;

      current = photon_list->head;
      while (current != 0 )
      {
        //cout << "photon " << current->position << "intensity: " << current->intensity << endl;
        col_CPC.r = col_CPC.r + current->intensity.r * caustic_flux_f;
        col_CPC.g = col_CPC.b + current->intensity.r * caustic_flux_f;
        col_CPC.b = col_CPC.r + current->intensity.r * caustic_flux_f;

        current = current->next;
      }

      //cout << "colour before: " << colour << endl;
      //cout << "global: " << col_GPC << "  caustic: " << col_CPC << endl;

     // colour.add(col_GPC);
     // colour.add(col_CPC);

      //cout << "colour after: " << colour << endl;

*/
    }

    colour.add(col_GPC);
    colour.add(col_CPC);


    

  } else
  {
    depth = 7.0f;
    colour.r = 0.0f;
    colour.g = 0.0f;
    colour.b = 0.0f;
  }	

  
  //Plotting photons
  if (settings.do_plot_photons && best_hit.flag)
  {
    
    PhotonList *photonlist = new PhotonList();
    global->search(photonlist, best_hit.position, settings.plot_photons_radius);

    if (photonlist->head == 0) caustic->search(photonlist, best_hit.position, settings.plot_photons_radius);

    if (photonlist->head != 0)
    {

      colour.r = photonlist->head->intensity.r;
      colour.g = photonlist->head->intensity.g;
      colour.b = photonlist->head->intensity.b;

      float max = colour.r;
      if (colour.g > max) max = colour.g;
      if (colour.b > max) max = colour.b;

      colour.r = colour.r/max;
      colour.g = colour.g/max;
      colour.b = colour.b/max;

      //cout << "Found " << photonlist->length << " photons nearby ray intersection " << photonlist->head->is_caustic << " " << photonlist->head->is_shadow << " " << photonlist->head->is_transmitted << endl;
    }
  }
  

}



void shadow_photons(Photon *photon, Vertex start, PhotonMap *global, Object *objects, Extents *scene_bounds)
{

  Photon *shadow_photon = new Photon();
  Hit shadow_hit = Hit();
  Ray shadow_ray = Ray();

  shadow_photon->direction = photon->direction;
  shadow_photon->is_shadow = true;

  shadow_photon->intensity = Colour();
  //shadow_photon->intensity.r = shadow_photon->intensity.r * -1.0f;
  //shadow_photon->intensity.g = shadow_photon->intensity.g * -1.0f;
  //shadow_photon->intensity.b = shadow_photon->intensity.b * -1.0f;
  
  
  shadow_ray.direction = photon->direction;
  shadow_ray.position = start;

  while (true)
  {

    object_test(shadow_ray, objects, shadow_hit, true);

    if (shadow_hit.flag == true)
    {

      shadow_photon->position = shadow_hit.position;
      global->addPhoton(shadow_photon);

      shadow_ray.position = shadow_ray.getPosition(shadow_hit.t + 0.0001f);

    } else break;

  }

}

void russian_roulette(Photon *photon, Hit hit, bool &absorbed, bool &is_caustic, bool &is_transmitted, Settings settings)
{
  float weight_absorb; //
  float weight_reflect; //reflection, specular
  float weight_transmit; //refraction, diffuse

  //max for a RGB sum is 3.
  float diffuse_sum = hit.what->material->diffuse.rgbSum();
  float specular_sum = hit.what->material->specular.rgbSum();
  float ambient_sum = hit.what->material->ambient.rgbSum();

  float reflect_sum = hit.what->reflectrefract->reflection.rgbSum(); //we want these to have more of an effect
  float refract_sum = hit.what->reflectrefract->refraction.rgbSum();




  weight_reflect = 0.5 * (0.4*(specular_sum/3) + 0.6*(reflect_sum/3)); //max is 0.5
  weight_transmit = 0.5 * (0.4*(diffuse_sum/3) + 0.6*(refract_sum/3)); //max is 0.5


  //sums can be low, so just tweak the values in their favour
  float tweak = settings.russian_roulette_tweak;
  weight_reflect = pow(weight_reflect, tweak);
  weight_transmit = pow(weight_transmit, tweak);

  //weight_absorb = 1;
  weight_absorb = 1 - weight_reflect - weight_transmit;






  mt19937 rng(rand());
  uniform_real_distribution<float> gen(0, 1); // uniform, unbiased

  float frame = gen(rng);

  if (frame < weight_absorb)
  {
    //absorb
    absorbed = true;
  } else if (frame < weight_absorb + weight_reflect) {
    //reflect
    is_caustic = true;

    Vector true_reflect = Vector();
    hit.normal.reflection(photon->direction, true_reflect); 


    Extents unit_box = Extents(Vertex(-0.5, -0.5, -0.5), Vertex(0.5, 0.5, 0.5));

    Vector dir = Vector();
    Vertex rand_dir = Vertex();
    while (true)
    {

        unit_box.get_random_point(rand_dir);
        dir.x = rand_dir.x;
        dir.y = rand_dir.y;
        dir.z = rand_dir.z;
        dir.normalise();

        if (true_reflect.dot(dir) > settings.caustic_min_contribution) break; //only calculate reflections that are going to be worth it..
    }

    photon->direction = dir;

    photon->intensity.r *= true_reflect.dot(dir);
    photon->intensity.g *= true_reflect.dot(dir);
    photon->intensity.b *= true_reflect.dot(dir);


    //cout << "Reflected." << endl;
  } else {
    //transmit in a random direction
    is_transmitted = true;

    Extents unit_box = Extents(Vertex(-0.5, -0.5, -0.5), Vertex(0.5, 0.5, 0.5));

    Vector dir = Vector();
    Vertex rand_dir = Vertex();
    while (true)
    {

        unit_box.get_random_point(rand_dir);
        dir.x = rand_dir.x;
        dir.y = rand_dir.y;
        dir.z = rand_dir.z;
        dir.normalise();

        if (hit.normal.dot(dir) > 0) break;
    }

    photon->direction = dir;


  }



}

void init_photon_maps(PhotonMap *caustic, PhotonMap *global, Object *objects, Light *lights, Extents *scene_bounds, Settings settings)
{
  cout << "Initialising photon maps.. " << endl;

  int missed_photons = 0;
  int generated_photons = 0;

  Light *light = lights;

  while (light != 0)
  {

    cout << "Casting photons for light " << light->name << endl;


    int PercentComplete = 0;

    for (int i = 0; i < settings.photon_count; i++)
    {

      Photon *photon = new Photon();
      Hit photon_hit = Hit();
      Ray photon_ray = Ray();

      light->generate_photon(photon, settings.photon_count, scene_bounds);
      generated_photons++;


      bool absorbed = false;
      int bounces = -1;
      while (absorbed == false) //until the photon gets absorbed
      {
        bounces++;

        photon_ray.direction = photon->direction;
        photon_ray.position = photon->position;

        photon_hit.flag = false;

        object_test(photon_ray, objects, photon_hit, false);

        if (photon_hit.flag == true)
        {

          Colour intensity_scalar = photon_hit.what->material->ambient;

          if (settings.update_photon_intensity_after_storage == false) photon->intensity.scale(intensity_scalar);

          if (scene_bounds->contains(photon_hit.position) == true)
          {

            photon->position = photon_hit.position;

            if (photon->is_caustic == true) //add this bad boy to our respective photon map
            {
              caustic->addPhoton(photon, photon_hit.what);
            } else {
              global->addPhoton(photon, photon_hit.what);
            }

            // do shadow photons
            shadow_photons(photon, photon_ray.getPosition(photon_hit.t + 0.0001f), global, objects, scene_bounds);


            russian_roulette(photon, photon_hit, absorbed, photon->is_caustic, photon->is_transmitted, settings);
            photon->position = photon_ray.getPosition(photon_hit.t - 0.0001f); //move it out infront of the object it hit


            if (settings.update_photon_intensity_after_storage == true) photon->intensity.scale(intensity_scalar);

          } else { //photon will not contribute to our image

            absorbed = true;
            i = i-1;
          }


        } else {

          absorbed = true; //didn't get a hit so bin it
          i = i-1; // try an extra photon as we missed
          missed_photons++;
        }

      }



      if ( int((100*i)/settings.photon_count) > PercentComplete)
      {
        PercentComplete = (100*i)/settings.photon_count;
        cout << PercentComplete-1 << "% complete. (photon casting)" << endl;
      }


      if (i < 0) i = 0;
    }

    light = light->next;
  }


  global->balance();
  caustic->balance();


  cout << "Photon maps populated." << endl;
  cout << missed_photons << "/" << generated_photons << "  (" << (100*missed_photons)/generated_photons << "%) missed." << endl;
}


int main(int argc, char *argv[])
{
  float eps = 0.0001f;

  int FRAME_BUFFER_SIZE;
  cout << "Enter Frame Buffer Size (default:512): ";
  cin >> FRAME_BUFFER_SIZE;
  cout << "Frame Buffer Size set to " << FRAME_BUFFER_SIZE << endl;

  int photon_count;
  cout << "Enter photon count (default:~15k): ";
  cin >> photon_count;
  cout << "Photon count set to " << photon_count << endl;

  int width = FRAME_BUFFER_SIZE;
  int height = FRAME_BUFFER_SIZE;
  // Create a framebuffer
  FrameBuffer *fb = new FrameBuffer(width,height);




  //Create settings for all raytracing (mostly photon-based)

  Settings settings = Settings();

  settings.do_plot_photons = false;
  settings.plot_photons_radius = 0.05f;

  settings.frame_buffer_size = FRAME_BUFFER_SIZE;
  settings.photon_count = photon_count;


  settings.search_using_radius = true;
  settings.search_using_radius_fast = true;

  settings.search_radius = 0.4f;
  settings.search_radius_closeness_scaling = 0.5f;
  settings.search_radius_neighbour_baseline = 25;

  settings.search_neighbour_count = 25;

  settings.photon_global_flux = 2;
  settings.photon_caustic_flux = 10;

  settings.update_photon_intensity_after_storage = true;

  settings.russian_roulette_tweak = 0.2f;

  settings.ambient_contribution = 0.5f; 

  settings.caustic_min_contribution = 0.5f;

  settings.shadow_divide_effect = 10;





  // The following transform allows 4D homogeneous coordinates to be transformed. It moves the supplied teapot model to somewhere visible.
  Transform *transform_small = new Transform(1.0f, 0.0f, 0.0f,  0.0f,
				       0.0f, 0.0f, 1.0f, -1.7f, //used to be -2.7
				       0.0f, 1.0f, 0.0f, 5.0f,
				       0.0f, 0.0f, 0.0f, 1.0f);



  //  Read in the teapot model.
  PolyMesh *teapot_small = new PolyMesh((char *)"teapot_smaller.ply", transform_small);
  teapot_small->name = "Small Teapot";


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

  
  Sphere *sphere3 = new Sphere(Vertex(4.6, -1.8, 6.7), 0.8f);
  sphere3->name = "Sphere 3";


  Vertex c0 = Vertex(4.2, -2.7, 5.9);
  Vertex c1 = Vertex(4.7, -1, 7.5);
  Box *box1 = new Box(c0, c1);
  box1->name = "Box 1";
  //box1->negateObject = sphere3;


  Box *box2 = new Box(Vertex(-1.2, -2.7, 2.8), Vertex(-0.8, 1, 3.2));
  box2->name = "Box 2";
  //sphere2->unionObject = box2;


  Box *box3 = new Box(Vertex(4, -2.5, 6.1), Vertex(5, -1.2, 7.3));
  box3->name = "Box 3";
  box1->negateObject = box3;


  Box *box4 = new Box(Vertex(-4, -2.7, 1.5), Vertex(-1, 3, 2));
  box4->name = "Box 4";


  Plane *plane1 = new Plane(Vertex(0, -2.7, 0), Vector(0, 1, 0));
  plane1->name = "Plane 1";



  // ACTUAL SCENE BELOW

  Extents *scene_bounds = new Extents(Vertex(-6, -3, 0), Vertex(6, 6, 10)); //estimate of what we can see


  Plane *plane_floor = new Plane(Vertex(0, -2.7, 0), Vector(0, 1, 0));
  plane_floor->name = "Plane (Floor)";
  plane_floor->occlude_light = false;

  Plane *plane_back = new Plane(Vertex(0, 0, 8), Vector(0, 0, -1)); // 0 0 9
  plane_back->name = "Plane (Back)";
  plane_back->occlude_light = false;

  Plane *plane_left = new Plane(Vertex(-5, 0, 0), Vector(1, 0, 0));
  plane_left->name = "Plane (Left)";
  plane_left->occlude_light = false;

  Plane *plane_right = new Plane(Vertex(5, 0, 0), Vector(-1, 0, 0));
  plane_right->name = "Plane (Right)";
  plane_right->occlude_light = false;

  Plane *plane_ceiling = new Plane(Vertex(0, 5.5, 0), Vector(0, -1, 0));
  plane_ceiling->name = "Plane (Ceiling)";
  plane_ceiling->occlude_light = false;




  Sphere *stand = new Sphere(Vertex(0, -2.7, 5), 2.0f);
  stand->name = "Stand";
  
  Box *stand_neg = new Box(Vertex(-3, -1.7, 2), Vertex(3, 0, 8));
  stand->negateObject = stand_neg;




  Sphere *bulb = new Sphere(Vertex(-2, 3, 3), 0.3f);
  bulb->name = "Bulb";
  bulb->occlude_light = false;

  float bc_give = 0.02f;
  Box *bulb_cord = new Box(
    Vertex(bulb->center.x-bc_give, bulb->center.y+bc_give, bulb->center.z-bc_give),
    Vertex(bulb->center.x+bc_give, plane_ceiling->r0.y, bulb->center.z+bc_give)
  );
  bulb_cord->name = "Bulb Cord";
  bulb_cord->occlude_light = false;




  Box *table = new Box(Vertex(1, -2.7, 6), Vertex(4, 1, 8));
  table->name = "Table";

  float table_give = 0.2f;

  Box *table_cross_neg = new Box(
    Vertex(table->extents.min.x+table_give, table->extents.min.y-eps, table->extents.min.z-eps), 
    Vertex(table->extents.max.x-table_give, table->extents.max.y-table_give, table->extents.max.z+eps)
  );
  Box *table_cross_neg_union = new Box(
    Vertex(table->extents.min.x-eps, table->extents.min.y-eps, table->extents.min.z+table_give), 
    Vertex(table->extents.max.x+eps, table->extents.max.y-table_give, table->extents.max.z-table_give)
  );

  table_cross_neg->unionObject = table_cross_neg_union;
  table->negateObject = table_cross_neg;





  Transform *transform = new Transform(1.0f, 0.0f, 0.0f,  0.0f,
				       0.0f, 0.0f, 1.0f, -1.7f, //used to be -2.7
				       0.0f, 1.0f, 0.0f, 10.0f,
				       0.0f, 0.0f, 0.0f, 1.0f);

  PolyMesh *teapot = new PolyMesh((char *)"teapot.ply", transform);
  teapot->name = "Teapot";

  Box *back_wall = new Box(Vertex(-5, -2.7, 8), Vertex(5, 5.5, 12));
  back_wall->name = "Back Wall";
  back_wall->negateObject = teapot_small;



  Box *mirror = new Box(Vertex(4.95, -1, 5.5), Vertex(5, 2, 7));
  mirror->name = "Mirror";



  Box *glass = new Box(Vertex(-4, -2.7, 4.2), Vertex(-3.4, 0, 4.8));
  glass->name = "Glass";

  




  Ray ray;

  ray.position.x = 0.0001f;
  ray.position.y = 0.0f;
  ray.position.z = 0.0f;



  DirectionalLight *dl = new DirectionalLight(Vector(1.01f, -1.0f, 1.0f),Colour(1.0f, 1.0f, 1.0f, 0.0f));
  dl->name = "Directional Light";

  DirectionalLight *dl2 = new DirectionalLight(Vector(-1.01f, -1.0f, 1.0f),Colour(1.0f, 1.0f, 1.0f, 0.0f));
  dl2->name = "Directional Light 2";

  PointLight *pl = new PointLight(
    Vertex(bulb->center.x, bulb->center.y-eps, bulb->center.z), 
    Colour(1.0f, 1.0f, 1.0f, 0.0f)
  );
  pl->name = "Point Light";
  pl->fall_off_rate = 1;



  Phong bp1 = Phong(Colour(0.2f, 0.0f, 0.0f), Colour(0.4f, 0.0f, 0.0f), Colour(0.4f, 0.4f, 0.4f), 40.0f);
  Phong bp2 = Phong(Colour(0.0f, 0.2f, 0.2f), Colour(0.0f, 0.4f, 0.0f), Colour(0.4f, 0.4f, 0.4f), 40.0f);

  Phong bp3 = Phong(Colour(0.1f, 0.1f, 0.3f), Colour(0.0f, 0.0f, 0.5f), Colour(0.5f, 0.5f, 0.5f), 40.0f);
  Phong bp4 = Phong(Colour(0.3f, 0.2f, 0.1f), Colour(0.5f, 0.1f, 0.0f), Colour(0.5f, 0.5f, 0.5f), 40.0f);
  Phong bp5 = Phong(Colour(0.3f, 0.3f, 0.1f), Colour(0.0f, 0.6f, 0.2f), Colour(0.5f, 0.5, 0.5f), 40.0f);
  Phong bp6 = Phong(Colour(0.3f, 0.3f, 0.1f), Colour(0.3f, 0.3f, 0.0f), Colour(0.5f, 0.5f, 0.5f), 40.0f);
  Phong bp7 = Phong(Colour(0.3f, 0.2f, 0.3f), Colour(0.2f, 0.2f, 0.2f), Colour(0.5f, 0.5f, 0.5f), 40.0f);

  Phong black = Phong(Colour(), Colour(), Colour(), 40.0f);

  Phong ph_white = Phong(Colour(0.6f, 0.6f, 0.6f), Colour(0.4f, 0.4f, 0.4f), Colour(0.4f, 0.4f, 0.4f), 40.0f);
  Phong ph_red = Phong(Colour(0.6f, 0.1f, 0.1f), Colour(0.5f, 0.0f, 0.0f), Colour(0.5f, 0.5f, 0.5f), 40.0f);
  Phong ph_blue = Phong(Colour(0.1f, 0.1f, 0.6f), Colour(0.0f, 0.0f, 0.5f), Colour(0.5f, 0.5f, 0.5f), 40.0f);
  Phong ph_dark = Phong(Colour(0.05f, 0.05f, 0.05f), Colour(0.1f, 0.1f, 0.1f), Colour(0.2f, 0.2f, 0.2f), 40.0f);
  Phong ph_orange = Phong(Colour(0.6f, 0.3f, 0.1f), Colour(0.5f, 0.5f, 0.0f), Colour(0.5f, 0.5f, 0.5f), 40.0f);
  Phong ph_yellow = Phong(Colour(0.5f, 0.5f, 0.0f), Colour(0.5f, 0.5f, 0.0f), Colour(0.5f, 0.5f, 0.5f), 40.0f);


  teapot->material = &bp1;
	sphere->material = &bp2;
 	sphere2->material = &bp2;
  box1->material = &bp2;
  plane1->material = &bp1;

  plane_left->material = &ph_red;
  plane_right->material = &ph_blue;
  plane_ceiling->material = &ph_dark;
  plane_floor->material = &ph_white;
  plane_back->material = &ph_white;

  stand->material = &ph_dark;
  table->material = &ph_dark;
  teapot->material = &ph_orange;
  teapot_small->material = &ph_orange;

  bulb->material = &ph_white;
  bulb_cord->material = &ph_yellow;




  ReflectRefract reflective = ReflectRefract();
  reflective.setReflection(Colour(0.5f, 0.5f, 0.5f, 1.0f));
  
  ReflectRefract refraction = ReflectRefract();
  refraction.setRefraction(Colour(0.8f, 0.8f, 0.8f, 1.0f));
  refraction.refractive_index = 1.2f;


  //plane1->reflectrefract = &reflective;
  //box4->reflectrefract = &refraction;
  plane_floor->reflectrefract = &reflective;
  glass->reflectrefract = &refraction;


/*
  Object *obj = teapot;
	obj->next = sphere2;
*/

  Object *obj = plane_floor;
  obj->addObject(plane_left);
  obj->addObject(plane_back);
  obj->addObject(plane_ceiling);
  obj->addObject(plane_right);

  obj->addObject(teapot_small);
  obj->addObject(stand);
  
  //obj->addObject(bulb);
  //obj->addObject(bulb_cord);

  //obj->addObject(mirror);
  //obj->addObject(back_wall);
  obj->addObject(glass);

  //obj->addObject(table);
  //obj->addObject(table_cross_neg);
  


  Light *light = pl;
  light->next = dl;
  light->next->next = dl2;



  // Photons
  PhotonMap *caustic = new PhotonMap();
  PhotonMap *global = new PhotonMap();
  init_photon_maps(caustic, global, obj, light, scene_bounds, settings);


  // Ray tracing

  int PercentComplete = 0;
  
  cout << "Starting to loop through pixels.." << endl;
  for (int y = 0; y < height; y += 1)
  {
    for (int x = 0; x < width; x += 1)
    {
      //cout << "Pixel " << x << "," << y << endl;

      float fx = (float)x/(float)width;
      float fy = (float)y/(float)height;

      Vector direction;

      ray.direction.x = (fx-0.5f);
      ray.direction.y = (0.5f-fy);
      ray.direction.z = 0.5f;
      ray.direction.normalise();

      Colour colour;
      float depth;
      int reflection_depth = 0;
      int refractive_depth = 0;


      settings.current_x = x;
      settings.current_y = y;

      raytrace(ray, obj, light, colour, depth, reflection_depth, refractive_depth, caustic, global, settings);

      fb->plotPixel(x, y, colour.r, colour.g, colour.b);
      fb->plotDepth(x,y, depth);
    }


    if ((100*y)/FRAME_BUFFER_SIZE > PercentComplete)
    {
      PercentComplete = (100*y)/FRAME_BUFFER_SIZE;
      cout << PercentComplete-1 << "% complete. (raycasting)" << endl;
    }

  }
  
  // Output the framebuffer.
  fb->writeRGBFile((char *)"test.ppm");
  cout << "Wrote to RGB file" << endl;
  //  fb->writeDepthFile((char *)"depth.pteapot");
  return 0;
  
}
