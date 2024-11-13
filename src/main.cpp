#include <iostream>
#include "include/interface.hpp"
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

#define WINDOW_TITTLE "Torres de Hanoi"
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define FPS 60

enum Estado{MENU, INGAME, ENDGAME};

int main() {

    // Interfaz
    int state = MENU;
    unsigned int width = VideoMode::getDesktopMode().width;
    unsigned int height = VideoMode::getDesktopMode().height;
    Interface interfaz = Interface(width, height);

    // Ventana
    bool isFullscreen = true;
    bool returnPressed = false;
    RenderWindow window(VideoMode(width, height), WINDOW_TITTLE, Style::Fullscreen);
    window.setFramerateLimit(FPS);

    while(window.isOpen()){

        // Manejo de eventos
        Event event{};
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == Event::Closed || event.key.code == Keyboard::Delete) {
                window.close();
            }
            else{
                if(event.key.code == Keyboard::F11) {
                    returnPressed = true;
                }
                else {
                    returnPressed = false;
                }

                // Transici�n entre pantallas
                if(event.key.code == Keyboard::M){
                    state = MENU;
                }
                if(event.key.code == Keyboard::G){
                    state = INGAME;
                }
                if(event.key.code == Keyboard::E){
                    state = ENDGAME;
                }
            }
        }

        // Alternar entre pantalla completa y ventana
        if(returnPressed) {
            isFullscreen = !isFullscreen;
            if(isFullscreen) {
                window.create(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), WINDOW_TITTLE, Style::Fullscreen);
            }
            else {
                window.create(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), WINDOW_TITTLE, Style::Default);
            }
        }

        if(state == MENU){ // Ejecuci�n del men�
            interfaz.menu(window, state);
        }
        else if(state == INGAME){ // Ejecuci�n del juego
            interfaz.game(window, state);
        }
        else if(state == ENDGAME){ // Ejecuci�n posjuego
            interfaz.endgame(window, state);
        }

        window.display();
    }

    return 0;
}