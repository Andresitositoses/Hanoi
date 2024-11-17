#pragma once
#include "ProgramState.hpp"
#include <vector>

class MenuState : public ProgramState {
private:
    std::vector<std::pair<sf::Font*, sf::Text*>> texts;
    unsigned int width, height;

public:
    MenuState(unsigned int width, unsigned int height);
    ~MenuState();
    void run(sf::RenderWindow& window, int& state) override;
    void draw(sf::RenderWindow& window) override;
}; 