//
// Created by andre on 03/09/2021.
//

#include "include/torre.hpp"

Torre::Torre(int numDisks, double posX, double posY, double width, double height) {
    this->numDisks = numDisks;
    this->posX = posX;
    this->posY = posY;
    this->diskWidth = width;
    this->diskHeight = height;
    this->prop = (double) (1. / numDisks);
    this->disks = new vector<RectangleShape>(sizeof(RectangleShape) * numDisks);

    // Establece una semilla de azar a partir de un instante de tiempo
    srand(time(NULL));

    // Genera la anilla base
    if(numDisks != 0){
        generateDisk(width, height,
                     posX, posY,
                     new Color(rand() % 256,rand() % 256, rand() % 256));
    }

    // Genera el resto de anillas
    for(double i=1; i < numDisks; i++){
        generateDisk(width - width * prop * i, height,
                     posX + (width * prop) / 2 * i, this->disks->back().getPosition().y - height - 4,
                     new Color(rand() % 256,rand() % 256, rand() % 256));
    }

}

Torre::~Torre() {
    free(disks);
}

void Torre::draw(RenderWindow &window) {
    for(int i=0; i < disks->size(); i++){
        window.draw(disks->at(i));
    }
}

bool Torre::addDisk(RectangleShape *disk) {
    //TODO: Terminar esto
    disk->setPosition(posX + (diskWidth * prop) / 2 * disks->size(), disks->back().getPosition().y - diskHeight - 4);
    disks->push_back(*disk);
    numDisks++;
    return true;
}

RectangleShape *Torre::popDisk() {
    if(numDisks == 0){
        return nullptr;
    }
    else {
        RectangleShape *disk = &disks->back();
        disks->pop_back();
        numDisks--;
        return disk;
    }
}

bool Torre::isPlaceable(RectangleShape disk) {
    return this->disks->front().getSize().x > disk.getSize().x;
}

void Torre::generateDisk(double width, double height, double posX, double posY, Color *color) {
    RectangleShape *disk = new RectangleShape(Vector2f(width, height));
    disk->setPosition(posX, posY);
    disk->setFillColor(*color);
    this->disks->push_back(*disk);
}