#include "include/GameState.hpp"
#include "include/interface.hpp"
#include <SFML/Graphics.hpp>

#define FONTS_PATH "Fonts\\"

#define FLUTE_KEY_PRESS_SENSITIVITY 0.5f

GameState::GameState(unsigned int width, unsigned int height) 
    : width(width), height(height), tab(false), tab_pressed(false), 
      hasRingTaken(false), keyPressed(false), lastTowerSelected(SelectedTower::NO_TOWER),
      keyPressSensibility(FLUTE_KEY_PRESS_SENSITIVITY), diskWidth(height/7), diskHeight(height/35) {
    
    // Inicializar torres   
    torre1 = new Torre(getLevel(), width/4 - diskWidth/2, height - diskHeight*5, diskWidth, diskHeight, getAppearance());
    torre2 = new Torre(0, width/2 - diskWidth/2, height - diskHeight*5, diskWidth, diskHeight, getAppearance());
    torre3 = new Torre(0, width*3/4 - diskWidth/2, height - diskHeight*5, diskWidth, diskHeight, getAppearance());
    torreAux = new Torre(0, width/2 - 150, height/2, 300, diskHeight, getAppearance());

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
    torre1 = new Torre(getLevel(), width/4 - diskWidth/2, height - diskHeight*5, diskWidth, diskHeight, getAppearance());
    torre2 = new Torre(getLevel(), width/2 - diskWidth/2, height - diskHeight*5, diskWidth, diskHeight, getAppearance());
    torre3 = new Torre(getLevel(), width*3/4 - diskWidth/2, height - diskHeight*5, diskWidth, diskHeight, getAppearance());

    // La segunda torre y la tercera siempre están vacías
    torre1->fill();
    torre2->empty();
    torre3->empty();

    // La torre auxiliar siempre tiene nivel 1
    torreAux->empty();
    
    // Reiniciar estado del tab
    tab = false;
    tab_pressed = false;

    //TODO: Inicializar dispositivo de control, ya sea el teclado (que no habría que hacer nada) o la flauta (Inicializar NotesDetector)
    
}

void GameState::run(sf::RenderWindow& window, int& state) {
    tab_manage();
    
    // Gestión del movimiento de torres
    manage_movements(getSelectedTower() == SelectedTower::LEFT_TOWER,
                   getSelectedTower() == SelectedTower::MIDDLE_TOWER,
                   getSelectedTower() == SelectedTower::RIGHT_TOWER,
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
        if (first_selected && lastTowerSelected != SelectedTower::LEFT_TOWER ||
            second_selected && lastTowerSelected != SelectedTower::MIDDLE_TOWER ||
            third_selected && lastTowerSelected != SelectedTower::RIGHT_TOWER) {
            keyPressed = false; // Ha habido un cambio de selección brusco (El jugador es rápido)
        } else if (keyPressClock.getElapsedTime().asSeconds() < FLUTE_KEY_PRESS_SENSITIVITY) {
            keyPressClock.restart(); // Se ha presionado la misma tecla, pero no ha pasado el tiempo mínimo (Se trata de la misma pulsación)
        }
    
        if (!keyPressed) {
            keyPressed = true;
            keyPressClock.restart();  // Reiniciar el reloj cuando se presiona una tecla
            
            if (first_selected) lastTowerSelected = SelectedTower::LEFT_TOWER;
            else if (second_selected) lastTowerSelected = SelectedTower::MIDDLE_TOWER;
            else lastTowerSelected = SelectedTower::RIGHT_TOWER;
            
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
        // Solo considerar la tecla como no presionada si ha pasado al menos 0.3 segundos
        if (keyPressClock.getElapsedTime().asSeconds() >= FLUTE_KEY_PRESS_SENSITIVITY) {
            keyPressed = false;
        }
    }
}