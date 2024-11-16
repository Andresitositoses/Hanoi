//
// Created by andre on 03/09/2021.
//

#ifndef TORRES_DE_HANOI_TORRE_H
#define TORRES_DE_HANOI_TORRE_H

#include "SFML/Graphics.hpp"
using namespace std;
using namespace sf;

class Torre {
private:
    double baseDiskWidth, diskHeight;
    double posX, posY;
    double prop;
    vector<RectangleShape> disks;
public:
    Torre(int numDisks, double posX, double posY, double diskWidth, double diskHeight);
    ~Torre();
    bool addDisk(RectangleShape* disk);
    RectangleShape* popDisk();
    RectangleShape top() {return disks.back();}
    void draw(RenderWindow &window);
    bool isPlaceable(RectangleShape disk);
    bool isEmpty() {return disks.empty();}
private:
    void generateDisk(double width, double height, double posX, double posY, Color* color);
};


#endif //TORRES_DE_HANOI_TORRE_H
