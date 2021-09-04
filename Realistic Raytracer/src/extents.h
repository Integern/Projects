/*

    Joel Broad 2020

    A class that stores a pair of vertexes that have min/max values; useful for finding the bounding box of a polymesh

*/

#pragma once

#include "vertex.h"

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>


class Extents {
public:
    Vertex min;
    Vertex max;

    Extents(Vertex p0, Vertex p1)
    {
        min = Vertex(1000000, 1000000, 1000000);
        max = Vertex(-1000000, -1000000, -100000);

        introduceVertex(p0);
        introduceVertex(p1);
    }

    Extents()
    {
        min = Vertex(100000, 100000, 100000);
        max = Vertex(-100000, -100000, -100000);
    }

    void introduceVertex(Vertex point)
    {
        if (point.x > max.x) { max.x = point.x; }
        if (point.y > max.y) { max.y = point.y; }
        if (point.z > max.z) { max.z = point.z; }
            if (point.w > max.w) { max.w = point.w; }

        if (point.x < min.x) { min.x = point.x; }
        if (point.y < min.y) { min.y = point.y; }
        if (point.z < min.z) { min.z = point.z; }
            if (point.w < min.w) { min.w = point.w; }
    }

    bool contains(Vertex point)
    {
        float s = 0.0001; // small numbers to negate floating point errors
        if ( (point.x-s > max.x) || (point.x+s < min.x) ) { return false; }
        if ( (point.y-s > max.y) || (point.y+s < min.y) ) { return false; }
        if ( (point.z-s > max.z) || (point.z+s < min.z) ) { return false; }

        return true;
    }

    void get_random_point(Vertex &point)
    {
        mt19937 rng(rand());
        uniform_real_distribution<float> gen(0, 1); // uniform, unbiased

        point.x = gen(rng)*(max.x - min.x) + min.x;
        point.y = gen(rng)*(max.y - min.y) + min.y;
        point.z = gen(rng)*(max.z - min.z) + min.z;

    }


	friend ostream& operator<<(ostream& os, const Extents& ext)
	{
	  os << "Extents{[" << ext.min.x << "," << ext.min.y << "," << ext.min.z << "],[" << ext.max.x << "," << ext.max.y << "," << ext.max.z << "]}"; 
          return os;
	}

};
