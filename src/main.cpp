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

    // Crear la ventana en modo pantalla completa
    sf::RenderWindow window(desktop, "Torres de Hanoi", sf::Style::Fullscreen);
    window.setFramerateLimit(60);

    // Crear la interfaz con las dimensiones de la pantalla
    Interface interface(desktop.width, desktop.height);

    // Bucle principal del juego
    while (window.isOpen()) {
        // Procesar eventos
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Actualizar
        interface.update(window);

        // Dibujar
        window.clear(sf::Color::Black);
        interface.draw(window);
        window.display();
    }

    return 0;
}