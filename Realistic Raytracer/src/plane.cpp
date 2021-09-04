/*

    Joel Broad 2020

    Plane class; mathematical plane with a point on the plane and a normal

*/

#include "plane.h"
#include <math.h>

Plane::Plane(Vertex p1, Vertex p2, Vertex p3)
{
    Object();
    this->construct(p1, p2, p3);
}

Plane::Plane(Vertex point, Vector normal)
{
    Object();
    r0 = point;
    n = normal;
    normal.getInverse(n_neg);

    d = -(n.x * r0.x + n.y * r0.y + n.z * r0.z);
}

void Plane::construct(Vertex p1, Vertex p2, Vertex p3)
{
    Vector v0 = Vector(p2-p1);
    Vector v1 = Vector(p3-p1);
    v0.cross(v1, n);
    n.normalise();
    r0 = p1;
    d = -(n.x * r0.x + n.y * r0.y + n.z * r0.z);

    v0.cross(v1, n_neg);
    n_neg.normalise();
    n_neg.negate();
}

Plane::Plane()
{
    this->construct(Vertex(0,0,0), Vertex(1,1,1), Vertex(-1,-1,-1));
}

bool Plane::inPlane(Vertex point)
{
    Vector vector = Vector(point-r0);
    float dot = n.dot(vector);
    if (dot == 0)
    {
        return true;
    }
    return false;
}

void Plane::intersection(Ray ray, Hit &hit)
{
    
    float t;
    Vector D = ray.direction;
    Vector P = Vector(ray.position);
    Vector norm;

    hit.flag = false;

    if (n.dot(D) != 0) //If the ray and the plane are not paralell
    {
        t = ( n.dot(Vector(r0-ray.position)) ) / ( n.dot(ray.direction) );
        norm = n;
    } else if (n_neg.dot(D) != 0) { //check the back just in case..
        t = ( n_neg.dot(Vector(r0-ray.position)) ) / ( n_neg.dot(ray.direction) );
        norm = n_neg;
    } else { return; }

    hit.what = this;
    hit.t = t;
	hit.position.x = ray.position.x + t * ray.direction.x;
	hit.position.y = ray.position.y + t * ray.direction.y;
	hit.position.z = ray.position.z + t * ray.direction.z;
    hit.normal = norm;
	hit.flag = true;
}

void Plane::intersection_disc(Ray ray, Hit &hit, float r)
{
    this->intersection_disc(ray, hit, r0, r);
}

void Plane::intersection_disc(Ray ray, Hit &hit, Vertex p, float r)
{
    this->intersection(ray, hit);
    if (hit.flag == false) { return; }

    float x = hit.position.x - p.x;
    float y = hit.position.y - p.y;
    float z = hit.position.z - p.z;
    float dist = (float)sqrt((double)(x*x + y*y + z*z));

    if (dist > r) //Hit is outside the disc
    {
        hit.flag = false;
    }
}
