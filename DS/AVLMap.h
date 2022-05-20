#ifndef AVLMap_h
#define AVLMap_h
#include <iostream>
#include <algorithm>
#include <utility>
#include <vector>
#include "../Listings/Listing.h"
#include "AVLTree.h"
#include <unordered_map>

#endif /* AVLMap_h */



class AVLMap {
private:
    std::unordered_map<std::string, AVLTree> map;
public:
    AVLMap(std::vector<Listing> listings){
        for(int i = 0; i < listings.size(); i++){
            if(map.count(listings[i].neighborhood)){
                map[listings[i].neighborhood].insertListing(listings[i]);
            }else{
                std::vector<Listing> newListing;
                newListing.push_back(listings[i]);
                map[listings[i].neighborhood] = AVLTree(newListing);
            }
        }
    }
    std::vector<Listing> search(double rating1,double rating2, double price1, double price2, std::string neighborhood = ""){
        if(neighborhood != ""){
            return map[neighborhood].searchListings(rating1, rating2, price1, price2, neighborhood);
        }else{
            std::vector<Listing> listings;
            for(auto iter = map.begin(); iter != map.end(); iter++){
                std::vector<Listing> newListings = iter->second.searchListings(rating1, rating2, price1, price2, neighborhood);
                listings.insert(listings.begin(), newListings.begin(), newListings.end());
            }
            std::sort(listings.begin(), listings.end(), [](const Listing& lhs, const Listing& rhs){
                return lhs.rating < rhs.rating;
            });
            return listings;
        }
    }
};
