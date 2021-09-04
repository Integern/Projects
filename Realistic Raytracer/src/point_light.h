/*

    Joel Broad 2020

    Point Light class which is a type of Light

*/
#pragma once

#include <math.h>
#include "light.h"


class PointLight : public Light {
public:

    Vertex position;
    Vector direction;
    bool has_direction = false;
    Colour intensity;

    Extents unit_box = Extents(Vertex(-0.5f, -0.5f, -0.5f), Vertex(0.5f, 0.5f, 0.5f)); //used for generating random direction

    int fall_off_rate = 1; //the higher the value, the more focused the intensity on the direction.

    PointLight()
    {
        Light();
    }

    PointLight(Vertex pos, Colour col)
    {
        Light();

        position = pos;
        intensity = col;
    }

    PointLight(Vector dir, Vertex pos, Colour col)
    {
        Light();

        position = pos;
        intensity = col;

        direction = dir;
        direction.normalise();
        has_direction = true;
    }


	bool get_direction(Vertex &surface, Vector &dir)
	{
        dir.x = surface.x - position.x;
        dir.y = surface.y - position.y;
        dir.z = surface.z - position.z;
        dir.normalise();

        if (has_direction && direction.dot(dir) <= 0) return false; //

        return true;
        
	}

    void get_position(Hit hit, Vertex &vertex)
    {
        vertex.x = position.x;
        vertex.y = position.y;
        vertex.z = position.z;
    }

	void get_intensity(Vertex &surface, Colour &level)
	{
        Vector look_dir;
        if ( get_direction(surface, look_dir) )
        {
            level = intensity;

            if (has_direction)
            {
                float dot_product = pow(look_dir.dot(direction), fall_off_rate);
                level.r = level.r * dot_product;
                level.g = level.g * dot_product;
                level.b = level.b * dot_product;
            }

        } else {
            level = Colour(); 
        }
	}

    void generate_photon(Photon *photon, int total_photons, Extents *scene_bounds)
	{
		photon->position = position;

        photon->intensity = intensity;
		photon->intensity.r = photon->intensity.r * 1/total_photons;
		photon->intensity.g = photon->intensity.g * 1/total_photons;
		photon->intensity.b = photon->intensity.b * 1/total_photons;

        Vector dir = Vector();
        Vertex rand_dir = Vertex();
        while (true)
        {

            unit_box.get_random_point(rand_dir);
            dir.x = rand_dir.x;
            dir.y = rand_dir.y;
            dir.z = rand_dir.z;
            dir.normalise();

            if (has_direction) //needs to be in our scope
            {
                if (direction.dot(dir) > 0)
                {
                    break;
                }
            } else {
                break; //can be any direction
            }
        }
        photon->direction = dir;
        
	}

};