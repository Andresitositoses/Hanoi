//
// Created by andre on 03/09/2021.
//

#include "include/torre.hpp"

Torre::Torre(unsigned int level, double posX, double posY, double width, double height, Appearance appearance) 
    : posX(posX), posY(posY), baseDiskWidth(width), diskHeight(height), prop((level > 0) ? (double) (1. / level) : 0), appearance(appearance) {
    this->disks = vector<Anilla>(); // Vector de anillas
    this->level = level;

    // Cargar la textura en el constructor
    if (appearance == Appearance::COLORS) {
        srand(time(NULL));
    } else {
        string path = AppearanceStrings.at(appearance);
        if (!diskTexture.loadFromFile(path)) {
            // Manejar error si no se puede cargar
        }
    }
}

// Destructor de la torre
Torre::~Torre() {
    disks.clear();
}

// Llena la torre con un número determinado de anillas
void Torre::fill() {
    // Genera la anilla base
    if(level != 0){
        generateDisk(baseDiskWidth, diskHeight, posX, posY);
    }

    // Genera el resto de anillas
    for(double i=1; i < level; i++){
        generateDisk(baseDiskWidth - baseDiskWidth * prop * i, diskHeight,
                     posX + (baseDiskWidth * prop) / 2 * i, this->disks.back().getPosition().y - diskHeight);
    }
}

// Vacía la torre
void Torre::empty() {
    disks.clear();
}

// Genera una anilla y la añade a la torre
void Torre::generateDisk(double width, double height, double posX, double posY) {
    Anilla* disk = new Anilla(Vector2f(width, height));
    disk->setPosition(posX, posY);
    if (appearance == Appearance::COLORS) {
        disk->setTexture(nullptr);
        disk->setFillColor(Color(rand() % 256, rand() % 256, rand() % 256));
    } else {
        disk->setTexture(&diskTexture);
    }
    this->disks.push_back(*disk);
    delete disk;
}

// Añade una anilla externa a la torre
bool Torre::addDisk(Anilla* disk) {
    if (disks.empty()) {
        disk->setPosition(posX, posY);
    } else {
        disk->setPosition(disks.back().getPosition().x + (disks.back().getSize().x - disk->getSize().x) / 2, disks.back().getPosition().y - diskHeight);
    }
    disks.push_back(*disk);
    return true;
}

// Elimina y devuelve una copia de la anilla superior de la torre
Anilla* Torre::popDisk() {
    if(disks.size() == 0){
        return nullptr;
    }
    else {
        Anilla* disk = new Anilla(disks.back());
        disks.pop_back();
        return disk;
    }
}

// Comprueba si una determinada anilla puede ser colocada en la torre
bool Torre::isPlaceable(Anilla disk) {
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

void Torre::cambiarApariencia() {
    // Alternar entre las apariencias existentes
    appearance = (appearance == WOODEN) ? COLORS : WOODEN;
    
    // Actualizar la apariencia de todas las anillas existentes
    for(auto& disk : disks) {
        if (appearance == WOODEN) {
            disk.setTexture(&diskTexture);
            disk.setFillColor(sf::Color::White); // Reset color
        } else {
            disk.setTexture(nullptr);
            disk.setFillColor(Color(rand() % 256, rand() % 256, rand() % 256));
        }
    }
}