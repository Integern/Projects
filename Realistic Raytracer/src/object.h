/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2019.
 *
 * Do what you like with this code as long as you retain this comment.
 */

// Object is the base class for objects.

/*

    Joel Broad 2020

	added reflection/refraction characteristics (e.g. mirror, glass)
	added default material values
	added addObject method

*/

#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "ray.h"
#include "hit.h"
#include "phong.h"
#include "material.h"
#include "reflectrefract.h"

class Object {
public:

	Object *next;
	Material *material;
	ReflectRefract *reflectrefract;

	string name;

	bool occlude_light; //if false, won't shadow/block light paths

	Object *negateObject; //if it is negated by any object
	Object *unionObject; //if it is union with any object

	Object()
	{
		next = (Object *)0;

		negateObject = (Object *)0;
		unionObject = (Object *)0;

		name = "Unknown Object";

		occlude_light = true;
		
		Phong *bp1 = new Phong();
		bp1->ambient.r = 1.0f;
		bp1->ambient.g = 1.0f;
		bp1->ambient.b = 1.0f;
		bp1->diffuse.r = 0.4f;
		bp1->diffuse.g = 0.4f;
		bp1->diffuse.b = 0.4f;
		bp1->specular.r = 0.4f;
		bp1->specular.g = 0.4f;
		bp1->specular.b = 0.4f;
		bp1->power = 10.0f;
		material = bp1;
		
		reflectrefract = new ReflectRefract();
	}
	
	virtual void intersection(Ray ray, Hit &hit)
	{

	}

	void setMaterial(Material mat)
	{
		material = &mat;
	}

	void addObject(Object *object)
	{
		Object *next_obj = this;

		while (next_obj->next != 0) next_obj = next_obj->next;

		next_obj->next = object;
	}
};

#endif
