#ifndef GetBounds_h
#define GetBounds_h
#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;
#endif /* GetBounds_h */

// #include "ResourcePath.hpp" // Only for XCode build

// We want this to fit to 886 px
double latitudeToPixel(double latitude){
    return (40.94747646853455 - latitude) * 1939.411816264770054;
}
// We want this to fit to 880 px// We want this to fit to 880 px
double longitudeToPixel(double longitude){
    return (74.27814874911644 + longitude) * 1473.42466645383923 + 520;
}

unordered_map<string, sf::ConvexShape> GetNeighborhoodBounds(){
    sf::Color usColor = sf::Color(0,0,0,0);
    unordered_map<string, sf::ConvexShape> Neighborhoods;
    // ifstream file(resourcePath() + "NeighborhoodBounds.csv"); // Only for XCode build
   ifstream file("NeighborhoodBounds.csv"); // Regular build
    string s;
    getline(file, s);
    int numLines = 0;
    while(!file.fail()) {
        getline(file, s, '(');
        string coords;
        getline(file, coords, ')');
        if(coords.empty())
            break;
        coords = coords.substr(1);
        string neighborhood;
        getline(file, neighborhood);
        neighborhood = neighborhood.substr(3);
        if(neighborhood != "Midwood")
            neighborhood = neighborhood.substr(0, neighborhood.size() - 1);
        vector<sf::Vector2f> coordinates;
        while(coords.find(',') != string::npos){
            string newCoord = coords.substr(0, coords.find(','));
            coords = coords.substr(coords.find(',') + 2);
            double longitude = stod(newCoord.substr(0, newCoord.find(' ')));
            newCoord = newCoord.substr(newCoord.find(' ') + 1);
            double latitude = stod(newCoord);
            double x = longitudeToPixel(longitude);
            double y = latitudeToPixel(latitude);
            sf::Vector2f c(x,y);
            coordinates.push_back(c);
        }
        // Operations on coordinates
        sf::ConvexShape n(coordinates.size());
        for(int i = 0; i < coordinates.size(); i++){
            n.setPoint(i, coordinates[i]);
        }
        n.setFillColor(usColor);
        n.setOutlineColor(sf::Color::Black);
        n.setOutlineThickness(1.1);
        if(neighborhood == "Theater District/Times Square")
            neighborhood = "Theater District"; // Fixes discrepancy between files
        Neighborhoods[neighborhood] = n;
    }
    
    return Neighborhoods;
}
