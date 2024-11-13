//
// Created by andre on 01/09/2021.
//

#ifndef TORRES_DE_HANOI_INTERFACE_H
#define TORRES_DE_HANOI_INTERFACE_H

#include "SFML/Graphics.hpp"
#include "torre.hpp"
using namespace sf;

#define LEVEL 6

class Interface {
private:
    // Variables generales
    unsigned int width, height; // Resoluci�n
    pair<Font,Text> *fps; // FPS
    // Variables del men�
    vector<pair<Font,Text>> *menu_texts;
    // Variables del juego
    vector<pair<Font,Text>> *game_texts;
    Torre *torre1, *torre2, *torre3, *torreAux;
    // Variables de fin de juego
    vector<pair<Font,Text>> *ending_texts;

public:
    Interface(unsigned int width, unsigned int height);
    ~Interface();
    void menu(RenderWindow &window, int &state);
    void game(RenderWindow &window, int &state);
    void endgame(RenderWindow &window, int &state);

private:
    bool tab_manage();
    bool first_selected();
    bool second_selected();
    bool third_selected();
};


#endif //TORRES_DE_HANOI_INTERFACE_H
