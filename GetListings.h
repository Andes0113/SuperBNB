#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include "Listing.hpp"
using namespace std;

//#include "ResourcePath.hpp" // Xcode build only

vector<Listing> GetListings(){
    vector<Listing> listings;
    //ifstream file(resourcePath() + "NYC2019Data.csv"); // Xcode build only
    ifstream file("NYC2019Data.csv"); // Normal Build
    string s;
    getline(file, s);
    while(!file.fail()){
        string id,name,host_id,host_name,burrow,neighborhood,latitude,longitude,room_type,price,number_of_reviews,numHostListings,rating;
        getline(file, id, ',');
        getline(file, name,',');
        if(name[0] == '"' && name.substr(1,name.length() - 1).find('"') == -1){
            getline(file, s, '"');
            name += s;
        }
        getline(file, host_id, ',');
        getline(file, host_name,',');
        while(std::count(host_name.begin(), host_name.end(), '"') % 2 == 1){
            getline(file, s, ',');
            host_name += s;
        }
        getline(file, burrow,',');
        getline(file, neighborhood,',');
        getline(file, latitude,',');
        getline(file, longitude,',');
        getline(file, room_type,',');
        getline(file, price,',');
        getline(file, number_of_reviews,',');
        getline(file, numHostListings,',');
        getline(file, rating);
        if(id != "") {
            Listing l;
            l.id = stoi(id);
            l.name = name;
            l.host_id = stoi(host_id);
            l.burrow = burrow;
            l.neighborhood = neighborhood;
            l.roomType = room_type;
            l.price = stod(price);
            l.numHostListings = stoi(numHostListings);
            l.rating = stod(rating);
            listings.push_back(l);
        }
    }
    return listings;
}
