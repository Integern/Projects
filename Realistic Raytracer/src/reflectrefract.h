/*

    Joel Broad 2020

    Reflection and Refraction values/calculations for a Material

*/
#pragma once

#include <math.h>
#include "phong.h"
#include "vertex.h"
#include "vector.h"
#include "ray.h"
#include "colour.h"
#include "hit.h"


class ReflectRefract {
public:

    Colour reflection = Colour(); //r,g,b should only have values in [0, 1]
    Colour reflection_inverse = Colour(1-reflection.r, 1-reflection.g, 1-reflection.b, 1.0f); //inverse used to weighting colour that isn't reflected
	Colour refraction = Colour();
    Colour refraction_inverse = Colour(1-refraction.r, 1-refraction.g, 1-refraction.b, 1.0f);
    float refractive_index = 1.0f;

    ReflectRefract();

    void compute_reflection_direction(Ray &ray, Vector &hit_normal, Vector &reflection);
    bool compute_refraction_direction(Ray &ray, Hit &hit, float n_outside, float n_inside, Vector &refraction);

    void setReflection(Colour refl);
    void setRefraction(Colour refr);

};