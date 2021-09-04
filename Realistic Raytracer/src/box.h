/*

    Joel Broad 2020

    Box object; given 2 corners constructs a box (needn't be a cube)

*/

#pragma once

#include <iostream>
using namespace std;


#include "vertex.h"
#include "vector.h"
#include "extents.h"
#include "object.h"
#include "plane.h"


class Box : public Object {
public:
    Extents extents;
    Plane *sides;

    Box()
    {
        Object();
    }

    Box(Vertex c0, Vertex c1)
    {
        Object();
        this->construct(c0, c1);
    }
    Box(Extents EXT)
    {
        Object();
        this->construct(EXT.min, EXT.max);
    }

    void construct(Vertex c0, Vertex c1)
    {
        extents = Extents(c0, c1);

        sides = new Plane[6]; 

        //Create 6 sides using plane objects
        sides[0].construct(extents.min, extents.min+Vertex(1,0,0), extents.min+Vertex(0,1,0)); //front
        sides[1].construct(extents.min, extents.min+Vertex(0,0,1), extents.min+Vertex(0,1,0)); //left
        sides[2].construct(extents.min, extents.min+Vertex(1,0,0), extents.min+Vertex(0,0,1)); //bottom
        sides[3].construct(extents.max, extents.max+Vertex(0,0,-1), extents.max+Vertex(0,-1,0)); //right
        sides[4].construct(extents.max, extents.max+Vertex(0,0,-1), extents.max+Vertex(-1,0,0)); //top
        sides[5].construct(extents.max, extents.max+Vertex(-1,0,0), extents.max+Vertex(0,-1,0)); //back

    }

    void intersection(Ray ray, Hit &hit) //finds the closest intersection from the 6 sides, that are inside the bounds of the box
    {
        Hit plane_hit;
        hit.flag = false;
        plane_hit.flag = false;

        for (int j = 0; j < 6; j++)
        {
            plane_hit.flag = false;
            plane_hit.position = Vertex();
            sides[j].intersection(ray, plane_hit);

            if ( (plane_hit.flag) && (plane_hit.t > 0))
            {
                if (extents.contains(plane_hit.position) == true) //hit is bounded by our box
                {
                    if ( (hit.flag == false) || (hit.t > plane_hit.t) ) //We've hit a plane, inside our box, and its closest
                    {
                        hit.t = plane_hit.t;
                        hit.position = plane_hit.position;
                        hit.normal = plane_hit.normal;
                        hit.flag = true;
                    }
                }
            }
        }

        if (hit.flag)
        {
            hit.what = this;
        }
    }


	friend ostream& operator << (ostream& os, const Box& box)
	{
		os << "X: [" << box.extents.min.x << ", " << box.extents.max.x << "]  Y: [" << box.extents.min.y << ", " << box.extents.max.y << "]  Z: [" << box.extents.min.z << ", " << box.extents.max.z << "]";
		return os ;
	}

};
