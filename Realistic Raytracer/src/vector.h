/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2019.
 *
 * Do what you like with this code as long as you retain this comment.
 */

#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <math.h>
#include "vertex.h"

class Vector {
public:
	float x;
	float y;
	float z;

	Vector(float px, float py, float pz)
	{
		x = px;
		y = py;
		z = pz;
	}

	Vector()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	Vector(Vertex vert)
	{
		x = vert.x;
		y = vert.y;
		z = vert.z;
	}

	void normalise()
	{
		float len = (float)sqrt((double)(x*x + y*y + z*z));
		x = x / len;
		y = y / len;
		z = z / len;
	}

	float len_sqr()
	{
	  return(x*x + y*y + z*z);
	}

	float length()
	{
	  return(float)sqrt((double)(x*x + y*y + z*z));
	}
	/*
	float dot(Vector &other)
	{
		return x*other.x + y*other.y + z*other.z;
	}
	*/
	float dot(Vector other)
	{
		return x*other.x + y*other.y + z*other.z;
	}


	void reflection(Vector initial, Vector &reflect)
	{
		float d;

		d = dot(initial);
		d = d * 2.0f;

		reflect.x = initial.x - d * x;
		reflect.y = initial.y - d * y;
		reflect.z = initial.z - d * z;
	}

	void refraction(Vector initial, float ior, Vector &refract) // ior is index of refraction
	{
		float cosI = dot(initial);
		float cosT = sqrt( 1 - (1/ior*ior)*(1 - cosI*cosI) );

		refract.x = (1/ior) * initial.x - (cosT - (1/ior)*cosI) * x;
		refract.y = (1/ior) * initial.y - (cosT - (1/ior)*cosI) * y;
		refract.z = (1/ior) * initial.z - (cosT - (1/ior)*cosI) * z;
	}

	void negate()
	{
		x = -x;
		y = -y;
		z = -z;
	}

	void getInverse(Vector &inverse)
	{
		inverse.x = -x;
		inverse.y = -y;
		inverse.z = -z;
	}

	void cross(Vector &other, Vector &result)
	{
	  result.x = y*other.z - z*other.y;
	  result.y = z*other.x - x*other.z;
	  result.z = x*other.y - y*other.x;
	}

	void cross(Vector &other)
	{
	  Vector result;
	  result.x = y*other.z - z*other.y;
	  result.y = z*other.x - x*other.z;
	  result.z = x*other.y - y*other.x;
	  x = result.x;
	  y = result.y;
	  z = result.z;
	}

	void add(Vector &other)
	{
	  x += other.x;
	  y += other.y;
	  z += other.z;
	}

	void sub(Vector &other)
	{
	  x -= other.x;
	  y -= other.y;
	  z -= other.z;
	}

	Vector &operator=(Vector other)
	{
	  x = other.x;
	  y = other.y;
	  z = other.z;

	  return *this;
	}

	friend Vector operator+(const Vector &a, const Vector &b)
	{
	  Vector t;
	  t.x = a.x + b.x;
	  t.y = a.y + b.y;
	  t.z = a.z + b.z;
	  return t;
	}
	friend Vector operator-(const Vector &a, const Vector &b)
	{
	  Vector t;
	  t.x = a.x - b.x;
	  t.y = a.y - b.y;
	  t.z = a.z - b.z;
	  return t;
	}

        friend Vector operator*(const Vector &a, const Vector &b)
		{
	  Vector t;
	  t.x = a.x * b.x;
	  t.y = a.y * b.y;
	  t.z = a.z * b.z;
	  return t;
	}

	        friend Vector operator*(const float a, const Vector &b)
		{
	  Vector t;
	  t.x = a * b.x;
	  t.y = a * b.y;
	  t.z = a * b.z;
	  return t;
	}



	friend ostream& operator << (ostream& os, const Vector& vector)
	{
		os << vector.x << "," << vector.y << "," << vector.z;
		return os ;
	}

	
};

#endif
