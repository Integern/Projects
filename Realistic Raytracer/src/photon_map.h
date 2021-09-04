/*

    Joel Broad 2020

    PhotonMap class; this is KD-tree customised for my photon class.
    allows for storing, balancing and efficient searching of photons in a scene.

*/
#pragma once

#include <math.h>
#include "photon.h"
#include "extents.h"
#include "photon_list.h"
#include "object.h"

// sub-class for a KD-tree node; has a value (Photon) and subtrees.
class PhotonNode { 
public:

    Photon *photon;
    PhotonNode *left;
    PhotonNode *right;
    int depth; 

    PhotonNode()
    {
        photon = (Photon *)0;
        left = (PhotonNode *)0;
        right = (PhotonNode *)0;
    }

    PhotonNode(Photon *PHOTON)
    {
        photon = PHOTON;
        left = (PhotonNode *)0;
        right = (PhotonNode *)0;       
    }

};


class PhotonMap {
public:

    PhotonList photon_list; //before balancing our tree, we store a long list of photons

    PhotonNode *root;

    
    PhotonMap()
    {
        photon_list = PhotonList();
        root = (PhotonNode *)0;
    }

    void addPhoton(Photon *photon);
    void addPhoton(Photon *photon, Object *hit_object); //takes the material of the object into account when storing intensity

    void balance(); //sets up and balances the KD tree
    void print(); //prints the top few nodes of the tree; for testing purposes

    void search(PhotonList *list, Vertex origin, float radius); //gets all photons in a radius around an origin
    void search_nearest(PhotonList *list, Vertex origin, int neighbour_count); //gets K nearest neighbours from origin

    void search_slow(PhotonList *list, Vertex origin, float radius); //does a linear O(n) search through the whole photon list, of photons in radius aroud origin

    bool photon_in_sphere(Photon *photon, Vertex origin, float radius); //lets us know if a given photon is inside a sphere around an origin

protected:

    void balance_recurse(Photon *photon);
    void search_recurse(PhotonNode *node, PhotonList *list, Vertex origin, float radius);
    void search_nearest_recurse(PhotonNode *node, PhotonList *list, Vertex origin, int neighbour_count);

};