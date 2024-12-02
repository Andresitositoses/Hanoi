#include "include/universe.hpp"
#include <random>

std::vector<Estrella> generarEstrellas(unsigned int width, unsigned int height, int cantidad) {
    const float radioMinimo = 0.5f;
    const float radioMaximo = 2.0f;
    const float velocidadMinima = 20.0f;
    const float velocidadMaxima = 50.0f;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distribX(0.0f, static_cast<float>(width));
    std::uniform_real_distribution<float> distribY(0.0f, static_cast<float>(height));
    std::uniform_real_distribution<float> distribRadio(radioMinimo, radioMaximo);
    std::uniform_real_distribution<float> distribVel(velocidadMinima, velocidadMaxima);

    std::vector<Estrella> estrellas;
    estrellas.reserve(cantidad);

    for(int i = 0; i < cantidad; i++) {
        Estrella estrella;
        estrella.forma.setRadius(distribRadio(gen));
        estrella.forma.setPosition(distribX(gen), distribY(gen));
        estrella.forma.setFillColor(sf::Color::White);
        
        // Velocidad aleatoria hacia la derecha
        estrella.velocidad = sf::Vector2f(distribVel(gen), 0.0f);
        
        estrellas.push_back(estrella);
    }

    return estrellas;
}

void actualizarEstrellas(std::vector<Estrella>& estrellas, unsigned int width, unsigned int height, float deltaTime) {
    for(auto& estrella : estrellas) {
        // Mover estrella
        estrella.forma.move(estrella.velocidad * deltaTime);
        
        // Si sale por la derecha, volver a la izquierda
        if(estrella.forma.getPosition().x > width) {
            estrella.forma.setPosition(-estrella.forma.getRadius(), estrella.forma.getPosition().y);
        }
    }
} 