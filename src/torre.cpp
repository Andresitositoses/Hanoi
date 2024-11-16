//
// Created by andre on 03/09/2021.
//

#include "include/torre.hpp"

Torre::Torre(int numDisks, double posX, double posY, double width, double height) {
    this->posX = posX; // Posición X de la torre (Coordenada X de la anilla base)
    this->posY = posY; // Posición Y de la torre (Coordenada Y de la anilla base)
    this->baseDiskWidth = width; // Ancho de la anilla base
    this->diskHeight = height; // Altura de las anillas
    this->prop = (numDisks > 0) ? (double) (1. / numDisks) : 0; // Proporción de ancho de las anillas
    this->disks = vector<RectangleShape>(); // Vector de anillas

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
                     posX + (width * prop) / 2 * i, this->disks.back().getPosition().y - height,
                     new Color(rand() % 256,rand() % 256, rand() % 256));
    }

}

// Destructor de la torre
Torre::~Torre() {
    disks.clear();
}

// Genera una anilla y la añade a la torre
void Torre::generateDisk(double width, double height, double posX, double posY, Color *color) {
    RectangleShape* disk = new RectangleShape(Vector2f(width, height));
    disk->setPosition(posX, posY);
    disk->setFillColor(color != nullptr ? *color : Color::White);
    this->disks.push_back(*disk);
}

// Añade una anilla externa a la torre
bool Torre::addDisk(RectangleShape* disk) {
    disk->setPosition(posX + disk->getSize().x / 2, disks.back().getPosition().y - diskHeight);
    disks.push_back(*disk);
    return true;
}

// Elimina y devuelve una copia de la anilla superior de la torre
RectangleShape* Torre::popDisk() {
    if(disks.size() == 0){
        return nullptr;
    }
    else {
        RectangleShape* disk = new RectangleShape(disks.back());
        disks.pop_back();
        return disk;
    }
}

// Comprueba si una determinada anilla puede ser colocada en la torre
bool Torre::isPlaceable(RectangleShape disk) {
    if (disks.empty()) {
        return true;
    }
    return disks.back().getSize().x > disk.getSize().x;
}

// Dibuja la torre en la ventana
void Torre::draw(RenderWindow &window) {
    for(int i=0; i < disks.size(); i++){
        window.draw(disks.at(i));
    }
}