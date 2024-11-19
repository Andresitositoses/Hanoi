//
// Created by andre on 03/09/2021.
//

#ifndef TORRES_DE_HANOI_TORRE_H
#define TORRES_DE_HANOI_TORRE_H

#include "SFML/Graphics.hpp"
using namespace std;
using namespace sf;

// Definir Anilla como un alias para sf::RectangleShape
using Anilla = sf::RectangleShape;

class Torre {
private:
    double baseDiskWidth, diskHeight;
    double posX, posY;
    double prop;
    vector<Anilla> disks;
    unsigned int level;
public:
    Torre(unsigned int level, double posX, double posY, double diskWidth, double diskHeight);
    ~Torre();
    bool addDisk(Anilla* disk);
    Anilla* popDisk();
    Anilla top() {return disks.back();}
    void fill();
    void empty();
    bool isPlaceable(Anilla disk);
    bool isEmpty() {return disks.empty();}
    bool isComplete() {return disks.size() == level;}
    void draw(RenderWindow &window);
private:
    void generateDisk(double width, double height, double posX, double posY, Color* color);
};


#endif //TORRES_DE_HANOI_TORRE_H
