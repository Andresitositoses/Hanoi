#pragma once
#include "ProgramState.hpp"
#include "Torre.hpp"
#include <vector>

class GameState : public ProgramState {
private:
    Torre *torre1, *torre2, *torre3, *torreAux;
    std::vector<std::pair<sf::Font*, sf::Text*>> texts;
    unsigned int width, height;
    bool tab, tab_pressed;

public:
    GameState(unsigned int width, unsigned int height);
    ~GameState() override;
    void update(sf::RenderWindow& window, int& state) override;
    void draw(sf::RenderWindow& window) override;
    bool tab_manage();
}; 