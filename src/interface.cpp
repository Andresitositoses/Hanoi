//
// Created by andre on 01/09/2021.
//
#include "include/interface.hpp"
#include "include/MenuState.hpp"
#include "include/GameState.hpp"
#include "include/EndState.hpp"
#include <iostream>

Interface::Interface(unsigned int width, unsigned int height) {
    this->width = width;
    this->height = height;
    
    states[MENU] = new MenuState(width, height);
    states[GAME] = new GameState(width, height);
    states[END] = new EndState(width, height);
    
    currentState = MENU;
}

Interface::~Interface() {
    for (auto& state : states) {
        delete state.second;
    }
}

void Interface::update(sf::RenderWindow& window) {
    int nextState = currentState;
    states[currentState]->run(window, nextState);
    
    // Verificar que el nuevo estado es válido antes de cambiar
    if (nextState != currentState) {
        std::cout << "Cambiando de estado " << currentState << " a " << nextState << std::endl;
        currentState = nextState;
        //TODO: Inicializar nuevo estado y dibujar elementos
    }
}

void Interface::draw(sf::RenderWindow& window) {
    if (states.count(currentState) > 0) {
        states[currentState]->draw(window);
    }
}