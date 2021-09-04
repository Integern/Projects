/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2018.
 *
 * Do what you like with this code as long as you retain this comment.
 */

// DirectionaLight is a child class of Light and implements a light
// with constant value in a given direction. The light has no position
// and can be treated as infinitely far away.

/*

    Joel Broad 2020

    added generate_photon method

*/

#pragma once
#include "light.h"

class DirectionalLight : public Light {
public:

	Vector direction;
	Colour intensity;


	DirectionalLight()
	{
		Light();
	}
	
	DirectionalLight(Vector dir, Colour col)
	{
		Light();

		direction = dir;
		direction.normalise();
		intensity = col;
	}

	bool get_direction(Vertex &surface, Vector &dir)
	{
		dir = direction;

		return true;
	}

	void get_position(Hit hit, Vertex &vertex)
    {
		float scale = 0.1f; //treat that light is just infront of hitpoint to negate occlusion from any other objects

        vertex.x = hit.position.x - direction.x * scale;
        vertex.y = hit.position.y - direction.y * scale;
        vertex.z = hit.position.z - direction.z * scale;
    }

	void get_intensity(Vertex &surface, Colour &level)
	{
		level = intensity;
	}

	void generate_photon(Photon *photon, int total_photons, Extents *scene_bounds)
	{

		photon->direction = direction;
		photon->direction.normalise();
		
		photon->intensity = intensity;
		photon->intensity.r = photon->intensity.r * 1/total_photons;
		photon->intensity.g = photon->intensity.g * 1/total_photons;
		photon->intensity.b = photon->intensity.b * 1/total_photons;

		scene_bounds->get_random_point(photon->position); //ensures the photon will end up in the scene after raytracing.

	}

};
