#include "include/universe.hpp"
#include <random>

std::vector<Estrella> generarEstrellas(unsigned int width, unsigned int height, int cantidad, 
                                     ColorMode modo, sf::Color color, MovimientoEstrellas movimiento) {
    // Limitar la cantidad inicial de estrellas
    cantidad = std::min(cantidad, static_cast<int>(MAX_ESTRELLAS));
    
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
    std::uniform_real_distribution<float> distribAngulo(0.0f, 2.0f * 3.14159f);
    std::uniform_int_distribution<> distribColor(0, 255);

    std::vector<Estrella> estrellas;
    estrellas.reserve(cantidad);

    for(int i = 0; i < cantidad; i++) {
        Estrella estrella;
        
        if (movimiento == MovimientoEstrellas::RADIAL) {
            // Para movimiento radial, empezar cerca del centro
            float angulo = distribAngulo(gen);
            float distancia = distribVel(gen);
            estrella.forma.setPosition(
                width/2.0f + std::cos(angulo) * distancia,
                height/2.0f + std::sin(angulo) * distancia
            );
            // La velocidad apunta en la dirección del ángulo
            estrella.velocidad = sf::Vector2f(
                std::cos(angulo) * distribVel(gen),
                std::sin(angulo) * distribVel(gen)
            );
        } else {
            estrella.forma.setPosition(distribX(gen), distribY(gen));
            estrella.velocidad = sf::Vector2f(distribVel(gen), 0.0f);
        }
        
        estrella.forma.setRadius(distribRadio(gen));
        
        // Asignar color según el modo
        switch(modo) {
            case ColorMode::SINGLE:
                estrella.forma.setFillColor(color);
                break;
            case ColorMode::RANDOM:
                estrella.forma.setFillColor(sf::Color(
                    distribColor(gen),
                    distribColor(gen),
                    distribColor(gen)
                ));
                break;
            case ColorMode::WHITE:
            default:
                estrella.forma.setFillColor(sf::Color::White);
                break;
        }
        
        estrellas.push_back(estrella);
    }

    return estrellas;
}

void actualizarEstrellas(std::vector<Estrella>& estrellas, unsigned int width, unsigned int height, 
                        float deltaTime, MovimientoEstrellas movimiento) {
    for(auto& estrella : estrellas) {
        estrella.forma.move(estrella.velocidad * deltaTime);
        
        if (movimiento == MovimientoEstrellas::RADIAL) {
            // Para movimiento radial, reiniciar al centro cuando se sale de la pantalla
            auto pos = estrella.forma.getPosition();
            if (pos.x < 0 || pos.x > width || pos.y < 0 || pos.y > height) {
                estrella.forma.setPosition(width/2.0f, height/2.0f);
            }
        } else {
            // Para movimiento horizontal, reiniciar a la izquierda
            if(estrella.forma.getPosition().x > width) {
                estrella.forma.setPosition(-estrella.forma.getRadius(), estrella.forma.getPosition().y);
            }
        }
    }
}

void generarExplosionEstrellas(std::vector<Estrella>& estrellas, float x, float y, int cantidad, float velocidad, sf::Color color) {
    // Limitar la cantidad de estrellas por explosión
    cantidad = std::min(cantidad, static_cast<int>(MAX_ESTRELLAS_POR_EXPLOSION));
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distribAngulo(0.0f, 2.0f * 3.14159f);
    
    for(int i = 0; i < cantidad; i++) {
        float angulo = distribAngulo(gen);
        Estrella estrella;
        estrella.forma.setPosition(x, y);
        estrella.velocidad = sf::Vector2f(
            std::cos(angulo) * velocidad,
            std::sin(angulo) * velocidad
        );
        estrella.forma.setRadius(1.0f);
        estrella.forma.setFillColor(color);
        estrellas.push_back(estrella);
    }
    
    // Mantener el límite total de estrellas
    limitarEstrellas(estrellas);
}

void limitarEstrellas(std::vector<Estrella>& estrellas, size_t maximo) {
    if (estrellas.size() > maximo) {
        // Eliminar las estrellas más antiguas
        estrellas.erase(estrellas.begin(), estrellas.begin() + (estrellas.size() - maximo));
    }
} 