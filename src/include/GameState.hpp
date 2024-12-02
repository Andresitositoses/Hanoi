#pragma once
#include "ProgramState.hpp"
#include "Torre.hpp"
#include <vector>
#include <SFML/System/Clock.hpp>
#include <map>
#include <array>
#include "universe.hpp"

class GameState : public ProgramState {
private:
    enum TextId {
        TIMER,
        MIN_MOVES,
        CURRENT_MOVES,
        LEFT_TOWER,
        MIDDLE_TOWER,
        RIGHT_TOWER
    };

    Torre *torre1, *torre2, *torre3, *torreAux;
    std::map<TextId, std::pair<sf::Font*, sf::Text*>> texts;
    unsigned int width, height;
    unsigned int diskWidth, diskHeight;
    bool tab, tab_pressed;
    bool hasRingTaken;
    bool keyPressed;
    SelectedTower lastTowerSelected;
    sf::Clock keyPressClock;
    float keyPressSensibility;
    sf::RectangleShape towerBase1;
    sf::RectangleShape towerBase2;
    sf::RectangleShape towerBase3;
    sf::Clock gameTimer;
    bool timerStarted;
    int moveCount;

    // Para las estrellas
    std::vector<Estrella> estrellas;
    sf::Clock estrellasReloj;

public:
    GameState(unsigned int width, unsigned int height);
    ~GameState() override;
    void init(sf::RenderWindow& window) override;
    void run(sf::RenderWindow& window, int& state) override;
    void draw(sf::RenderWindow& window) override;
private:
    bool tab_manage();
    void manage_movements(bool first_selected, bool second_selected, bool third_selected, Torre* t1, Torre* t2, Torre* t3, Torre* tAux);
    void initControllersTexts(sf::Font* font);
    int getMinimumMoves() const { return (1 << getLevel()) - 1; } // 2^n - 1, donde n es el número de anillas
    void finishGame(int& state);
    void generarNuevasEstrellas(float x, float y, int cantidad, sf::Color color);
}; 