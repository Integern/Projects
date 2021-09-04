/*

    Joel Broad 

    Settings used for my raytracer

*/
#pragma once

#include <math.h>


class Settings {
public:

    bool do_plot_photons; //if to plot photon positions over the image
    float plot_photons_radius; //search radius

    float frame_buffer_size;
    float photon_count;

    int current_x; //current pixel information that can be passed to other functions for testing purposes
    int current_y;


    int search_neighbour_count; //if searching K nearest neighbours, use this value

    float search_radius;
    float search_radius_closeness_scaling; //how the contribution of a photon scales off the further from the origin it is
    float search_radius_neighbour_baseline; //will scale the amount of photons found to this amount. 0 ignores it.

    bool search_using_radius; //if true, will use radius search. else uses closest neighbours
    bool search_using_radius_fast; //if false, will use search_slow (slower BUT will get every photon, was used in testing)


    float photon_global_flux; //how many true photons each global/caustic photon represents
    float photon_caustic_flux;

    float caustic_min_contribution;

    bool update_photon_intensity_after_storage; //if true, a photon hits a surface. we store it. we update the colour from ambient of hit. if false, we colour the photon what it hits before storing it.

    float russian_roulette_tweak; //lower it is, the more chance a photon has to transmit/reflect. def=0.4

    float ambient_contribution; //how much ambient from an object actually gets added to pixel colour

    int shadow_divide_effect; //if a pixel has any shadow photons nearby, it will decrease the contribution of photons by this amount (enhance shadow effect)


};