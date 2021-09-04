/*

    Joel Broad 2020

    Plane class; mathematical plane with a point on the plane and a normal

*/

#pragma once

#include "vertex.h"
#include "vector.h"
#include "hit.h"
#include "ray.h"
#include "object.h"

class Plane : public Object {
public:
    Vector n;
    Vector n_neg; //inverse of normal; i want to detect even if i hit the back of a plane.
    Vertex r0;
    float d;

    Plane(Vertex p1, Vertex p2, Vertex p3);
    Plane(Vertex point, Vector normal);
    void construct(Vertex p1, Vertex p2, Vertex p3);
    Plane();
    bool inPlane(Vertex point);
    void intersection(Ray ray, Hit &hit);
    void intersection_disc(Ray ray, Hit &hit, float r);
    void intersection_disc(Ray ray, Hit &hit, Vertex p, float r);

};
