#ifndef Listing_h
#define Listing_h
#pragma once
#include <stdio.h>
#include <string>

#endif

struct Listing{
    int id, host_id, numHostListings;
    std::string name, hostName, borough, neighborhood, roomType;
    double price,rating;
};
