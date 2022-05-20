#ifndef NeighborSearchGraphics_h
#define NeighborSearchGraphics_h
#include <SFML/Graphics.hpp>

#endif /* NeighborSearchGraphics_h */

struct NeighborhoodBox {
    std::string neighborhood;
    sf::RectangleShape base;
    sf::Text text;
    NeighborhoodBox(std::string n, int x, int y, sf::Font& font){
        neighborhood = n;
        base.setSize(sf::Vector2f(516, 50));
        base.setFillColor(sf::Color(77,139,240));
        base.setPosition(x, y);
        base.setOutlineColor(sf::Color::Black);
        base.setOutlineThickness(2);
        text.setCharacterSize(30);
        text.setFillColor(sf::Color::White);
        text.setPosition(x + 5, y + 5);
        text.setFont(font);
        text.setString(neighborhood);
    }
    void SetNeighborhood(std::string n){
        neighborhood = n;
        text.setString(n);
    }
};
