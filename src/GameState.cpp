#include "include/GameState.hpp"
#include "include/interface.hpp"
#include <SFML/Graphics.hpp>

#define FONTS_PATH "Fonts\\"
#define DEFAULT_LEVEL 3  // Número de discos inicial

unsigned int level = DEFAULT_LEVEL; // No me gusta que esto sea necesario
unsigned int appearance = 0; // 0: color aleatorio, 1: textura de madera

struct Controls {
    sf::Keyboard::Key first_towel = sf::Keyboard::J;
    sf::Keyboard::Key second_towel = sf::Keyboard::K;
    sf::Keyboard::Key third_towel = sf::Keyboard::L;
} controls;

GameState::GameState(unsigned int width, unsigned int height) 
    : width(width), height(height), tab(false), tab_pressed(false), 
      hasRingTaken(false), keyPressed(false), lastKeyPressed(sf::Keyboard::Unknown),
      diskWidth(height/7), diskHeight(height/35) {
    
    // Inicializar torres   
    torre1 = new Torre(level, width/4 - diskWidth/2, height - diskHeight*5, diskWidth, diskHeight, appearance);
    torre2 = new Torre(0, width/2 - diskWidth/2, height - diskHeight*5, diskWidth, diskHeight, appearance);
    torre3 = new Torre(0, width*3/4 - diskWidth/2, height - diskHeight*5, diskWidth, diskHeight, appearance);
    torreAux = new Torre(0, width/2 - 150, height/2, 300, diskHeight, appearance);

    // Inicializar textos
    sf::Font* font = new sf::Font();
    if(font->loadFromFile((std::string)FONTS_PATH + "stjelogo/Stjldbl1.ttf")) {
        sf::Text* text = new sf::Text();
        text->setFont(*font);
        text->setString("En juego\nPresiona ESC para salir al menú principal");
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

void GameState::init(sf::RenderWindow& window) {
    // Reiniciar torres en función del nivel
    delete torre1;
    delete torre2;
    delete torre3;
    torre1 = new Torre(level, width/4 - diskWidth/2, height - diskHeight*5, diskWidth, diskHeight, appearance);
    torre2 = new Torre(level, width/2 - diskWidth/2, height - diskHeight*5, diskWidth, diskHeight, appearance);
    torre3 = new Torre(level, width*3/4 - diskWidth/2, height - diskHeight*5, diskWidth, diskHeight, appearance);

    // La segunda torre y la tercera siempre están vacías
    torre1->fill();
    torre2->empty();
    torre3->empty();

    // La torre auxiliar siempre tiene nivel 1
    torreAux->empty();
    
    // Reiniciar estado del tab
    tab = false;
    tab_pressed = false;
}

void GameState::run(sf::RenderWindow& window, int& state) {
    tab_manage();
    
    // Gestión del movimiento de torres
    manage_movements(sf::Keyboard::isKeyPressed(controls.first_towel),
                   sf::Keyboard::isKeyPressed(controls.second_towel),
                   sf::Keyboard::isKeyPressed(controls.third_towel),
                   torre1, torre2, torre3, torreAux);
                   
    // Determinar si se ha ganado
    if (torre3->isComplete()) {
        state = END;
    }

    // Transición provisional al estado final
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        state = MENU;
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

void GameState::manage_movements(bool first_selected, bool second_selected, bool third_selected, Torre* t1, Torre* t2, Torre* t3, Torre* tAux) {
    //TODO: Estaría guapo que se visualizase el movimiento de las anillas, en lugar de teleportarlas
    
    // Control de teclas presionadas
    if (first_selected || second_selected || third_selected) {
        if (!keyPressed) {
            keyPressed = true;
            if (first_selected) lastKeyPressed = controls.first_towel;
            else if (second_selected) lastKeyPressed = controls.second_towel;
            else lastKeyPressed = controls.third_towel;
            
            // Lógica de movimiento
            if (!hasRingTaken) {
                if (first_selected && !t1->isEmpty() && tAux->isEmpty()) {
                    Anilla* disk = t1->popDisk();
                    tAux->addDisk(disk);
                    hasRingTaken = true;
                }
                else if (second_selected && !t2->isEmpty() && tAux->isEmpty()) {
                    Anilla* disk = t2->popDisk();
                    tAux->addDisk(disk);
                    hasRingTaken = true;
                }
                else if (third_selected && !t3->isEmpty() && tAux->isEmpty()) {
                    Anilla* disk = t3->popDisk();
                    tAux->addDisk(disk);
                    hasRingTaken = true;
                }
            } else {
                if (first_selected && t1->isPlaceable(tAux->top())) {
                    Anilla* disk = tAux->popDisk();
                    t1->addDisk(disk);
                    hasRingTaken = false;
                }
                else if (second_selected && t2->isPlaceable(tAux->top())) {
                    Anilla* disk = tAux->popDisk();
                    t2->addDisk(disk);
                    hasRingTaken = false;
                }
                else if (third_selected && t3->isPlaceable(tAux->top())) {
                    Anilla* disk = tAux->popDisk();
                    t3->addDisk(disk);
                    hasRingTaken = false;
                }
            }
        }
    } else {
        keyPressed = false;
        lastKeyPressed = sf::Keyboard::Unknown;
    }
}