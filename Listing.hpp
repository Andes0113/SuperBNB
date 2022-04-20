#ifndef Listing_hpp
#define Listing_hpp
#pragma once
#include <stdio.h>
#include <string>

#endif

struct Listing{
    int id, host_id, numHostListings;
    std::string name, hostName, borough, neighborhood, roomType;
    double price,rating;
};
