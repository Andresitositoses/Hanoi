#pragma once
#include "ProgramState.hpp"
#include "universe.hpp"
#include <map>
#include <SFML/Graphics.hpp>
#include <vector>

enum class TextId {
    TITLE,
    STATS
};

class EndState : public ProgramState {
private:
    std::map<TextId, std::pair<sf::Font*, sf::Text*>> texts;
    unsigned int width, height;
    sf::Font* font;
    
    // Para las estrellas
    std::vector<Estrella> estrellas;
    sf::Clock estrellasReloj;

public:
    EndState(unsigned int width, unsigned int height);
    ~EndState() override;
    void init(sf::RenderWindow& window) override;
    void run(sf::RenderWindow& window, int& state) override;
    void draw(sf::RenderWindow& window) override;
private:
    void updateAnimation();
}; 