#pragma once
#include "ProgramState.hpp"
#include "Torre.hpp"
#include <vector>

class GameState : public ProgramState {
private:
    Torre *torre1, *torre2, *torre3, *torreAux;
    std::vector<std::pair<sf::Font*, sf::Text*>> texts;
    unsigned int width, height;
    unsigned int diskWidth, diskHeight;
    bool tab, tab_pressed;
    bool hasRingTaken;
    bool keyPressed;
    sf::Keyboard::Key lastKeyPressed;

public:
    GameState(unsigned int width, unsigned int height);
    ~GameState() override;
    void init(sf::RenderWindow& window) override;
    void run(sf::RenderWindow& window, int& state) override;
    void draw(sf::RenderWindow& window) override;
private:
    bool tab_manage();
    void manage_movements(bool first_selected, bool second_selected, bool third_selected, Torre* t1, Torre* t2, Torre* t3, Torre* tAux);
}; 