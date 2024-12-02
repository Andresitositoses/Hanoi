#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

struct Estrella {
    sf::CircleShape forma;
    sf::Vector2f velocidad;
};

std::vector<Estrella> generarEstrellas(unsigned int width, unsigned int height, int cantidad);
void actualizarEstrellas(std::vector<Estrella>& estrellas, unsigned int width, unsigned int height, float deltaTime);
