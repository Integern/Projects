/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2019.
 *
 * Do what you like with this code as long as you retain this comment.
 */

// Ray is a class to store and maniplulate 3D rays.

#pragma once

#include <iostream>
using namespace std;

#include "vertex.h"
#include "vector.h"

class Ray {
public:
	Vertex position;
	Vector direction;

	Ray()
	{
	}

	

	Ray(Vertex p, Vector d)
	{
		position = p;
		direction = d;
	}

	Vertex getPosition(float t) // gives position of ray at t value; so t=0 would give ray origin
	{
		Vertex answer;
		answer.w = 1.0f;

		answer.x = position.x + direction.x * t;
		answer.y = position.y + direction.y * t;
		answer.z = position.z + direction.z * t;

		return answer;
	}

	friend ostream& operator<<(ostream& os, const Ray& r)
	{
	  os << "Ray{[" << r.position.x << "," << r.position.y << "," << r.position.z << "],[" << r.direction.x << "," << r.direction.y << "," << r.direction.z << "]}"; 
          return os;
	}
};
