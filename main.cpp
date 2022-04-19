#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "GetBounds.h"
#include "GetListings.h"
#include "ConvexContains.h"
#include "avltree.h"
#include "ListingGraphics.h"

// Only for XCode build
//#include "ResourcePath.hpp"

int main(int, char const**)
{
    sf::Color listingColor = sf::Color(77,139,240);
    sf::Color selColor = sf::Color(77,139,240, 160);
    sf::Color unselColor = sf::Color(0,0,0,0);
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "SuperBNB New York");
    window.setFramerateLimit(60);
    unordered_map<std::string, sf::ConvexShape> NeighborhoodBounds = GetNeighborhoodBounds();
    
    vector<Listing> listings = GetListings();
    std::vector<Listing> small;
    small.push_back(listings[0]);
    AVLTree fullAVL(small);
    for(int i = 1; i < listings.size(); i++){
        fullAVL.insertListing(listings[i]);
    }
    
    // Set the Icon
    sf::Image icon;
    if (!icon.loadFromFile( "icon.png")) {
        return EXIT_FAILURE;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    // Load a sprite to display
    sf::Texture texture;
    if (!texture.loadFromFile( "NYCMAP.jpg")) {
        return EXIT_FAILURE;
    }
    sf::Font font;
    if (!font.loadFromFile( "sansation.ttf")) {
        return EXIT_FAILURE;
    }

    sf::Sprite sprite(texture);
    sprite.setPosition(520.f, 0.0);
    
    ListingGraphic selectedGraphic(GetListings()[0], 520.f, 886.f, font);
    selectedGraphic.base.setSize(sf::Vector2f(880.f, 200.f));
    selectedGraphic.setListing(GetListings()[0]);
    
    vector<ListingGraphic> listGraphics;
    for(int i = 0; i < 4; i++){
        listGraphics.push_back(ListingGraphic(GetListings()[i], 2, 280 + i * 200, font));
    }

    string selectedNeighborhood = "";
    int ticksSinceLastQuery = 10;
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed) {
                window.close();
            }
                        
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                
                
                sf::Vector2f position = sf::Vector2f(sf::Mouse::getPosition(window));
                auto iter = NeighborhoodBounds.begin();
                int conflicts = 0;
                for(; iter != NeighborhoodBounds.end(); iter++){
                    if(contains(iter->second, position)){
                        if(ticksSinceLastQuery > 3){
                            if(selectedNeighborhood != "")
                                NeighborhoodBounds[selectedNeighborhood].setFillColor(unselColor);
                            selectedNeighborhood = iter->first;
                            NeighborhoodBounds[iter->first].setFillColor(selColor);
                            conflicts++;
                            ticksSinceLastQuery = 0;
                        }
                    }
                }
                iter = NeighborhoodBounds.begin();
                // If the more accurate click detection does not get a result,
                // we switch over to a much more inaccurate detection so that we
                // can highlight at least one neighborhood when the user clicks
                // on a neighborhood
                if(conflicts == 0){
                    for(; iter != NeighborhoodBounds.end(); iter++){
                        if(iter->second.getGlobalBounds().contains(position)){
                            if(ticksSinceLastQuery > 3){ // Ensures that holding down click doesn't query > 1x
                                if(selectedNeighborhood != "") // Unhighlights previous selection
                                    NeighborhoodBounds[selectedNeighborhood].setFillColor(unselColor);
                                NeighborhoodBounds[iter->first].setFillColor(selColor);
                                selectedNeighborhood = iter->first;
                                conflicts++;
                                ticksSinceLastQuery = 0;
                            }
                        }
                    }
                }
                // After this, we can query our data structures with selectedNeighborhood
                if(conflicts != 0) { // Don't want to do unnecessary queries if clicking outside of a neighborhood
                    // Do data structure queries inside here
                    if(fullAVL.searchListings(0.0, 5.0, 100.0, 200.0, selectedNeighborhood).size() != 0){
                        std::vector<Listing> queryRes = fullAVL.searchListings(0.0, 5.0, 0.0, 10000.0, selectedNeighborhood);
                        for(int i = 0; i < 4; i++){
                            if(i == queryRes.size())
                                break;
                            listGraphics[i].setListing(queryRes[queryRes.size() - (1 + i)]);
                        }
                        selectedGraphic.setListing(queryRes[queryRes.size() - 1]);
                    }
                    else{
                        selectedGraphic.name.setString("No listings available for that query");
                    }
                }
                
                
            }
        }
        ticksSinceLastQuery++;
        
        // Clear screen
        window.clear();

        // Draw scene
        window.draw(sprite);
        for(int i = 0; i < listGraphics.size(); i++){
            window.draw(listGraphics[i].base);
            window.draw(listGraphics[i].name);
            window.draw(listGraphics[i].description1);
            window.draw(listGraphics[i].description2);

        }
        
        auto iter = NeighborhoodBounds.begin();
        for(; iter != NeighborhoodBounds.end(); iter++){
            window.draw(iter->second);
        }

        // Draw text
        window.draw(selectedGraphic.base);
        window.draw(selectedGraphic.name);
        window.draw(selectedGraphic.description1);
        window.draw(selectedGraphic.description2);



        // Update the window
        window.display();
    }

    return EXIT_SUCCESS;
}
