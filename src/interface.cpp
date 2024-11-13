//
// Created by andre on 01/09/2021.
//
#include "include/interface.hpp"
#include <iostream>
#include <chrono>
using namespace std;
using namespace sf;

#define FPS_SIZE 20
#define FONTS_PATH "Fonts\\"

struct Controls {
    Keyboard::Key first_towel = Keyboard::Key::J;
    Keyboard::Key second_towel = Keyboard::Key::K;
    Keyboard::Key third_towel = Keyboard::Key::L;
} controls;

int getNumDigits(int num);
void manage_movement(bool first, bool second, bool third, Torre *t1, Torre *t2, Torre *t3, Torre *tAux);

bool tab = false;
bool tab_pressed = false;

Interface::Interface(unsigned int width, unsigned int height) {

    // Dimensiones de la pantalla
    this->width = width;
    this->height = height;

    /* Posiciones de las torres: -|-|-|-
     * Se divide la pantalla en 4 partes y se sit�an las torres entre los l�mites.
     * Para centrar las posiciones de las torres, hay que desplazarlas la mitad de su anchura. */
    int diskWidth = height/7, diskHeight = height/35;
    this->torre1 = new Torre(LEVEL, width/4 - diskWidth/2, height - diskHeight*5, diskWidth, diskHeight);
    this->torre2 = new Torre(0, width/2 - diskWidth/2, height - diskHeight*5, diskWidth, diskHeight);
    this->torre3 = new Torre(0, width*3/4 - diskWidth/2, height - diskHeight*5, diskWidth, diskHeight);
    this->torreAux = new Torre(0, width*3/4 - diskWidth/2, 200, 300, diskHeight);

    // FPS
    Font *fps_font = new Font();
    fps_font->loadFromFile((string)FONTS_PATH + "arial.ttf");
    Text *fps_text = new Text("FPS: 0", *fps_font, FPS_SIZE);
    fps = new pair<Font,Text>();
    fps->first = *fps_font;
    fps->second = *fps_text;

    // Men� texts
    menu_texts = new vector<pair<Font,Text>>();
    Font *menu_font = new Font();
    if(menu_font->loadFromFile((string)FONTS_PATH + "stjelogo\\Stjldbl1.ttf")){
        Text *menu_text = new Text(L"Menú principal", *menu_font, 100);
        menu_text->setPosition(width/2, height/2);
        auto *menu_text_pair = new pair<Font,Text>();
        menu_text_pair->first = *menu_font;
        menu_text_pair->second = *menu_text;
        menu_texts->push_back(*menu_text_pair);
    }

    // Game texts
    game_texts = new vector<pair<Font,Text>>();
    Font *game_font = new Font();
    if(game_font->loadFromFile((string)FONTS_PATH + "stjelogo\\Stjldbl1.ttf")){
        Text *game_text = new Text("En juego", *game_font, 100);
        game_text->setPosition(width/4, 100);
        auto *game_text_pair = new pair<Font,Text>();
        game_text_pair->first = *game_font;
        game_text_pair->second = *game_text;
        game_texts->push_back(*game_text_pair);
    }

    // Ending game texts
    ending_texts = new vector<pair<Font,Text>>();
    Font *ending_font = new Font();
    if(ending_font->loadFromFile((string)FONTS_PATH + "stjelogo\\Stjldbl1.ttf")){
        Text *ending_text = new Text("Fin de juego", *ending_font, 100);
        ending_text->setPosition(width/2, height/2);
        auto *ending_text_pair = new pair<Font,Text>();
        ending_text_pair->first = *ending_font;
        ending_text_pair->second = *ending_text;
        ending_texts->push_back(*ending_text_pair);
    }

    // FPS
    auto *par = new pair<Font,Text>();
    par->first = *fps_font;
    par->second = *fps_text;

}

Interface::~Interface() {
    free(fps);
    free(menu_texts); free(game_texts); free(ending_texts);
    free(torre1); free(torre2); free(torre3); free(torreAux);
}

void Interface::menu(RenderWindow &window, int &state) {

    // Mostrar im�genes y textos
    window.clear();
    for(const pair<Font,Text>& texto : *menu_texts) {
        window.draw(texto.second);
    }
}

clock_t previous;
clock_t current;
void Interface::game(RenderWindow &window, int &state) {

    // Gesti�n de la entrada
    tab_manage();

    // Ejecución del movimiento
    manage_movement(Keyboard::isKeyPressed(controls.first_towel),
                    Keyboard::isKeyPressed(controls.second_towel),
                    Keyboard::isKeyPressed(controls.third_towel),
                    torre1, torre2, torre3, torreAux);

    // Mostrar im�genes y textos
    window.clear();
    torre1->draw(window);
    torre2->draw(window);
    torre3->draw(window);
    torreAux->draw(window);
    if(tab) { // Mostrar/Ocultar FPS con el tabulador
        current = clock();
        int fps_value = (int) ((double) CLOCKS_PER_SEC / ((current - previous)));
        previous = current;
        string str = "FPS: ";
        for(int i=0;i<getNumDigits(fps_value);i++) str += " ";
        str += to_string(fps_value);
        fps->second.setString(str);
        window.draw(fps->second);
    }
    for(const pair<Font,Text>& texto : *game_texts) {
        window.draw(texto.second);
    }
}

void Interface::endgame(RenderWindow &window, int &state) {

    // Mostrar im�genes y textos
    window.clear();
    for(const pair<Font,Text>& texto : *ending_texts) {
        window.draw(texto.second);
    }
}

bool Interface::tab_manage() {
    if(Keyboard::isKeyPressed(Keyboard::Tab) && !tab_pressed) {
        tab_pressed = true;
        tab = !tab;
    }
    else if(!Keyboard::isKeyPressed(Keyboard::Tab)){
        tab_pressed = false;
    }
    return tab;
}

bool Interface::first_selected() {return Keyboard::isKeyPressed(controls.first_towel);}
bool Interface::second_selected() {return Keyboard::isKeyPressed(controls.second_towel);}
bool Interface::third_selected() {return Keyboard::isKeyPressed(controls.third_towel);}

int getNumDigits(int num){
    if(num >= 100)
        return 0;
    else if(num >= 10)
        return 1;
    else return 2;
}

void manage_movement(bool first, bool second, bool third, Torre *t1, Torre *t2, Torre *t3, Torre *tAux) {
    // Gestionar la primera torre
    if(first && !second && !third) {
        if(tAux->isEmpty()) { // No hay ninguna anilla seleccionada
            tAux->addDisk(t1->popDisk());
        }
        else { // Hay una anilla seleccionada
            RectangleShape *disk = t1->isPlaceable(tAux->top()) ? tAux->popDisk() : nullptr;
            if(disk) t1->addDisk(disk);
        }
    }

    // Gestionar la segunda torre

    // Gestionar la tercera torre
}