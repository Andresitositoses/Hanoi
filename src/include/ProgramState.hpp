#pragma once
#include <SFML/Graphics.hpp>

class ProgramState {
public:
    virtual ~ProgramState() = default;
    virtual void init(sf::RenderWindow& window) = 0;
    virtual void run(sf::RenderWindow& window, int& state) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
}; 