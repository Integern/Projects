/*

    Joel Broad 2020

    test code for testing functionality of code i've written

*/
#include "framebuffer.h"
#include "ray.h"
#include "hit.h"
#include "polymesh.h"
#include "sphere.h"
#include "light.h"
#include "directional_light.h"
#include "material.h"
#include "phong.h"
#include "box.h"
#include "reflectrefract.h"
#include "photon.h"
#include "photon_map.h"

#include "extents.h"
#include "vertex.h"
#include "photon_list.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include <random>

using namespace std;

//      g++ -o test test.cpp photon_map.cpp -lm

void temp();
void photon_map_nearest_test(int neighbour_count, int photon_count);
void photon_list_nearest_test(int neighbour_count, int photon_count);
void photon_map_search_test(int photon_count);
void rands(int N);

int main(int argc, char *argv[])
{
  cout << "test.cpp running.." << endl;
  srand(time(NULL));

  photon_map_search_test(50000);
  //rands(1000);
  //photon_list_nearest_test(3, 100);
  //temp();
  //for (int j = 0; j < 10; j++) cout << endl;

  //photon_map_nearest_test(5, 100);
}





void rands(int N)
{

  double PI = 3.141;
  mt19937 rng(time(NULL));

  uniform_real_distribution<float> gen(0, 1); // uniform, unbiased
  float r = gen(rng);

  cout << r;
}

void temp()
{
  PhotonMap map = PhotonMap();
  
  PhotonList *list = new PhotonList(); //ordered list
  PhotonList *list_brute = new PhotonList();

  Extents ext = Extents(Vertex(-10, -10, -10), Vertex(10, 10, 10));
  Vertex origin = Vertex(0,0,0);

  for (int i = 0; i < 5; i++)
  {
    Vertex pos;
    ext.get_random_point(pos);
    Photon *photon = new Photon();
    photon->position = pos;
    
    cout << "Photon " << i << " position: " << photon->position << endl;


    map.addPhoton(photon);
  }

  cout << "finished" << endl;
}

float dist(Vertex v0, Vertex v1)
{
  return (v0-v1).len();
}

void photon_map_nearest_test(int neighbour_count, int photon_count)
{
  PhotonMap map = PhotonMap();

  PhotonList *list = new PhotonList();
  Extents ext = Extents(Vertex(-10, -10, -10), Vertex(10, 10, 10));
  Vertex origin = Vertex(0,0,0);

  for (int i = 0; i < photon_count; i++)
  {
    Vertex pos;
    ext.get_random_point(pos);
    Photon *photon = new Photon(pos);
    
    cout << "Photon " << i << " position: " << photon->position << " (dist: " << list->dist(origin, photon) << ")" << endl;

    list->addOrderedPhoton(photon, origin, neighbour_count);
    map.addPhoton(photon);
  }



  cout << "LIST Nearest " << neighbour_count << " photons: " << endl;

  Photon *current = list->head;
  while (current != 0)
  {
    cout << "Photon: " << current->position << " (dist: " << list->dist(origin, current) << ")" << endl;
    current = current->next;
  }


  
  map.balance();
  PhotonList *search_list = new PhotonList();
  map.search_nearest(search_list, origin, neighbour_count);

  cout << "SEARCH_LIST Nearest " << neighbour_count << " photons: " << endl;

  current = search_list->head;
  while (current != 0)
  {
    cout << "Photon: " << current->position << " (dist: " << search_list->dist(origin, current) << ")" << endl;
    current = current->next;
  } 


}

void photon_list_nearest_test(int neighbour_count, int photon_count)
{
  PhotonList *list = new PhotonList();
  Extents ext = Extents(Vertex(-10, -10, -10), Vertex(10, 10, 10));
  Vertex origin = Vertex(0,0,0);

  for (int i = 0; i < photon_count; i++)
  {
    Vertex pos;
    ext.get_random_point(pos);
    Photon *photon = new Photon(pos);
    
    cout << "Photon " << i << " position: " << photon->position << " (dist: " << list->dist(origin, photon) << ")" << endl;

    list->addOrderedPhoton(photon, origin, neighbour_count);
  }



  cout << "Nearest " << neighbour_count << " photons: " << endl;

  Photon *current = list->head;
  while (current != 0)
  {
    cout << "Photon: " << current->position << " (dist: " << list->dist(origin, current) << ")" << endl;
    current = current->next;
  }


}

void photon_map_search_test(int photon_count)
{

  float radius = 5;
  Vertex origin = Vertex(0, 0, 0);



  PhotonMap map = PhotonMap();
  Extents ext = Extents(Vertex(-10, -10, -10), Vertex(10, 10, 10));

  for (int i = 0; i < photon_count; i++)
  {
    Vertex pos;
    ext.get_random_point(pos);
    Photon *photon = new Photon(pos);
    
/*    
    float dist = map.photon_list.dist(origin, photon);
    cout << "Photon " << i << " position: " << photon->position << "  (dist: " << dist << " )";
    if (dist <= radius) cout << " DING DING DING";
    cout << endl;
*/

    map.addPhoton(photon);
  }

  map.balance();
  map.print();


  int tests = 250;

  for (int j = 0; j < tests; j++)
  {
    ext.get_random_point(origin);

    PhotonList *list = new PhotonList();
    int count_slow;
    int count_fast;


    map.search_slow(list, origin, radius);
    count_slow = list->length;

    list->clear();
    map.search(list, origin, radius);
    count_fast = list->length;


    cout << "Slow: " << count_slow << "  Fast: " << count_fast << "   Origin: " << origin << "   (" << int(100 * (float(count_fast)/float(count_slow))) << "%) " << endl;

  }

}


