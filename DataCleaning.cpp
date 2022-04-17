#include <fstream>
#include <iostream>
#include <string>
#include <math.h>
#include <algorithm>
using namespace std;

// To clean AB_NYC_2019.csv, run this file in the same folder. Will output in NYC2019Data.csv
// Don't care about columns: minimum_nights, last_review, reviews_per_month, availability_365
// Ratings are randomly generated

int main(){
    ifstream file("AB_NYC_2019.csv");
    ofstream outfile("NYC2019Data.csv");
    string header;
    getline(file, header);
    outfile << "id,name,host_id,host_name,burrow,neighborhood,latitude,longitude,room_type,price,number_of_reviews,numHostListings,rating\n";
    while(!file.fail()){
        string s;
        string id, name, host_id, host_name, burrow, neighborhood, latitude, longitude, room_type, price, number_of_reviews, numHostListings;
        getline(file, id,',');
        if(id != ""){
            cout << "id: " << id << endl;
            getline(file, name,',');
            while(std::count(name.begin(), name.end(), '"') % 2 == 1){
                getline(file, s, ',');
                name += s;
            }
            cout << "name: " << name << endl;
            getline(file, host_id,',');
            cout << "host_id: " << host_id << endl;
            getline(file, host_name,',');
            while(std::count(host_name.begin(), host_name.end(), '"') % 2 == 1){
                getline(file, s, ',');
                host_name += s;
            }
            cout << "host_name: " << host_name << endl;
            getline(file, burrow,',');
            cout << "neighbourhood_group: " << burrow << endl;
            getline(file, neighborhood,',');
            cout << "neighbourhood: " << neighborhood << endl;
            getline(file, latitude,',');
            while (std::count(neighborhood.begin(), neighborhood.end(), '"') % 2 == 1) {
                getline(file, s, ',');
                neighborhood += s;
            }
            if (neighborhood.find("Bay Terrace Staten Island") != string::npos)
                neighborhood = "Bay Terrace";
            cout << "latitude: " << latitude << endl;
            getline(file, longitude,',');
            cout << "longitude: " << longitude << endl;
            getline(file, room_type,',');
            cout << "room_type: " << room_type << endl;
            getline(file, price,',');
            cout << "price: " << price << endl;
            getline(file, s,',');
            cout << "minimum_nights: " << s << endl;
            getline(file, number_of_reviews,',');
            cout << "number_of_reviews: " << number_of_reviews << endl;
            getline(file, s,',');
            cout << "last_review: " << s << endl;
            getline(file, s,',');
            cout << "reviews_per_month: " << s << endl;
            getline(file, numHostListings,',');
            cout << "calculated_host_listings_count: " << numHostListings << endl;
            getline(file, s);
            cout << "availability_365: " << s << endl;
            bool validLine = !id.empty() && !name.empty() && !host_id.empty() && !burrow.empty() && !neighborhood.empty() && !numHostListings.empty();
            validLine = validLine && !latitude.empty() && !longitude.empty() && !room_type.empty() && !price.empty() && !numHostListings.empty();
            if(validLine){
                string rating;
                if(number_of_reviews == "0")
                    rating = "0.00";
                else{
                    double r = sqrt((stoi(id) % 500) * 5)/10;
                    if(r == 0)
                        r = 5;
                    if(r < 1)
                        r+=3.5;
                    rating = to_string(r);
                    rating = rating.substr(0, rating.find('.') + 3);
                }
                cout << "rating: " << rating << endl;
                // id, name, host_id, host_name, burrow, neighborhood, latitude, longitude, room_type, price, number_of_reviews, numHostListings;
                outfile << id << "," << name << "," << host_id << "," << host_name << ",";
                outfile << burrow << "," << neighborhood << "," << latitude << "," << longitude << ",";
                outfile << room_type << "," << price << "," << number_of_reviews << "," << numHostListings << "," << rating << "\n";
            }
        }
    }
}