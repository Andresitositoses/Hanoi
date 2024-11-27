#pragma once
#include "ProgramState.hpp"
#include "torre.hpp"
#include <vector>
#include <map>

class MenuState : public ProgramState {
private:
    unsigned int width;
    unsigned int height;
    std::vector<std::pair<sf::Font*, sf::Text*>> optionsTexts;
    int selectedOption;
    std::vector<std::pair<std::string, void(MenuState::*)(sf::RenderWindow&, int*)>> menuOptions;

public:
    MenuState(unsigned int width, unsigned int height);
    ~MenuState();
    void init(sf::RenderWindow& window) override;
    void run(sf::RenderWindow& window, int& state) override;
    void draw(sf::RenderWindow& window) override;

private:
    void startGameOption(sf::RenderWindow& window, int* state);
    void setLevelOption(sf::RenderWindow& window, int* level);
    void setAppearanceOption(sf::RenderWindow& window, int* state);
    void setModeOption(sf::RenderWindow& window, int* state);
    void setControlsOption(sf::RenderWindow& window, int* state);
    void exitGameOption(sf::RenderWindow& window, int* state);
}; 