#ifndef GenerateRandListings_h
#define GenerateRandListings_h
#include <SFML/Graphics.hpp>
#include "Listing.hpp"
#include "GetListings.h"
#include <math.h>
#include <stdlib.h>
#include "GetBounds.h"

#endif /* GenerateRandListings_h */

std::vector<Listing> GenerateRandomListings(int n){
    std::vector<Listing> presets = GetListings();
    unordered_map<std::string, sf::ConvexShape> bounds = GetNeighborhoodBounds();
    vector<std::string> neighborhoods;
    for(auto iter = bounds.begin(); iter != bounds.end(); iter++){
        neighborhoods.push_back(iter->first);
    }
    std::vector<Listing> listings;
    for(int i = 0; i < n; i++){
        Listing l;
        l.id = rand() % presets.size();
        l.host_id = rand() % presets.size();
        l.name = presets[rand() % presets.size()].name;
        l.hostName = presets[rand() % presets.size()].hostName;
        l.burrow = "";
        l.neighborhood = neighborhoods[i % neighborhoods.size()];
        l.roomType = presets[rand() % presets.size()].roomType;
        l.price = presets[rand() % presets.size()].price;
        l.rating = presets[rand() % presets.size()].rating;
        if(l.rating > 4.5){
            l.rating = presets[rand() % presets.size()].rating;
        }
        listings.push_back(l);
    }
    return listings;
}
