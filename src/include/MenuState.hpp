#pragma once
#include "ProgramState.hpp"
#include <vector>

class MenuState : public ProgramState {
private:
    unsigned int width;
    unsigned int height;
    std::vector<std::pair<sf::Font*, sf::Text*>> texts;
    int selectedOption;
    const int NUM_OPTIONS = 5;

public:
    MenuState(unsigned int width, unsigned int height);
    ~MenuState();
    void init(sf::RenderWindow& window);
    void run(sf::RenderWindow& window, int& state);
    void draw(sf::RenderWindow& window);
}; 