#include "include/GameState.hpp"
#include "include/interface.hpp"
#include <SFML/Graphics.hpp>

#define FONTS_PATH "Fonts\\"
#define LEVEL 5  // Número de discos inicial

struct Controls {
    sf::Keyboard::Key first_towel = sf::Keyboard::J;
    sf::Keyboard::Key second_towel = sf::Keyboard::K;
    sf::Keyboard::Key third_towel = sf::Keyboard::L;
} controls;

void manage_movement(bool first, bool second, bool third, Torre* t1, Torre* t2, Torre* t3, Torre* tAux) {
    // ... (mantener el contenido de la función igual)
}

GameState::GameState(unsigned int width, unsigned int height) 
    : width(width), height(height), tab(false), tab_pressed(false) {
    
    // Inicializar torres
    int diskWidth = height/7, diskHeight = height/35;
    torre1 = new Torre(LEVEL, width/4 - diskWidth/2, height - diskHeight*5, diskWidth, diskHeight);
    torre2 = new Torre(0, width/2 - diskWidth/2, height - diskHeight*5, diskWidth, diskHeight);
    torre3 = new Torre(0, width*3/4 - diskWidth/2, height - diskHeight*5, diskWidth, diskHeight);
    torreAux = new Torre(0, width*3/4 - diskWidth/2, 200, 300, diskHeight);

    // Inicializar textos
    sf::Font* font = new sf::Font();
    if(font->loadFromFile((std::string)FONTS_PATH + "stjelogo/Stjldbl1.ttf")) {
        sf::Text* text = new sf::Text();
        text->setFont(*font);
        text->setString("En juego\nPresiona ESPACIO para finalizar");
        text->setCharacterSize(50);
        text->setPosition(width/4.0f, 100.0f);
        texts.push_back({font, text});
    }
}

GameState::~GameState() {
    delete torre1;
    delete torre2;
    delete torre3;
    delete torreAux;
    
    for(auto& [font, text] : texts) {
        delete font;
        delete text;
    }
}

bool GameState::tab_manage() {
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Tab) && !tab_pressed) {
        tab_pressed = true;
        tab = !tab;
    }
    else if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)){
        tab_pressed = false;
    }
    return tab;
}

void GameState::run(sf::RenderWindow& window, int& state) {
    tab_manage();
    
    // Gestión del movimiento de torres
    manage_movement(sf::Keyboard::isKeyPressed(controls.first_towel),
                   sf::Keyboard::isKeyPressed(controls.second_towel),
                   sf::Keyboard::isKeyPressed(controls.third_towel),
                   torre1, torre2, torre3, torreAux);

    // Transición provisional al estado final
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        state = END;
    }
}

void GameState::draw(sf::RenderWindow& window) {
    window.clear();
    
    // Dibujar torres
    torre1->draw(window);
    torre2->draw(window);
    torre3->draw(window);
    torreAux->draw(window);
    
    // Dibujar textos
    for(const auto& [font, text] : texts) {
        window.draw(*text);
    }
} 