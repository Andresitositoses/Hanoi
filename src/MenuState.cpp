#include "include/MenuState.hpp"
#include "include/interface.hpp"
#include "include/GameState.hpp"
#define FONTS_PATH "Fonts\\"
#define MAX_LEVEL 20

extern unsigned int level;

MenuState::MenuState(unsigned int width, unsigned int height) 
    : width(width), height(height), selectedOption(0) {
    
    sf::Font* menuFont = new sf::Font();
    sf::Font* levelFont = new sf::Font();
    
    if(menuFont->loadFromFile((std::string)FONTS_PATH + "stjelogo/Stjldbl1.ttf") &&
       levelFont->loadFromFile((std::string)FONTS_PATH + "arial.ttf")) {
        
        std::vector<std::string> menuOptions = {
            "Iniciar partida",
            "Numero de anillas",
            "Modo",
            "Accesibilidad",
            "Salir del juego"
        };

        float startY = height/3.0f;
        float spacing = 80.0f;

        // Crear las opciones del menú
        for(int i = 0; i < menuOptions.size(); i++) {
            sf::Text* text = new sf::Text();
            text->setFont(*menuFont);
            text->setString(menuOptions[i]);
            text->setCharacterSize(60);
            text->setPosition(width/2.0f, startY + i * spacing);
            text->setOrigin(text->getLocalBounds().width/2.0f, text->getLocalBounds().height/2.0f);
            texts.push_back({menuFont, text});
        }

        // Crear el texto del nivel actual en la esquina superior izquierda
        sf::Text* levelText = new sf::Text();
        levelText->setFont(*levelFont);
        levelText->setString("Nivel actual: " + std::to_string(level));
        levelText->setCharacterSize(40);
        levelText->setPosition(50, 50);
        texts.push_back({levelFont, levelText});
    }
}

MenuState::~MenuState() {
    // Limpieza de memoria
    for(auto& [font, text] : texts) {
        delete font;
        delete text;
    }
}

void MenuState::init(sf::RenderWindow& window) {
    // TODO: En esta etapa debería seleccionarse la primera opción por defecto (Aunque realmente ya se ha quedado con esta opción)
}

void MenuState::run(sf::RenderWindow& window, int& state) {
    static bool keyReleased = true;

    if (keyReleased) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            selectedOption = (selectedOption - 1 + NUM_OPTIONS) % NUM_OPTIONS;
            keyReleased = false;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            selectedOption = (selectedOption + 1) % NUM_OPTIONS;
            keyReleased = false;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
            switch(selectedOption) {
                case 0: // Iniciar partida
                    state = GAME;
                    break;
                case 1: // Número de anillas
                    level = (level % MAX_LEVEL) + 1;
                    // Actualizar solo el texto del nivel
                    texts[5].second->setString("Nivel actual: " + std::to_string(level));
                    break;
                case 4: // Salir del juego
                    window.close();
                    break;
                default:
                    break;
            }
            keyReleased = false;
        }
    }
    
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && 
        !sf::Keyboard::isKeyPressed(sf::Keyboard::Down) &&
        !sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
        keyReleased = true;
    }
}

void MenuState::draw(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
    
    // Dibujar las opciones del menú (los primeros 5 textos)
    for(int i = 0; i < 5; i++) {
        const auto& [font, text] = texts[i];
        if (i == selectedOption) {
            text->setFillColor(sf::Color::Yellow);
            text->setCharacterSize(70);
        } else {
            text->setFillColor(sf::Color::White);
            text->setCharacterSize(60);
        }
        window.draw(*text);
    }
    
    // Dibujar el nivel actual (el último texto)
    const auto& [font, text] = texts[5];
    window.draw(*text);
} 