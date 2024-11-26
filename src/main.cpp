#include <SFML/Graphics.hpp>
#include "interface.hpp"
#include <iostream>
#include "include/MusicalController/notesDetector.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

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
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Torres de Hanoi", sf::Style::Default);
    window.setFramerateLimit(60);

    // Crear la interfaz con la resolución de la pantalla
    Interface interface(SCREEN_WIDTH, SCREEN_HEIGHT);

    NotesDetector notesDetector;
    notesDetector.start();

    // Bucle principal del juego
    while (window.isOpen()) {
        // Procesar eventos
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        std::string note;
        if (notesDetector.getDetectedNote(note)) {
            std::cout << "Nota detectada: " << note << std::endl;
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