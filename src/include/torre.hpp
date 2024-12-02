//
// Created by andre on 03/09/2021.
//

#ifndef TORRES_DE_HANOI_TORRE_H
#define TORRES_DE_HANOI_TORRE_H

#include "SFML/Graphics.hpp"
#include "ProgramState.hpp"

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
    Appearance appearance;
    sf::Texture diskTexture;
public:
    Torre(unsigned int level, double posX, double posY, double diskWidth, double diskHeight, Appearance appearance);
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
    sf::Vector2f getTopPosition() {return disks.back().getPosition();}
private:
    void generateDisk(double width, double height, double posX, double posY);
};


#endif //TORRES_DE_HANOI_TORRE_H
