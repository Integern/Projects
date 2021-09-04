/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2019.
 *
 * Do what you like with this code as long as you retain this comment.
 */

// Hit is a class to store and maniplulate information about an intersection
// between a ray and an object.

/*

    Joel Broad 2020

	added override method; makes code concise when we're finding the closest hit

*/

#pragma once

#include <iostream>
using namespace std;


#include "vertex.h"
#include "vector.h"

class Object;

class Hit {
public:
	bool flag;
	float t;
	Object *what;
	Vertex position;
	Vector normal;

	Hit()
	{
		flag = false;
		t = -1.0f;
	}

	void override(Hit &other, bool ignore_t) //custom_function
	{
		if (ignore_t == false && flag == true && (other.t > t || other.flag == false)) return;

		flag = other.flag;
		t = other.t;
		what = other.what;
		position = other.position;
		normal = other.normal;
	}

	friend ostream& operator<<(ostream& os, const Hit& h)
	{
	  os << "Hit{" << h.flag << " t:" << h.t << ",[" << h.position.x << "," << h.position.y << "," << h.position.z << "],[" << h.normal.x << "," << h.normal.y << "," << h.normal.z << "]}"; 
          return os;
	}
};
