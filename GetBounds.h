#ifndef GetBounds_h
#define GetBounds_h
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;
#endif /* GetBounds_h */

//#include "ResourcePath.hpp" // Only for XCode build

double latitudeToPixel(double latitude){
    return (40.94747646853455 - latitude) * 2188.952388560688549;
}
double longitudeToPixel(double longitude){
    return (74.27814874911644 + longitude) * 1674.346211879362761;
}
unordered_map<string, sf::ConvexShape> GetNeighborhoodBounds(){
    unordered_map<string, sf::ConvexShape> Neighborhoods;
    //ifstream file(resourcePath() + "NeighborhoodBounds.csv"); // Only for XCode build
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
        Neighborhoods[neighborhood] = n;
    }
    
    return Neighborhoods;
}
