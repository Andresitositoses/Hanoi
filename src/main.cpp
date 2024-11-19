#include <SFML/Graphics.hpp>
#include "include/interface.hpp"
#include <iostream>

int main() {
    // Verificar que podemos cargar las fuentes
    sf::Font font;
    if (!font.loadFromFile("Fonts/stjelogo/Stjldbl1.ttf")) {
        std::cerr << "Error: No se pudo cargar la fuente\n";
        return -1;
    }

    // Obtener la resolución de la pantalla
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

    // Crear la ventana con resolución Full HD
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Torres de Hanoi", sf::Style::Default);
    window.setFramerateLimit(60);

    // Crear la interfaz con las dimensiones de la ventana
    Interface interface(1920, 1080);

    // Bucle principal del juego
    while (window.isOpen()) {
        // Procesar eventos
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Actualizar estado del juego
        interface.update(window);

        // Dibujar
        window.clear(sf::Color::Black); // Eliminar elementos anteriores
        interface.draw(window); // Dibujar nuevos elementos
        window.display(); // Mostrar ventana
    }

    return 0;
}