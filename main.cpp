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
    vector<std::string> neighborhoods;
    for(auto iter = NeighborhoodBounds.begin(); iter != NeighborhoodBounds.end(); iter++){
        neighborhoods.push_back(iter->first);
    }
    
    vector<Listing> listings = GetListings();
    vector<Listing> randListings = GenerateRandomListings(10000);
    listings.insert(listings.end(), randListings.begin(), randListings.end());
    
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

    // Load NYC Map jpg
    sf::Texture texture;
    if (!texture.loadFromFile("NYCMAP.jpg")) {
        return EXIT_FAILURE;
    }
    // Load font
    sf::Font font;
    if (!font.loadFromFile("sansation.ttf")) {
        return EXIT_FAILURE;
    }
    // Load interactive map of NYC
    sf::Sprite NYCMap(texture);
    NYCMap.setPosition(520.f, 0.0);
    
    ListingGraphic selectedGraphic(GetListings()[0], 520.f, 886.f, font);
    selectedGraphic.base.setSize(sf::Vector2f(880.f, 200.f));
    selectedGraphic.setListing(GetListings()[0]);
    
    // Grpahic for Deselect Neighborhood button
    sf::RectangleShape deselect;
    deselect.setSize(sf::Vector2f(300, 50));
    deselect.setFillColor(listingColor);
    deselect.setOutlineColor(sf::Color::Black);
    deselect.setOutlineThickness(5);
    deselect.setPosition(530, 10);
    sf::Text dsText("Deselect Neighborhood", font, 25);
    dsText.setFillColor(sf::Color::White);
    dsText.setPosition(547, 17);
    // Graphic for Full AVL button
    sf::RectangleShape chooseFull;
    chooseFull.setSize(sf::Vector2f(150, 50));
    chooseFull.setFillColor(listingColor);
    chooseFull.setOutlineColor(sf::Color::Black);
    chooseFull.setOutlineThickness(5);
    chooseFull.setPosition(530, 65);
    sf::Text cfText("Full AVL", font, 25);
    cfText.setFillColor(sf::Color::White);
    cfText.setPosition(555, 75);
    // Graphic for Map AVL Button
    sf::RectangleShape chooseMap;
    chooseMap.setSize(sf::Vector2f(150, 50));
    chooseMap.setFillColor(listingColor);
    chooseMap.setOutlineColor(sf::Color::Black);
    chooseMap.setOutlineThickness(5);
    chooseMap.setPosition(680, 65);
    sf::Text cmText("AVL Map", font, 25);
    cmText.setFillColor(sf::Color::White);
    cmText.setPosition(705, 75);
    // Graphic for elapsed time
    sf::RectangleShape elapsedTimeBox;
    elapsedTimeBox.setSize(sf::Vector2f(306, 50));
    elapsedTimeBox.setFillColor(listingColor);
    elapsedTimeBox.setOutlineColor(sf::Color::Black);
    elapsedTimeBox.setOutlineThickness(2);
    elapsedTimeBox.setPosition(527, 120);
    sf::Text etText("Elapsed Time: 0ms", font, 25);
    etText.setFillColor(sf::Color::White);
    etText.setPosition(550, 130);
    
    // Graphic for selected Neighborhood
    sf::RectangleShape selectedNeighborBox;
    selectedNeighborBox.setSize(sf::Vector2f(516, 51));
    selectedNeighborBox.setFillColor(listingColor);
    selectedNeighborBox.setOutlineColor(sf::Color::Black);
    selectedNeighborBox.setOutlineThickness(2);
    selectedNeighborBox.setPosition(2, 170);
    sf::Text snText("Neighborhood: None", font, 25);
    snText.setFillColor(sf::Color::White);
    snText.setPosition(5, 180);
    // Graphic for search box
    sf::RectangleShape searchBox;
    searchBox.setSize(sf::Vector2f(510, 50));
    searchBox.setFillColor(listingColor);
    searchBox.setOutlineColor(sf::Color::Black);
    searchBox.setOutlineThickness(5);
    searchBox.setPosition(5, 225);
    sf::Text sbText("Search", font, 45);
    sbText.setFillColor(sf::Color::White);
    sbText.setPosition(187, 220);
    
    
    // Populate the initial AirBNB listing menu with selection from every listing
    exeTimeClock.restart();
    std::vector<Listing> queryRes = fullAVL.searchListings(rating1, rating2, price1, price2);
    etText.setString("Elapsed Time: " + std::to_string(exeTimeClock.getElapsedTime().asMilliseconds()) + "ms");
        
    vector<ListingGraphic> listGraphics;
    for(int i = 0; i < 4; i++){
        listGraphics.push_back(ListingGraphic(queryRes[queryRes.size() - (1 + i)], 2, 280 + i * 200, font));
    }
    
    int queryIndex = 0;
    
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
            
            if(event.type == sf::Event::KeyPressed){
                if(event.key.code == sf::Keyboard::Up){
                    queryIndex--;
                    if(queryIndex < 0){
                        queryIndex = 0;
                    }else{
                        for(int i = 0; i < 4; i++){
                            if(i >= queryRes.size() - queryIndex)
                                listGraphics[i].setListing(Listing());
                            else
                                listGraphics[i].setListing(queryRes[queryRes.size() - (1 + i + queryIndex)]);
                        }
                        if(queryRes.size() == 0){
                            Listing nullListing;
                            nullListing.name = "No listings found for that query";
                            nullListing.id = 0;
                            listGraphics[0].setListing(nullListing);
                            selectedGraphic.setListing(nullListing);
                        }else{
                            selectedGraphic.setListing(queryRes[queryRes.size() - 1]);
                        }
                    }
                }
                else if(event.key.code == sf::Keyboard::Down){
                    if(queryRes.size() - (1 + queryIndex) > 0){
                        queryIndex++;
                    }
                    for(int i = 0; i < 4; i++){
                        if(i >= queryRes.size() - queryIndex)
                            listGraphics[i].setListing(Listing());
                        else
                            listGraphics[i].setListing(queryRes[queryRes.size() - (1 + i + queryIndex)]);
                    }
                    
                    if(queryRes.size() == 0){
                        Listing nullListing;
                        nullListing.name = "No listings found for that query";
                        nullListing.id = 0;
                        listGraphics[0].setListing(nullListing);
                        selectedGraphic.setListing(nullListing);
                    }else{
                        selectedGraphic.setListing(queryRes[queryRes.size() - 1]);
                    }
                }
            }
                        
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                sf::Vector2f position = sf::Vector2f(sf::Mouse::getPosition(window));
                for(int i = 0; i < listGraphics.size(); i++){
                    if(listGraphics[i].base.getGlobalBounds().contains(position)){
                        selectedGraphic.setListing(listGraphics[i].list);
                    }
                }
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
                    if(useMapAVL)
                        queryRes = mapAVL.search(rating1, rating2, price1, price2, selectedNeighborhood);
                    else
                        queryRes = fullAVL.searchListings(rating1, rating2, price1, price2, selectedNeighborhood);
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
                        selectedGraphic.setListing(nullListing);
                    }else{
                        selectedGraphic.setListing(queryRes[queryRes.size() - 1]);
                    }
                    queryIndex = 0;
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
                    if(selectedNeighborhood == "")
                        snText.setString("Neighborhood: None");
                    else
                        snText.setString("Neighborhood: " + selectedNeighborhood);
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
        window.draw(selectedNeighborBox);
        window.draw(elapsedTimeBox);
        
        window.draw(dsText);
        window.draw(cfText);
        window.draw(cmText);
        window.draw(sbText);
        window.draw(snText);
        window.draw(etText);
        

        // Update the window
        window.display();
    }

    return EXIT_SUCCESS;
}
