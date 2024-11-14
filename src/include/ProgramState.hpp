#pragma once
#include <SFML/Graphics.hpp>

class ProgramState {
public:
    virtual ~ProgramState() = default;
    virtual void update(sf::RenderWindow& window, int& state) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
}; 