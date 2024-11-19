#pragma once
#include "ProgramState.hpp"
#include <vector>

class EndState : public ProgramState {
private:
    std::vector<std::pair<sf::Font*, sf::Text*>> texts;
    unsigned int width, height;

public:
    EndState(unsigned int width, unsigned int height);
    ~EndState() override;
    void init(sf::RenderWindow& window) override;
    void run(sf::RenderWindow& window, int& state) override;
    void draw(sf::RenderWindow& window) override;
}; 