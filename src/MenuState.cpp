#include "include/MenuState.hpp"
#include "include/interface.hpp"
#include "include/GameState.hpp"
#define FONTS_PATH "Fonts\\"
#define MAX_LEVEL 20

MenuState::MenuState(unsigned int width, unsigned int height) 
    : width(width), height(height), selectedOption(0) {
    
    sf::Font* menuFont = new sf::Font();
    sf::Font* levelFont = new sf::Font();
    
    if(menuFont->loadFromFile((std::string)FONTS_PATH + "stjelogo/Stjldbl1.ttf") &&
       levelFont->loadFromFile((std::string)FONTS_PATH + "arial.ttf")) {
        
        menuOptions = {
            {"Iniciar partida", &MenuState::startGameOption},  
            {"Numero de anillas", &MenuState::setLevelOption},
            {"Apariencia", &MenuState::setAppearanceOption},
            {"Modo", &MenuState::setModeOption},
            {"Accesibilidad", &MenuState::setAccessibilityOption},
            {"Salir del juego", &MenuState::exitGameOption}
        };

        float startY = height/3.0f;
        float spacing = 80.0f;

        // Crear las opciones del menú
        int optionCount = 0;
        for(const auto& [optionText, _] : menuOptions) {
            sf::Text* text = new sf::Text();
            text->setFont(*menuFont);
            text->setString(optionText);
            text->setCharacterSize(60);
            text->setPosition(width/2.0f, startY + optionCount * spacing);
            text->setOrigin(text->getLocalBounds().width/2.0f, text->getLocalBounds().height/2.0f);
            optionsTexts.push_back({menuFont, text});
            optionCount++;
        }

        // Crear el texto del nivel actual
        sf::Text* levelText = new sf::Text();
        levelText->setFont(*levelFont);
        levelText->setString("Nivel actual: " + std::to_string(level));
        levelText->setCharacterSize(40);
        levelText->setPosition(50, 50);
        optionsTexts.push_back({levelFont, levelText});

        // Crear el texto de la apariencia actual
        sf::Text* appearanceText = new sf::Text();
        appearanceText->setFont(*levelFont);
        std::string appearanceStr = "";
        switch (currentAppearance) {    
            case Appearance::COLORS:
                appearanceStr = "Colores aleatorios";
                break;
            case Appearance::WOODEN:
                appearanceStr = "Madera";
                break;
            case Appearance::BRICK:
                appearanceStr = "Ladrillos";
                break;
            case Appearance::ICE:
                appearanceStr = "Hielo";
                break;
            case Appearance::ROCKS:
                appearanceStr = "Rocas";
                break;
            case Appearance::UNIVERSE:
                appearanceStr = "Universo";
                break;
            default:
                appearanceStr = "Colores aleatorios";
                break;
        }
        appearanceText->setString("Apariencia: " + appearanceStr);
        appearanceText->setCharacterSize(40);
        appearanceText->setPosition(50, 100); // Debajo del texto del nivel
        optionsTexts.push_back({levelFont, appearanceText});
    }
}

MenuState::~MenuState() {
    // Limpieza de memoria
    for(auto& [font, text] : optionsTexts) {
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
            selectedOption = (selectedOption - 1 + menuOptions.size()) % menuOptions.size();
            keyReleased = false;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            selectedOption = (selectedOption + 1) % menuOptions.size();
            keyReleased = false;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
            // Ejecuta la función asociada a la opción seleccionada
            auto& [optionText, optionFunc] = menuOptions[selectedOption];
            (this->*optionFunc)(window, &state);
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
    
    // Dibujar las opciones del menú
    for(int i = 0; i < menuOptions.size(); i++) {
        const auto& [font, text] = optionsTexts[i];
        if (i == selectedOption) {
            text->setFillColor(sf::Color::Yellow);
            text->setCharacterSize(70);
        } else {
            text->setFillColor(sf::Color::White);
            text->setCharacterSize(60);
        }
        window.draw(*text);
    }
    
    // Dibujar los textos informativos (nivel y apariencia)
    const auto& [levelFont, levelText] = optionsTexts[menuOptions.size()];
    const auto& [appearanceFont, appearanceText] = optionsTexts[menuOptions.size() + 1];
    window.draw(*levelText);
    window.draw(*appearanceText);
} 

// Menu options

void MenuState::startGameOption(sf::RenderWindow& window, int* state) {
    *state = GAME;
}

void MenuState::setLevelOption(sf::RenderWindow& window, int* state) {
    level = (level >= MAX_LEVEL) ? 1 : level + 1;
    
    // Actualizar el texto informativo del nivel
    optionsTexts[menuOptions.size()].second->setString(
        "Nivel actual: " + std::to_string(level)
    );
}

void MenuState::setAppearanceOption(sf::RenderWindow& window, int* state) {
    currentAppearance = (currentAppearance == Appearance::UNIVERSE) ? Appearance::COLORS : currentAppearance + 1; // Si es el último, vuelve a empezar
    
    // Actualizar el texto informativo de la apariencia
    std::string appearanceStr = "";
    switch (currentAppearance) {
        case Appearance::WOODEN:
            appearanceStr = "Madera";
            break;
        case Appearance::COLORS:
            appearanceStr = "Colores aleatorios";
            break;
        case Appearance::BRICK:
            appearanceStr = "Ladrillos";
            break;
        case Appearance::ICE:
            appearanceStr = "Hielo";
            break;
        case Appearance::ROCKS:
            appearanceStr = "Rocas";
            break;
        case Appearance::UNIVERSE:
            appearanceStr = "Universo";
            break;
        default:
            appearanceStr = "Colores aleatorios";
            break;
    }
    optionsTexts[menuOptions.size() + 1].second->setString("Apariencia: " + appearanceStr);

    appearance = currentAppearance;
}

void MenuState::setModeOption(sf::RenderWindow& window, int* state) {
    //TODO: Implementar
}

void MenuState::setAccessibilityOption(sf::RenderWindow& window, int* state) {
    //TODO: Implementar
}

void MenuState::exitGameOption(sf::RenderWindow& window, int* state) {
    window.close();
}