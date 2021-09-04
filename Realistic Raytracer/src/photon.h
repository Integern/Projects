/*

    Joel Broad 2020

    Photon class; simulates a photon!

*/
#pragma once

#include <math.h>
#include "colour.h"
#include "vertex.h"
#include "vector.h"


class Photon {
public:

    Photon *next; //used as a temporary pointer; (when adding photons to the map before balancing & for a list of found photon in search())
    Photon *list_next; //used when we create photons and want to keep them in a list in the photon map.

    Colour intensity;
    Vector direction;
    Vertex position;
    
    bool is_caustic; //type of the photon
    bool is_shadow;
    bool is_transmitted;

    Photon()
    {
        next = (Photon *)0;
        list_next = (Photon *)0;
        intensity = Colour(0.0f, 0.0f, 0.0f, 0.0f);
        direction = Vector();
        position = Vertex();
        is_caustic = false;
        is_shadow = false;
        is_transmitted = false;
    }

    Photon(Vertex POSITION)
    {
        next = (Photon *)0;
        list_next = (Photon *)0;
        intensity = Colour(0.0f, 0.0f, 0.0f, 0.0f);
        direction = Vector();
        position = POSITION;
        is_caustic = false;
        is_shadow = false;
        is_transmitted = false;
    }

    void clone(Photon *copy) //method used when we're storing a photon to our photonmap; using pointers so got to make a new Photon.
    {
        intensity = copy->intensity;
        direction = copy->direction;
        position = copy->position;

        is_caustic = copy->is_caustic;
        is_shadow = copy->is_shadow;
        is_transmitted = copy->is_transmitted;

        next = copy->next;
        list_next = copy->list_next;
    }


};