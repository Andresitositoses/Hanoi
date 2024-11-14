#include "include/MenuState.hpp"
#include "include/interface.hpp"

#define FONTS_PATH "Fonts\\"

MenuState::MenuState(unsigned int width, unsigned int height) 
    : width(width), height(height) {
    
    // Crear y almacenar la fuente de manera persistente
    sf::Font* font = new sf::Font();
    if(font->loadFromFile((std::string)FONTS_PATH + "stjelogo/Stjldbl1.ttf")) {
        sf::Text* text = new sf::Text();
        text->setFont(*font);
        text->setString(L"Menú Principal\nPresiona ENTER para jugar");
        text->setCharacterSize(100);
        text->setPosition(width/2.0f, height/2.0f);
        text->setOrigin(text->getLocalBounds().width/2.0f, text->getLocalBounds().height/2.0f);
        
        texts.push_back({font, text});
    }
}

MenuState::~MenuState() {
    // Limpieza de memoria
    for(auto& [font, text] : texts) {
        delete font;
        delete text;
    }
}

void MenuState::update(sf::RenderWindow& window, int& state) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
        state = GAME;
    }
}

void MenuState::draw(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
    for(const auto& [font, text] : texts) {
        window.draw(*text);
    }
} 