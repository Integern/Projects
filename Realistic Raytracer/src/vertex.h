/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2019.
 *
 * Do what you like with this code as long as you retain this comment.
 */

#pragma once

#include <iostream>
#include <math.h>
using namespace std;

class Vertex {
public:
	float x;
	float y;
	float z;
	float w;

	Vertex()
	{
		x = 0.0;
		y = 0.0;
		z = 0.0;
		w = 1.0f;
	}

	Vertex(float px, float py, float pz, float pw)
	{
		x = px;
		y = py;
		z = pz;
		w = pw;
	}

	Vertex(float px, float py, float pz)
	{
		x = px;
		y = py;
		z = pz;
		w = 1.0f;
	}


		Vertex operator + (Vertex vertex1)
	{
		Vertex vertex2;
		vertex2.x = x + vertex1.x;
		vertex2.y = y + vertex1.y;
		vertex2.z = z + vertex1.z;
		return vertex2;
	}

	Vertex operator - (Vertex vertex1)
	{
		Vertex vertex2;
		vertex2.x = x - vertex1.x;
		vertex2.y = y - vertex1.y;
		vertex2.z = z - vertex1.z;
		return vertex2;		
	}

	Vertex &operator=(Vertex other)
	{
	  x = other.x;
	  y = other.y;
	  z = other.z;

	  return *this;
	}

	float len() //distance from (0,0,0)
	{
		return sqrt(x*x + y*y + z*z); 
	}



	bool inBounds(Vertex bound0, Vertex bound1, float leeway) //bound0 has smallest values, bound1 has largest values
	{
		if ( (bound0.x > bound1.x) || (bound0.y > bound1.y) || (bound0.z > bound1.z) )
		{
			cerr << "ERROR. Should be   smallest: " << bound0 << "  biggest: " << bound1 << endl;
		}
		/*
		cout << "in bounds check " << bound0 << " " << bound1 << endl;
		cout << "plane hit: " << x << " " << y << " " << z << endl;
		cout << "x inequalities: " << (bound0.x <= x + leeway) << " " << (x - leeway <= bound1.x) << endl;
		cout << "y inequalities: " << (bound0.y <= y + leeway) << " " << (y - leeway <= bound1.y) << endl;
		cout << "z inequalities: " << (bound0.z <= z + leeway) << " " << (z - leeway <= bound1.z) << endl;
		*/
		int matches = 0; //must be 3 (for 3 axis) to return true

		if (bound0.x <= x + leeway && x - leeway <= bound1.x)
		{
			matches++;
		}
		if (bound0.y <= y + leeway && y - leeway <= bound1.y)
		{
			matches++;
		}
		if (bound0.z <= z + leeway && z - leeway <= bound1.z)
		{
			matches++;
		}

		//cout << "matched on total axis:" << matches << endl;
		if (matches == 3) return true;

		return false;
	}


	friend ostream& operator << (ostream& os, const Vertex& vertex)
	{
		os << vertex.x << "," << vertex.y << "," << vertex.z << " ," << vertex.w;
		return os ;
	}

};
