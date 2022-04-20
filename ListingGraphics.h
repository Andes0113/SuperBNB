#ifndef ListingGraphics_h
#define ListingGraphics_h

#include "Listing.hpp"
#include <SFML/Graphics.hpp>

#endif /* ListingGraphics_h */


struct ListingGraphic{
    Listing list;
    sf::RectangleShape base;
    sf::Text name;
    sf::Text description1;
    sf::Text description2;
    ListingGraphic(Listing l, int x, int y, sf::Font& font){
        base.setSize(sf::Vector2f(516.f,200.f));
        base.setFillColor(sf::Color(77,139,240));
        base.setPosition(x, y);
        base.setOutlineColor(sf::Color::Black);
        base.setOutlineThickness(2.0);
        name.setCharacterSize(30);
        name.setFillColor(sf::Color::White);
        name.setPosition(x + 10, y + 2);
        name.setFont(font);
        description1.setCharacterSize(30);
        description1.setFillColor(sf::Color::White);
        description1.setPosition(x + 10, y + 45);
        description1.setFont(font);
        description2.setCharacterSize(30);
        description2.setFillColor(sf::Color::White);
        description2.setPosition(description2.getGlobalBounds().width + 10, y + 45);
        description2.setFont(font);
        setListing(l);
    }
    void setListing(Listing l){
        list = l;
        name.setString(l.name);
        int desc1X = base.getGlobalBounds().left + 10;
        if(name.getGlobalBounds().width > base.getGlobalBounds().width - 10){
            name.setString(l.name.substr(0, l.name.size() / 2) + "\n" + l.name.substr(l.name.size() / 2));
            int desc2X = desc1X + description1.getLocalBounds().width + 10;
            description1.setPosition(base.getGlobalBounds().left + 5, base.getGlobalBounds().top + 75);
            description2.setPosition(desc2X, base.getGlobalBounds().top + 75);
        }else{
            int desc2X = desc1X + description1.getLocalBounds().width + 10;
            description1.setPosition(desc1X, base.getGlobalBounds().top + 45);
            description2.setPosition(desc2X, base.getGlobalBounds().top + 45);
        }
        std::string descText1, descText2;
        descText1 += "Rating: " + std::to_string(l.rating).substr(0,4) + "\n";
        descText2 += "Price: $" + std::to_string(l.price).substr(0, std::to_string(l.price).find('.') + 3) + "\n";
        descText1 += l.neighborhood + "\n";
        descText2 += l.borough;
        descText1 += l.roomType;
        description1.setString(descText1);
        description2.setString(descText2);
        int desc2X = base.getGlobalBounds().left + base.getGlobalBounds().width - (description2.getLocalBounds().width + 10);
        description2.setPosition(desc2X, description2.getGlobalBounds().top);
    }
};
