/*

    Joel Broad 2020

    A class that helps manipulate having a list of Photons; linked list (not doubly)

*/
#pragma once

#include <math.h>
#include "extents.h"
#include "photon.h"

class PhotonList {
public:

    Photon *head;
    Photon *tail;

    int length;

    int caustic_count; //keeps track of how many photons of each type have been added to the list (does not register when photons get removed when PhotonList has a limited length (so photons get removed)
    int transmit_count;
    int shadow_count;

    PhotonList()
    {
        head = (Photon *)0;
        tail = (Photon *)0;
        length = 0;
    }


    void clear()
    {
        head = (Photon *)0;
        tail = (Photon *)0;
        length = 0;

        caustic_count = 0;
        transmit_count = 0;
        shadow_count = 0;
    }

    void addPhoton(Photon *photon)
    {
        addPhoton(photon, false);
    }

    //adds a photon to the tail photon
    void addPhoton(Photon *photon, bool ignore_list_next)
    {
        photon->next = (Photon *)0; //new
        if (ignore_list_next == false) photon->list_next = (Photon *)0;

        if (head == 0)
        {
            head = photon;
            tail = photon;
        } else {
            tail->next = photon;
            if (ignore_list_next == false) tail->list_next = photon;

            tail = photon;
        }

        length++;      


        
        if (photon->is_caustic) caustic_count++;
        if (photon->is_shadow) shadow_count++;
        if (photon->is_transmitted) transmit_count++;

    }


    float getBoundingSphereArea() //returns the area of the smallest sphere S that contains all stored photons
    {
        if (head == 0 ) return 10000000; //no sphere, so make it stupid big to negate any contribution

        // won't lie, we're going to cheat and use a cube. we'll take the longest edge and take that as a diameter. just to get in the range of magnitude.
        Extents fake_sphere = Extents();

        Photon *current = head;
        while (current != 0)
        {
            fake_sphere.introduceVertex(current->position);

            current = current->next;
        }

        float edge_x = (fake_sphere.max.x - fake_sphere.min.x);
        float edge_y = (fake_sphere.max.y-fake_sphere.min.y);
        float edge_z = (fake_sphere.max.z-fake_sphere.min.z);

        float edge_long = edge_x;
        if (edge_y > edge_long) edge_long = edge_y;
        if (edge_z > edge_long) edge_long = edge_z;

        return 3.141 * (edge_long/2) * (edge_long/2); //generate sphere volume using longest edge as the diameter
    }
    

    //when storing photons with this method, head is closest and tail is furthest from given origin. if <length> exceeds max length, culls photons furthest away. 
    //assumes that in a given PhotonList, that the origin given is the same each time.
    void addOrderedPhoton(Photon *photon, Vertex origin, int max_length)
    {
        photon->next = (Photon *)0; //new

        if (head == 0) //edge case
        {
            addPhoton(photon);
            return;
        }


        Photon *current_last = (Photon *)0;
        Photon *current = head;

        while (current != 0)
        {
            if (dist(origin, current) < dist(origin, photon))
            {

                if (current == tail && length < max_length) //the new photon is the furthest away, but there is space
                {
                    addPhoton(photon);
                    break;
                }

                current_last = current;
                current = current->next;
            } else { //this photon should go before current

                length++; //adding a photon so increase length

                if (current == head)
                {
                    head = photon;
                    head->next = current;
                } else {
                    current_last->next = photon;
                    photon->next = current;

                    if (current == tail) tail = photon;

                }

                break;
            }
            
        }


        //now check if we need to cull any photons (list is too long)
        int ranking = 0;
        current = head;
        while (current != 0)
        {
            ranking++;

            if (ranking == max_length) //current is the last photon in our list
            {
                current->next = (Photon *)0;
                current = tail;
                break;
            }

            current = current->next;
        }


    }

    //3D distance between given origin and photons position
    float dist(Vertex origin, Photon *photon)
    {
        return (origin - photon->position).len();
    }

};