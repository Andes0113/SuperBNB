#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "GetBounds.h"
#include "GetListings.h"
#include "ConvexContains.h"

// Only for XCode build
//#include "ResourcePath.hpp"

int main(int, char const**)
{
    sf::Color listingColor = sf::Color(77,139,240);
    sf::Color selColor = sf::Color(77,139,240, 160);
    sf::Color unselColor = sf::Color(0,0,0,0);
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(1600, 900), "SFML window");
    window.setFramerateLimit(60);
    unordered_map<std::string, sf::ConvexShape> NeighborhoodBounds = GetNeighborhoodBounds();
    vector<Listing> listings = GetListings();

    // Load a sprite to display
    sf::Texture texture;
    if (!texture.loadFromFile("cute_image.jpg")) {
        return EXIT_FAILURE;
    }

    // Create a graphical text to display
    sf::Font font;
    if (!font.loadFromFile("sansation.ttf")) {
        return EXIT_FAILURE;
    }

    sf::Sprite sprite(texture);
        sprite.setPosition(520.f, 0.0);
    
    Listing l = listings[0];
    sf::RectangleShape selectedListing;
    selectedListing.setFillColor(listingColor);
    selectedListing.setSize(sf::Vector2f(880.f,200.f));
    selectedListing.setPosition(520.f, 880.f);
    sf::Text text(l.name, font, 30);
    text.setFillColor(sf::Color::White);
    text.setPosition(520.f, 880.f);

    string selectedNeighborhood = "";
    int ticksSinceLastQuery = 10;

    // Start the game loop
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
                    std::cout << selectedNeighborhood << std::endl;
                }
                
                
            }
        }
        ticksSinceLastQuery++;

        // Clear screen
        window.clear();

        // Draw the sprite
        window.draw(sprite);
        window.draw(selectedListing);

        auto iter = NeighborhoodBounds.begin();
        for(; iter != NeighborhoodBounds.end(); iter++){
            window.draw(iter->second);
        }

        // Draw the string
        window.draw(text);

        // Update the window
        window.display();
    }

    return EXIT_SUCCESS;
}
