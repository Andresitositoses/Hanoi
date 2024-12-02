#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

enum class MovimientoEstrellas {
    HORIZONTAL,  // Izquierda a derecha
    RADIAL      // Del centro hacia fuera
};

struct Estrella {
    sf::CircleShape forma;
    sf::Vector2f velocidad;
};

enum class ColorMode {
    SINGLE,     // Un solo color específico
    RANDOM,     // Colores aleatorios
    WHITE       // Blanco por defecto
};

const size_t MAX_ESTRELLAS = 5000;  // Límite máximo de estrellas simultáneas
const size_t MAX_ESTRELLAS_POR_EXPLOSION = 20;  // Límite por explosión

std::vector<Estrella> generarEstrellas(
    unsigned int width, 
    unsigned int height, 
    int cantidad, 
    ColorMode modo = ColorMode::WHITE,
    sf::Color color = sf::Color::White,
    MovimientoEstrellas movimiento = MovimientoEstrellas::HORIZONTAL
);

void actualizarEstrellas(
    std::vector<Estrella>& estrellas, 
    unsigned int width, 
    unsigned int height, 
    float deltaTime,
    MovimientoEstrellas movimiento = MovimientoEstrellas::HORIZONTAL
);

void generarExplosionEstrellas(
    std::vector<Estrella>& estrellas,
    float x, 
    float y, 
    int cantidad,
    float velocidad,
    sf::Color color
);

void limitarEstrellas(std::vector<Estrella>& estrellas, size_t maximo = MAX_ESTRELLAS);
