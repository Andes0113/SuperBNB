#ifndef TextField_h
#define TextField_h
#include <SFML/Graphics.hpp>

#endif /* TextField_h */

struct TextField{
    bool selected;
    sf::RectangleShape field;
    sf::Text text;
    int maxText;
    TextField(int x, int y, std::string s, sf::Font& font){
        maxText = 5;
        field.setSize(sf::Vector2f(50, 50));
        field.setFillColor(sf::Color::White);
        field.setOutlineColor(sf::Color::Black);
        field.setOutlineThickness(5);
        field.setPosition(x, y);
        text.setString(s);
        text.setFont(font);
        text.setCharacterSize(30);
        text.setFillColor(sf::Color::Black);
        text.setOrigin(sf::Vector2f(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2));
        int textX = field.getGlobalBounds().left + field.getLocalBounds().width / 2;
        int textY = field.getGlobalBounds().top + field.getLocalBounds().height / 2 - text.getLocalBounds().height / 2;
        text.setPosition(textX, textY);
    }
    std::string GetText(){
        return text.getString();
    }
    void SetText(std::string s){
        text.setString(s);
        text.setOrigin(sf::Vector2f(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2));
        int textX = field.getGlobalBounds().left + field.getLocalBounds().width / 2;
        int textY = field.getGlobalBounds().top + field.getLocalBounds().height / 2 - text.getLocalBounds().height / 2;
        text.setPosition(textX, textY);
    }
    void Select(){
        selected = true;
        field.setFillColor(sf::Color(0, 213, 255));
    }
    void Deselect(){
        selected = false;
        field.setFillColor(sf::Color::White);
    }
    void SetPosition(int x, int y){
        field.setPosition(x, y);
        text.setPosition(sf::Vector2f(x + 5, y + 5));
    }
    void setSize(int x, int y){
        field.setSize(sf::Vector2f(x, y));
        int textX = field.getGlobalBounds().left + field.getLocalBounds().width / 2;
        int textY = field.getGlobalBounds().top + field.getLocalBounds().height / 2 - text.getLocalBounds().height / 2;
        text.setPosition(textX, textY);
    }
};

double ParseDouble(TextField field){
    std::string dub = field.GetText();
    if(dub[0] == '.')
        dub = '0' + dub;
    if(dub.find('.') == string::npos)
        dub = dub + ".0";
    std::string postDec = dub.substr(dub.find('.') + 1);
    if(postDec.find('.') != string::npos){
        dub = dub.substr(0, dub.find('.')) + '.' + postDec.substr(0, dub.find('.'));
    }
    return stod(dub);;
}
