#include "include/GameState.hpp"
#include "include/interface.hpp"
#include <SFML/Graphics.hpp>
#include <locale>

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

    // Crear las líneas base de las torres
    float baseHeight = 3.0f; // Altura de la línea base
    float baseWidth = diskWidth * 1.2f; // Un poco ms ancha que los discos

    towerBase1.setSize(sf::Vector2f(baseWidth, baseHeight));
    towerBase2.setSize(sf::Vector2f(baseWidth, baseHeight));
    towerBase3.setSize(sf::Vector2f(baseWidth, baseHeight));

    towerBase1.setPosition(width/4 - baseWidth/2, height - diskHeight*4);
    towerBase2.setPosition(width/2 - baseWidth/2, height - diskHeight*4);
    towerBase3.setPosition(width*3/4 - baseWidth/2, height - diskHeight*4);

    towerBase1.setFillColor(sf::Color::White);
    towerBase2.setFillColor(sf::Color::White);
    towerBase3.setFillColor(sf::Color::White);

    // Inicializar textos
    sf::Font* font = new sf::Font();
    initControllersTexts(font);
}

GameState::~GameState() {
    delete torre1;
    delete torre2;
    delete torre3;
    delete torreAux;
    
    for(auto& [id, pair] : texts) {
        delete pair.first;
        delete pair.second;
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
    
    // Actualizar los textos de los controles
    auto controlNames = getTriggerInputNames();
    texts[TextId::LEFT_TOWER].second->setString(controlNames[0]);
    texts[TextId::MIDDLE_TOWER].second->setString(controlNames[1]);
    texts[TextId::RIGHT_TOWER].second->setString(controlNames[2]);

    // Recentrar los textos
    std::array<TextId, 3> towerTexts = {LEFT_TOWER, MIDDLE_TOWER, RIGHT_TOWER};
    for (const auto& textId : towerTexts) {
        sf::FloatRect bounds = texts[textId].second->getLocalBounds();
        float xPos = (textId == LEFT_TOWER) ? width/4.0f : 
                    (textId == MIDDLE_TOWER) ? width/2.0f : width*3/4.0f;
        texts[textId].second->setPosition(
            xPos - bounds.width/2,
            height - diskHeight*3
        );
    }

    // Reiniciar temporizador y contadores
    timerStarted = false;
    gameTimer.restart();
    texts[TextId::TIMER].second->setString("0:00");
    moveCount = 0;
    texts[TextId::MIN_MOVES].second->setString(sf::String(L"Movimientos mínimos: " + std::to_string(getMinimumMoves())));
    texts[TextId::CURRENT_MOVES].second->setString(sf::String(L"Movimientos realizados: " + std::to_string(moveCount)));
    texts[TextId::CURRENT_MOVES].second->setFillColor(sf::Color::Green);
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
        finishGame(state);
    }

    // Transición provisional al estado final
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        state = MENU;
    }
}

void GameState::draw(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
    
    // Actualizar y dibujar las estrellas
    float deltaTime = estrellasReloj.restart().asSeconds();
    actualizarEstrellas(estrellas, width, height, deltaTime, MovimientoEstrellas::RADIAL);
    for(const auto& estrella : estrellas) {
        window.draw(estrella.forma);
    }
    
    // Actualizar texto del temporizador si está activo
    if (timerStarted) {
        sf::Time elapsed = gameTimer.getElapsedTime();
        float seconds = elapsed.asSeconds();
        
        int days = static_cast<int>(seconds) / (24 * 3600);
        seconds = std::fmod(seconds, 24 * 3600);
        int hours = static_cast<int>(seconds) / 3600;
        seconds = std::fmod(seconds, 3600);
        int minutes = static_cast<int>(seconds) / 60;
        int remainingSeconds = static_cast<int>(std::fmod(seconds, 60));

        // Construir el string del tiempo
        std::string timeStr;
        if (days > 0) {
            timeStr += std::to_string(days) + "d ";
        }
        if (hours > 0 || days > 0) {
            timeStr += std::to_string(hours) + "h ";
        }
        timeStr += std::to_string(minutes) + ":" + 
                   (remainingSeconds < 10 ? "0" : "") + std::to_string(remainingSeconds);
        
        texts[TextId::TIMER].second->setString(timeStr);
        texts[TextId::TIMER].second->setPosition(
            width/2.0f - texts[TextId::TIMER].second->getLocalBounds().width/2,
            50.0f
        );
    }

    // Dibujar torres
    torre1->draw(window);
    torre2->draw(window);
    torre3->draw(window);
    torreAux->draw(window);

    // Dibujar líneas base de las torres
    window.draw(towerBase1);
    window.draw(towerBase2);
    window.draw(towerBase3);
    
    // Dibujar textos
    for(const auto& [id, pair] : texts) {
        window.draw(*pair.second);
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
                // Iniciar temporizador con la primera anilla que se toma
                if (!timerStarted && (
                    (first_selected && !t1->isEmpty()) ||
                    (second_selected && !t2->isEmpty()) ||
                    (third_selected && !t3->isEmpty())
                )) {
                    timerStarted = true;
                    gameTimer.restart();
                }

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
                    moveCount++;
                    texts[TextId::CURRENT_MOVES].second->setString(sf::String(L"Movimientos realizados: " + std::to_string(moveCount)));
                    texts[TextId::CURRENT_MOVES].second->setFillColor(moveCount > getMinimumMoves() ? sf::Color::Red : sf::Color::Green);
                    // Generar estrellas en la posición donde se coloca el disco
                    generarNuevasEstrellas(width/4.0f, t1->getTopPosition().y, 10, disk->getFillColor());
                }
                else if (second_selected && t2->isPlaceable(tAux->top())) {
                    Anilla* disk = tAux->popDisk();
                    t2->addDisk(disk);
                    hasRingTaken = false;
                    moveCount++;
                    texts[TextId::CURRENT_MOVES].second->setString(sf::String(L"Movimientos realizados: " + std::to_string(moveCount)));
                    texts[TextId::CURRENT_MOVES].second->setFillColor(moveCount > getMinimumMoves() ? sf::Color::Red : sf::Color::Green);
                    // Generar estrellas en la posición donde se coloca el disco
                    generarNuevasEstrellas(width/2.0f, t2->getTopPosition().y, 10, disk->getFillColor());
                }
                else if (third_selected && t3->isPlaceable(tAux->top())) {
                    Anilla* disk = tAux->popDisk();
                    t3->addDisk(disk);
                    hasRingTaken = false;
                    moveCount++;
                    texts[TextId::CURRENT_MOVES].second->setString(sf::String(L"Movimientos realizados: " + std::to_string(moveCount)));
                    texts[TextId::CURRENT_MOVES].second->setFillColor(moveCount > getMinimumMoves() ? sf::Color::Red : sf::Color::Green);
                    // Generar estrellas en la posición donde se coloca el disco
                    generarNuevasEstrellas(width*3/4.0f, t3->getTopPosition().y, 10, disk->getFillColor());
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

void GameState::initControllersTexts(sf::Font* font) {
    // Crear una fuente normal para el temporizador y los contadores
    sf::Font* normalFont = new sf::Font();
    if(normalFont->loadFromFile((std::string)FONTS_PATH + "arial.ttf")) {
        // Texto del temporizador
        sf::Text* timerText = new sf::Text();
        timerText->setFont(*normalFont);
        timerText->setString("0:00");
        timerText->setCharacterSize(50);
        timerText->setPosition(width/2.0f - timerText->getLocalBounds().width/2, 50.0f);
        texts[TextId::TIMER] = {normalFont, timerText};

        // Texto de movimientos mínimos (superior)
        sf::Text* minMovesText = new sf::Text();
        minMovesText->setFont(*normalFont);
        sf::String minText = L"Movimientos mínimos: " + std::to_string(getMinimumMoves());
        minMovesText->setString(minText);
        minMovesText->setCharacterSize(30);
        minMovesText->setPosition(10.0f, 10.0f);
        texts[TextId::MIN_MOVES] = {normalFont, minMovesText};

        // Texto de movimientos actuales
        sf::Text* currentMovesText = new sf::Text();
        currentMovesText->setFont(*normalFont);
        currentMovesText->setString(L"Movimientos realizados: 0");
        currentMovesText->setCharacterSize(30);
        currentMovesText->setPosition(10.0f, 45.0f);
        currentMovesText->setFillColor(sf::Color::Green);
        texts[TextId::CURRENT_MOVES] = {normalFont, currentMovesText};
    }

    // Resto de textos con la fuente decorativa
    if(font->loadFromFile((std::string)FONTS_PATH + "stjelogo/Stjldbl1.ttf")) {
        // Torre izquierda
        sf::Text* leftControl = new sf::Text();
        leftControl->setFont(*font);
        leftControl->setString(getTriggerInputNames()[0]);
        leftControl->setCharacterSize(30);
        sf::FloatRect leftBounds = leftControl->getLocalBounds();
        leftControl->setPosition(
            width/4.0f - leftBounds.width/2,
            height - diskHeight*3
        );
        texts[TextId::LEFT_TOWER] = {font, leftControl};

        // Torre central
        sf::Text* middleControl = new sf::Text();
        middleControl->setFont(*font);
        middleControl->setString(getTriggerInputNames()[1]);
        middleControl->setCharacterSize(30);
        sf::FloatRect middleBounds = middleControl->getLocalBounds();
        middleControl->setPosition(
            width/2.0f - middleBounds.width/2,
            height - diskHeight*3
        );
        texts[TextId::MIDDLE_TOWER] = {font, middleControl};

        // Torre derecha
        sf::Text* rightControl = new sf::Text();
        rightControl->setFont(*font);
        rightControl->setString(getTriggerInputNames()[2]);
        rightControl->setCharacterSize(30);
        sf::FloatRect rightBounds = rightControl->getLocalBounds();
        rightControl->setPosition(
            width*3/4.0f - rightBounds.width/2,
            height - diskHeight*3
        );
        texts[TextId::RIGHT_TOWER] = {font, rightControl};
    }
}

void GameState::finishGame(int& state) {
    // Limpiar estrellas previas
    estrellas.clear();

    setLastGameStats(LastGameStats{moveCount, std::to_string(gameTimer.getElapsedTime().asSeconds())});
    state = END;
}

void GameState::generarNuevasEstrellas(float x, float y, int cantidad, sf::Color color) {
    generarExplosionEstrellas(estrellas, x, y, cantidad, 100.0f, color);
}
