#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>
#include "Collisions/GetBounds.h"
#include "Listings/GetListings.h"
#include "Collisions/ConvexContains.h"
#include "DS/AVLMap.h"
#include "Graphics/ListingGraphics.h"
#include "Graphics/NeighborSearchGraphics.h"
#include "Graphics/TextField.h"
#include "Listings/GenerateRandListings.h"

// Only for XCode build
// #include "ResourcePath.hpp"

int main(int, char const**)
{
    Listing nullListing;
    sf::Clock exeTimeClock;
    sf::Color listingColor = sf::Color(77,139,240);
    sf::Color selColor = sf::Color(77,139,240,160);
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
    vector<Listing> randListings = GenerateRandomListings(200000);
    listings.insert(listings.end(), randListings.begin(), randListings.end());
    
    AVLTree fullAVL(listings);
    AVLMap mapAVL(listings);
    
    double rating1 = 0.0;
    double rating2 = 5.0;
    double price1 = 0.0;
    double price2 = 1000.0;
    
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
        
    sf::RectangleShape fieldBox;
    fieldBox.setSize(sf::Vector2f(516, 168));
    fieldBox.setFillColor(listingColor);
    fieldBox.setOutlineColor(sf::Color::Black);
    fieldBox.setOutlineThickness(2);
    fieldBox.setPosition(2, 2);
    
    std::vector<TextField*> textFields;
    
    // Rating range selection field
    sf::Text ratingText("Rating:", font, 40);
    ratingText.setFillColor(sf::Color::White);
    ratingText.setPosition(58, 20);
    TextField ratingField1(40, 90, "0.0", font);
    TextField ratingField2(140, 90, "5.0", font);
    ratingField1.maxText = 3;
    ratingField2.maxText = 3;
    textFields.push_back(&ratingField1);
    textFields.push_back(&ratingField2);
    
    // Price range selection field
    sf::Text priceText("Price:", font, 40);
    priceText.setFillColor(sf::Color::White);
    priceText.setPosition(310, 20);
    TextField priceField1(240, 90, "0.00", font);
    TextField priceField2(380, 90, "1000.0", font);
    priceField1.setSize(100, 50);
    priceField2.setSize(100, 50);
    textFields.push_back(&priceField1);
    textFields.push_back(&priceField2);
    
    // Search through neighborhoods
    TextField neighborSearch(1405, 5, "", font);
    neighborSearch.setSize(510, 50);
    neighborSearch.maxText = 25;
    textFields.push_back(&neighborSearch);
    sf::RectangleShape neighborSearchBox;
    neighborSearchBox.setSize(sf::Vector2f(510, 43));
    neighborSearchBox.setFillColor(listingColor);
    neighborSearchBox.setOutlineColor(sf::Color::Black);
    neighborSearchBox.setOutlineThickness(5);
    neighborSearchBox.setPosition(1405, 60);
    sf::Text nsText("Search Neighborhoods", font, 30);
    nsText.setFillColor(sf::Color::White);
    nsText.setPosition(1410, 62);
    vector<std::string> nQueryRes = neighborhoods;
    // Neighborhood box graphics
    std::vector<NeighborhoodBox> nBoxes;
    for(int i = 0; i < 18; i++){
        nBoxes.push_back(NeighborhoodBox(nQueryRes[i], 1402, (i + 2) * 54 + 2, font));
    }
    
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
    
    // Selected Listing Graphic
    ListingGraphic selectedGraphic(GetListings()[0], 520.f, 886.f, font);
    selectedGraphic.base.setSize(sf::Vector2f(880.f, 200.f));
    selectedGraphic.setListing(queryRes[queryRes.size() - 1]);
    
    // Populate listings on startup
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
            
            if (event.type == sf::Event::TextEntered)
            {
                for(auto field: textFields){
                    std::string fString = field->GetText();
                    if(event.text.unicode == '\b'){
                        if(field->selected){
                            if(fString.size() > 0)
                                fString.erase(fString.size() -1, 1);
                            field->SetText(fString);
                        }
                    }else{
                        char key = static_cast<char>(event.text.unicode);
                        if(field->selected && field->GetText().size() <= field->maxText){
                            if(field != textFields[4])
                                if(!((event.text.unicode > 47 && event.text.unicode < 58) || key == '.'))
                                    break;
                            field->SetText(field->GetText() + key);
                        }
                    }
                }
            }
            
            if(event.type == sf::Event::KeyPressed){
                // Use arrow keys to move through query results
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
                    }
                }
            }
            // If left click
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                sf::Vector2f position = sf::Vector2f(sf::Mouse::getPosition(window));
                // Check if listing was clicked
                for(int i = 0; i < listGraphics.size(); i++){
                    if(listGraphics[i].base.getGlobalBounds().contains(position)){
                        if(selectedNeighborhood != listGraphics[i].list.neighborhood){
                            NeighborhoodBounds[selectedNeighborhood].setFillColor(unselColor);
                            selectedNeighborhood = listGraphics[i].list.neighborhood;
                            NeighborhoodBounds[selectedNeighborhood].setFillColor(selColor);
                        }
                        selectedGraphic.setListing(listGraphics[i].list);
                    }
                }
                // Check if a neighborhood's search box was clicked
                for(int i = 0; i < nBoxes.size(); i++){
                    if(nBoxes[i].base.getGlobalBounds().contains(position)){
                        if(selectedNeighborhood != "")
                            NeighborhoodBounds[selectedNeighborhood].setFillColor(unselColor);
                        selectedNeighborhood = nBoxes[i].neighborhood;
                        NeighborhoodBounds[selectedNeighborhood].setFillColor(selColor);
                    }
                }
                // Check if text field was clicked
                for(auto f : textFields){
                    f->Deselect();
                    if(f->field.getGlobalBounds().contains(position))
                        f->Select();
                }
                // Check if Full AVL / Map AVL was clicked
                if(chooseFull.getGlobalBounds().contains(position)){
                    useMapAVL = false;
                }else if(chooseMap.getGlobalBounds().contains(position)){
                    useMapAVL = true;
                }
                // Check if deselect neighborhoods button clicked
                else if(deselect.getGlobalBounds().contains(position)){
                    snText.setString("Neighborhood: None");
                    if(selectedNeighborhood != "")
                        NeighborhoodBounds[selectedNeighborhood].setFillColor(unselColor);
                    selectedNeighborhood = "";
                }
                // Check if neighborhood search button was clicked
                else if(neighborSearchBox.getGlobalBounds().contains(position)){
                    std::string nQuery = neighborSearch.GetText();
                    nQueryRes.clear();
                    for(int i = 0; i < neighborhoods.size(); i++){
                        if(neighborhoods[i].find(nQuery) != string::npos)
                            nQueryRes.push_back(neighborhoods[i]);
                    }
                    // Checks if a neighborhood box was clicked
                    for(int i = 0; i < 18; i++){
                        if(i >= nQueryRes.size())
                            nBoxes[i].SetNeighborhood("");
                        else
                            nBoxes[i].SetNeighborhood(nQueryRes[i]);
                    }
                }
                // Checks if search box for listings was clicked
                else if(searchBox.getGlobalBounds().contains(position)){
                    // Querying our data set
                    rating1 = ParseDouble(ratingField1);
                    rating2 = ParseDouble(ratingField2);
                    price1 = ParseDouble(priceField1);
                    price2 = ParseDouble(priceField2);
                    exeTimeClock.restart(); // Keeps track of time a query takes
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
                        // If no results from query, say that no listings were found
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
                    // Check if a neighborhood on NYC map was cilcked
                    auto iter = NeighborhoodBounds.begin();
                    int conflicts = 0;
                    for(; iter != NeighborhoodBounds.end(); iter++){
                        // Uses convex shape geometry to detect if a click is inside a neighborhood
                        // ConvexContains.h
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
        
        
        window.draw(fieldBox);
        window.draw(ratingText);
        window.draw(priceText);
        for(auto field: textFields){
            window.draw(field->field);
            window.draw(field->text);
        }
        
        for(auto nb: nBoxes){
            if(nb.neighborhood != ""){
                window.draw(nb.base);
                window.draw(nb.text);
            }
        }
        
        window.draw(neighborSearchBox);
        window.draw(nsText);
        // Update the window
        window.display();
    }

    return EXIT_SUCCESS;
}
