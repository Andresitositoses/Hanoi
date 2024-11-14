//
// Created by andre on 01/09/2021.
//

#ifndef TORRES_DE_HANOI_INTERFACE_H
#define TORRES_DE_HANOI_INTERFACE_H

#include <SFML/Graphics.hpp>
#include "ProgramState.hpp"
#include <map>

enum GameStates {
    MENU,
    GAME,
    END
};

class Interface {
private:
    unsigned int width, height;
    std::map<int, ProgramState*> states;
    int currentState;

public:
    Interface(unsigned int width, unsigned int height);
    ~Interface();
    
    void update(sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
};

#endif //TORRES_DE_HANOI_INTERFACE_H
