#include "include/EndState.hpp"
#include "include/interface.hpp"

#define FONTS_PATH "Fonts\\"

EndState::EndState(unsigned int width, unsigned int height) 
    : width(width), height(height) {
    
    // Crear y almacenar la fuente de manera persistente
    sf::Font* font = new sf::Font();
    if(font->loadFromFile((std::string)FONTS_PATH + "stjelogo/Stjldbl1.ttf")) {
        sf::Text* text = new sf::Text();
        text->setFont(*font);
        text->setString("Fin de juego\nPresiona ESC para volver al menú");
        text->setCharacterSize(100);
        text->setPosition(width/2.0f, height/2.0f);
        text->setOrigin(text->getLocalBounds().width/2.0f, text->getLocalBounds().height/2.0f);
        
        texts.push_back({font, text});
    }
}

EndState::~EndState() {
    // Limpieza de memoria
    for(auto& [font, text] : texts) {
        delete font;
        delete text;
    }
}

void EndState::run(sf::RenderWindow& window, int& state) {
    static bool escReleased = false;

    // Esperar a que se suelte la tecla ESC antes de permitir una nueva transición
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        escReleased = true;
    }
    
    // Solo transicionar si ESC fue soltado y presionado de nuevo
    if (escReleased && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        state = MENU;  // Volver al menú principal
        escReleased = false;  // Resetear el flag
    }
}

void EndState::draw(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
    for(const auto& [font, text] : texts) {
        window.draw(*text);
    }
} 