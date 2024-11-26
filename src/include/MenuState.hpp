#pragma once
#include "ProgramState.hpp"
#include "torre.hpp"
#include <vector>
#include <map>

extern unsigned int level;
extern unsigned int appearance;

class MenuState : public ProgramState {
private:
    unsigned int width;
    unsigned int height;
    std::vector<std::pair<sf::Font*, sf::Text*>> optionsTexts;
    int selectedOption;
    std::vector<std::pair<std::string, void(MenuState::*)(sf::RenderWindow&, int*)>> menuOptions;
    unsigned int currentAppearance = Appearance::COLORS;

public:
    MenuState(unsigned int width, unsigned int height);
    ~MenuState();
    void init(sf::RenderWindow& window);
    void run(sf::RenderWindow& window, int& state);
    void draw(sf::RenderWindow& window);

private:
    void startGameOption(sf::RenderWindow& window, int* state);
    void setLevelOption(sf::RenderWindow& window, int* level);
    void setAppearanceOption(sf::RenderWindow& window, int* state);
    void setModeOption(sf::RenderWindow& window, int* state);
    void setAccessibilityOption(sf::RenderWindow& window, int* state);
    void exitGameOption(sf::RenderWindow& window, int* state);
}; 