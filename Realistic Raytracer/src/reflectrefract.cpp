/*

    Joel Broad 2020

    Reflection and Refraction values/calculations for a Material

*/
#include "reflectrefract.h"

#include <math.h>
#include "ray.h"
#include "vector.h"
#include "hit.h"


ReflectRefract::ReflectRefract()
{

}

void ReflectRefract::compute_reflection_direction(Ray &ray, Vector &hit_normal, Vector &reflection)
{
	
	hit_normal.reflection(ray.direction, reflection);
	reflection.normalise();
}



bool ReflectRefract::compute_refraction_direction(Ray &ray, Hit &hit, float n_outside, float n_inside, Vector &refraction)
{
    //compute refraction inside material
    float n = n_inside/n_outside;
    float cos_theta_i = ray.direction.dot(hit.normal);
    float cos_theta_t = sqrt(1 - (1/pow(n, 2))*(1-pow(cos_theta_i, 2)));


    if (cos_theta_t > cos_theta_i)
    {
        //total internal reflection
        return false;
    }

    refraction.x = (1/n) * ray.direction.x - (cos_theta_t - (1/n)*cos_theta_i) * hit.normal.x;
    refraction.y = (1/n) * ray.direction.y - (cos_theta_t - (1/n)*cos_theta_i) * hit.normal.y;
    refraction.z = (1/n) * ray.direction.z - (cos_theta_t - (1/n)*cos_theta_i) * hit.normal.z;

    return true;
}


void ReflectRefract::setReflection(Colour refl)
{
    reflection = refl;
    reflection_inverse = Colour(1-reflection.r, 1-reflection.g, 1-reflection.b, 1.0f);
}

void ReflectRefract::setRefraction(Colour refr)
{
    refraction = refr;
    refraction_inverse = Colour(1-refraction.r, 1-refraction.g, 1-refraction.b, 1.0f);
}