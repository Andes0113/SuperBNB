#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "GetBounds.h"
#include "GetListings.h"
#include "ConvexContains.h"
#include "AVLMap.h"
#include "ListingGraphics.h"
#include "GenerateRandListings.h"

// Only for XCode build
//#include "ResourcePath.hpp"

int main(int, char const**)
{
    Listing nullListing;
    sf::Clock exeTimeClock;
    sf::Color listingColor = sf::Color(77,139,240);
    sf::Color selColor = sf::Color(77,139,240, 160);
    sf::Color unselColor = sf::Color(0,0,0,0);
    bool useMapAVL = false;
    
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "SuperBNB New York");
    window.setFramerateLimit(60);
    
    unordered_map<std::string, sf::ConvexShape> NeighborhoodBounds = GetNeighborhoodBounds();
    unordered_map<std::string, sf::ConvexShape> bounds = GetNeighborhoodBounds();
    vector<std::string> neighborhoods;
    for(auto iter = bounds.begin(); iter != bounds.end(); iter++){
        neighborhoods.push_back(iter->first);
    }
    
    vector<Listing> listings = GetListings();
    //vector<Listing> randListings = GenerateRandomListings(1000000);
    //listings.insert(listings.end(), randListings.begin(), randListings.end());
    
    AVLTree fullAVL(listings);
    AVLMap mapAVL(listings);
    
    double rating1 = 0.0;
    double rating2 = 5.0;
    double price1 = 0.0;
    double price2 = 10000.0;
    
    // Set the Icon
    sf::Image icon;
    if (!icon.loadFromFile("icon.png")) {
        return EXIT_FAILURE;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    // Load a sprite to display
    sf::Texture texture;
    if (!texture.loadFromFile("NYCMAP.jpg")) {
        return EXIT_FAILURE;
    }
    sf::Font font;
    if (!font.loadFromFile("sansation.ttf")) {
        return EXIT_FAILURE;
    }

    sf::Sprite NYCMap(texture);
    NYCMap.setPosition(520.f, 0.0);
    
    ListingGraphic selectedGraphic(GetListings()[0], 520.f, 886.f, font);
    selectedGraphic.base.setSize(sf::Vector2f(880.f, 200.f));
    selectedGraphic.setListing(GetListings()[0]);
    
    sf::RectangleShape deselect;
    deselect.setSize(sf::Vector2f(300, 50));
    deselect.setFillColor(listingColor);
    deselect.setOutlineColor(sf::Color::Black);
    deselect.setOutlineThickness(5);
    deselect.setPosition(530, 10);
    sf::Text dsText("Deselect Neighborhood", font, 25);
    dsText.setFillColor(sf::Color::White);
    dsText.setPosition(547, 17);
    
    sf::RectangleShape chooseFull;
    chooseFull.setSize(sf::Vector2f(150, 50));
    chooseFull.setFillColor(listingColor);
    chooseFull.setOutlineColor(sf::Color::Black);
    chooseFull.setOutlineThickness(5);
    chooseFull.setPosition(530, 65);
    sf::Text cfText("Full AVL", font, 25);
    cfText.setFillColor(sf::Color::White);
    cfText.setPosition(555, 75);
    
    sf::RectangleShape chooseMap;
    chooseMap.setSize(sf::Vector2f(150, 50));
    chooseMap.setFillColor(listingColor);
    chooseMap.setOutlineColor(sf::Color::Black);
    chooseMap.setOutlineThickness(5);
    chooseMap.setPosition(680, 65);
    sf::Text cmText("AVL Map", font, 25);
    cmText.setFillColor(sf::Color::White);
    cmText.setPosition(705, 75);
    
    sf::RectangleShape searchBox;
    searchBox.setSize(sf::Vector2f(510, 50));
    searchBox.setFillColor(listingColor);
    searchBox.setOutlineColor(sf::Color::Black);
    searchBox.setOutlineThickness(5);
    searchBox.setPosition(5, 225);
    sf::Text sbText("Search", font, 45);
    sbText.setFillColor(sf::Color::White);
    sbText.setPosition(187, 220);
    
    sf::RectangleShape elapsedTimeBox;
    elapsedTimeBox.setSize(sf::Vector2f(306, 50));
    elapsedTimeBox.setFillColor(listingColor);
    elapsedTimeBox.setOutlineColor(sf::Color::Black);
    elapsedTimeBox.setOutlineThickness(2);
    elapsedTimeBox.setPosition(527, 120);
    sf::Text etText("Elapsed Time: 0ms", font, 25);
    etText.setFillColor(sf::Color::White);
    etText.setPosition(550, 130);
    
    exeTimeClock.restart();
    std::vector<Listing> initQuery = fullAVL.searchListings(rating1, rating2, price1, price2);
    std::cout << exeTimeClock.getElapsedTime().asMilliseconds() << std::endl;
    etText.setString("Elapsed Time: " + std::to_string(exeTimeClock.getElapsedTime().asMilliseconds()) + "ms");
    
    vector<ListingGraphic> listGraphics;
    for(int i = 0; i < 4; i++){
        listGraphics.push_back(ListingGraphic(initQuery[initQuery.size() - (1 + i)], 2, 280 + i * 200, font));
    }
    
    string selectedNeighborhood = "";
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
                if(chooseFull.getGlobalBounds().contains(position)){
                    useMapAVL = false;
                }else if(chooseMap.getGlobalBounds().contains(position)){
                    useMapAVL = true;
                }else if(deselect.getGlobalBounds().contains(position)){
                    if(selectedNeighborhood != "")
                        NeighborhoodBounds[selectedNeighborhood].setFillColor(unselColor);
                    selectedNeighborhood = "";
                }
                else if(searchBox.getGlobalBounds().contains(position)){
                    exeTimeClock.restart();
                    std::vector<Listing> queryRes;
                    if(useMapAVL)
                        queryRes = mapAVL.search(rating1, rating2, price1, price2, selectedNeighborhood);
                    else
                        queryRes = fullAVL.searchListings(rating1, rating2, price1, price2, selectedNeighborhood);
                    std::cout << exeTimeClock.getElapsedTime().asMilliseconds() << std::endl;
                    etText.setString("Elapsed Time: " + std::to_string(exeTimeClock.getElapsedTime().asMilliseconds()) + "ms");
                    // After this, we can query our data structures with selectedNeighborhood
                    for(int i = 0; i < 4; i++){
                        if(i >= queryRes.size())
                            listGraphics[i].setListing(Listing());
                        else
                            listGraphics[i].setListing(queryRes[queryRes.size() - (1 + i)]);
                    }
                    if(queryRes.size() == 0){
                        Listing nullListing;
                        nullListing.name = "No listings found for that query";
                        nullListing.id = 0;
                        listGraphics[0].setListing(nullListing);
                    }
                    if(queryRes.size() > 0)
                        selectedGraphic.setListing(queryRes[queryRes.size() - 1]);
                    else
                        selectedGraphic.setListing(Listing());
                }
                else{
                    auto iter = NeighborhoodBounds.begin();
                    int conflicts = 0;
                    for(; iter != NeighborhoodBounds.end(); iter++){
                        if(contains(iter->second, position)){
                            if(selectedNeighborhood != "")
                                NeighborhoodBounds[selectedNeighborhood].setFillColor(unselColor);
                            selectedNeighborhood = iter->first;
                            NeighborhoodBounds[iter->first].setFillColor(selColor);
                            conflicts++;
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
                                if(selectedNeighborhood != "") // Unhighlights previous selection
                                    NeighborhoodBounds[selectedNeighborhood].setFillColor(unselColor);
                                NeighborhoodBounds[iter->first].setFillColor(selColor);
                                selectedNeighborhood = iter->first;
                                conflicts++;
                            }
                        }
                    }
                }
            }
        }
        
        // Clear screen
        window.clear();

        // Draw scene
        window.draw(NYCMap);
        for(int i = 0; i < listGraphics.size(); i++){
            if(listGraphics[i].list.id != -1){
                window.draw(listGraphics[i].base);
                window.draw(listGraphics[i].name);
                window.draw(listGraphics[i].description1);
                window.draw(listGraphics[i].description2);
            }
        }
        
        auto iter = NeighborhoodBounds.begin();
        for(; iter != NeighborhoodBounds.end(); iter++){
            window.draw(iter->second);
        }

        // Draw text
        window.draw(selectedGraphic.base);
        if(selectedGraphic.list.id != -1){
            window.draw(selectedGraphic.name);
            window.draw(selectedGraphic.description1);
            window.draw(selectedGraphic.description2);
        }
        
        window.draw(deselect);
        window.draw(chooseFull);
        window.draw(chooseMap);
        window.draw(searchBox);
        window.draw(elapsedTimeBox);
        
        window.draw(dsText);
        window.draw(cfText);
        window.draw(cmText);
        window.draw(sbText);
        window.draw(etText);
        

        // Update the window
        window.display();
    }

    return EXIT_SUCCESS;
}
