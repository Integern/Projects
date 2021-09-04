/*

    Joel Broad 2020

    PhotonMap class; this is KD-tree customised for my photon class.
    allows for storing, balancing and efficient searching of photons in a scene.

*/
#include "photon_map.h"


void PhotonMap::addPhoton(Photon *photon)
{
    Photon *ptr_photon = new Photon(); //create a copy; just storing its current direction, position etc..
    ptr_photon->clone(photon);
    ptr_photon->next = (Photon *)0;
    ptr_photon->list_next = (Photon *)0;

    //cout << "Adding photon " << ptr_photon->position << endl;

    photon_list.addPhoton(ptr_photon);
}

void PhotonMap::addPhoton(Photon *photon, Object *hit_object)
{
     Photon *ptr_photon = new Photon(); //create a copy; just storing its current direction, position etc..
    ptr_photon->clone(photon);
    ptr_photon->next = (Photon *)0;
    ptr_photon->list_next = (Photon *)0;

    float scalar = 0.5*hit_object->reflectrefract->reflection_inverse.rgbSum() + 0.5*hit_object->reflectrefract->refraction_inverse.rgbSum();
    ptr_photon->intensity.r *= scalar;
    ptr_photon->intensity.g *= scalar;
    ptr_photon->intensity.b *= scalar;

    photon_list.addPhoton(ptr_photon);   
}

void PhotonMap::balance()
{
    cout << "Balancing photon map.." << endl;
    if (photon_list.head == 0)
    {
        root = (PhotonNode *)0;
        cout << "No photons in this photonmap" << endl;
        return;
    }

    root = new PhotonNode(photon_list.head);
    root->depth = 0;

    Photon *current = root->photon->next;

    while (current != 0) //go through each photon in our list and run code to find its home in our tree.
    {
        balance_recurse(current);
        current = current->next;
    }

    cout << "Photon map balanced." << endl;
}

void PhotonMap::balance_recurse(Photon *photon)
{
    PhotonNode *node = new PhotonNode(photon); //create node with our given photon


    PhotonNode *current = root;
    PhotonNode *last;
    int depth = 0; //we use depth (how far down the tree) to decide with axis (X,Y,Z) to split on.
    bool _left = true;

    while (current != 0) //loop down the tree going through appropriate nodes
    {

        if (depth % 3 == 0) //X
        {

            //if the photon we're adding has a smaller value than the current node.. lower values go to the left subtree.
            if (current->photon->position.x > node->photon->position.x)
            {
                _left = true;
            } else {
                _left = false;
            }

        } else if (depth % 3 == 1) { //Y

            if (current->photon->position.y > node->photon->position.y)
            {
                _left = true;
            } else {
                _left = false;
            }

        } else { //Z

            if (current->photon->position.z > node->photon->position.z)
            {
                _left = true;
            } else {
                _left = false;
            }

        }


        depth++;
        node->depth = depth;   // current->depth + 1;


        //either search down the left/right subtree, or if it is a leaf node add it instead.
        if (_left) // <node> needs to go in the left subtree (value is lower than <current>)
        {

            if (current->left == (PhotonNode *)0)
            {
                current->left = node;
                current = (PhotonNode *)0;
            } else {
                current = current->left;
            }

        } else {

            if (current->right == (PhotonNode *)0)
            {
                current->right = node;
                current = (PhotonNode *)0;
            } else {
                current = current->right;
            }

        }

    }
}

float dist(Vertex origin, Photon *photon)
{
    return (origin - photon->position).len();
}

void PhotonMap::search_nearest(PhotonList *list, Vertex origin, int neighbour_count)
{
    search_nearest_recurse(root, list, origin, neighbour_count);
}

void PhotonMap::search_nearest_recurse(PhotonNode *node, PhotonList *list, Vertex origin, int neighbour_count)
{
    list->addOrderedPhoton(node->photon, origin, neighbour_count); //we test if this node is a contender for being one of the closest


    bool search_left = true;
    bool search_right = true;

    if (list->length < neighbour_count) //gotta populate this beauty, so keep the search up!
    {
        //do nothing and let both nodes be searched
    } else {

        if (node->depth % 3 == 0) //X
        {

            if ( (list->tail->position.x > origin.x) && (node->photon->position.x > list->tail->position.x) ) search_right = false;
            if ( (list->tail->position.x < origin.x) && (node->photon->position.x < list->tail->position.x) ) search_left = false;

        } else if (node->depth % 3 == 1) { //Y

            if ( (list->tail->position.y > origin.y) && (node->photon->position.y > list->tail->position.y) ) search_right = false;
            if ( (list->tail->position.y < origin.y) && (node->photon->position.y < list->tail->position.y) ) search_left = false;

        } else { //Z

            if ( (list->tail->position.z > origin.z) && (node->photon->position.z > list->tail->position.z) ) search_right = false;
            if ( (list->tail->position.z < origin.z) && (node->photon->position.z < list->tail->position.z) ) search_left = false;

        }

    }


    if (search_left && node->left) search_nearest_recurse(node->left, list, origin, neighbour_count);
    if (search_right && node->right) search_nearest_recurse(node->right, list, origin, neighbour_count);
}


void PhotonMap::search_slow(PhotonList *list, Vertex origin, float radius)
{
    Photon *current = photon_list.head;

    while (current != 0)
    {

        if ( photon_in_sphere(current, origin, radius) )
        {
            list->addPhoton(current, true);
        }

        current = current->list_next;
    }
}


void PhotonMap::search(PhotonList *list, Vertex origin, float radius)
{
    if (root == 0) return;
    search_recurse(root, list, origin, radius);
}

void PhotonMap::search_recurse(PhotonNode *node, PhotonList *list, Vertex origin, float radius)
{
    float eps = 0.0001f;

    if ( photon_in_sphere(node->photon, origin, radius) )
    {
        list->addPhoton(node->photon, true);
    }


    bool search_left = false;
    bool search_right = false;

    if (node->depth % 3 == 0) //X
    {

        if (node->photon->position.x > origin.x-eps) 
        {
            search_left = true;
            if (node->photon->position.x < origin.x+eps + radius) search_right = true;
        } else if (node->photon->position.x < origin.x+eps)
        {
            search_right = true;
            if (node->photon->position.x > origin.x-eps - radius) search_left = true;
        }

    } else if (node->depth % 3 == 1) { //Y

        if (node->photon->position.y > origin.y-eps) 
        {
            search_left = true;
            if (node->photon->position.y < origin.y+eps + radius) search_right = true;
        } else if (node->photon->position.y < origin.y+eps)
        {
            search_right = true;
            if (node->photon->position.y > origin.y-eps - radius) search_left = true;
        }

    } else { //Z

        if (node->photon->position.z > origin.z-eps) 
        {
            search_left = true;
            if (node->photon->position.z < origin.z+eps + radius) search_right = true;
        } else if (node->photon->position.z < origin.z+eps)
        {
            search_right = true;
            if (node->photon->position.z > origin.z-eps - radius) search_left = true;
        }

    }

    if (search_left && node->left != 0) search_recurse(node->left, list, origin, radius);
    if (search_right && node->right != 0) search_recurse(node->right, list, origin, radius);

}

bool PhotonMap::photon_in_sphere(Photon *photon, Vertex origin, float radius)
{
    if ( (photon->position - origin).len() <= radius )
    {
        return true;
    }

    return false;
}


void PhotonMap::print()
{

    if (root == 0) 
    {
        cout << "PhotonMap has no root node." << endl;
        return;
    }

    cout << "Printing photon map.." << endl;
    cout << "Root: " << root->photon->position << endl;
    if (root->left != (PhotonNode *)0) cout << "Root L:" << root->left->photon->position << endl;
    if (root->left->left != (PhotonNode *)0) cout << "Root LL:" << root->left->left->photon->position << endl;
    if (root->left->right != (PhotonNode *)0) cout << "Root LR:" << root->left->right->photon->position << endl;
    if (root->right != (PhotonNode *)0) cout << "Root R:" << root->right->photon->position << endl;
    if (root->right->left != (PhotonNode *)0) cout << "Root RL:" << root->right->left->photon->position << endl;
    if (root->right->right != (PhotonNode *)0) cout << "Root RR:" << root->right->right->photon->position << endl;
    if (root->right->right->right != (PhotonNode *)0) cout << "Root RRR:" << root->right->right->right->photon->position << endl;
}