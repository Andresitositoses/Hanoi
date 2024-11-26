//
// Created by andre on 03/09/2021.
//

#ifndef TORRES_DE_HANOI_TORRE_H
#define TORRES_DE_HANOI_TORRE_H

#include "SFML/Graphics.hpp"
#include <unordered_map>
using namespace std;
using namespace sf;

// Definir Anilla como un alias para sf::RectangleShape
using Anilla = sf::RectangleShape;

// Enumerado para las apariencias
enum Appearance {
    COLORS = 0,
    WOODEN = 1,
    BRICK = 2, 
    ICE = 3,
    ROCKS = 4,
    UNIVERSE = 5
};

namespace std {
    template <>
    struct hash<Appearance> {
        size_t operator()(const Appearance& a) const {
            return static_cast<size_t>(a); // Asegúrate de que Appearance sea un enum o tenga un valor entero
        }
    };
}

// Mapa de apariencias a strings
static const std::unordered_map<Appearance, std::string> AppearanceStrings = {
    {COLORS, "COLORS"},
    {WOODEN, "img/wooden_disk.png"}, 
    {BRICK, "img/brick_disk.png"},
    {ICE, "img/ice_disk.png"},
    {ROCKS, "img/rocks_disk.png"},
    {UNIVERSE, "img/universe_disk.png"}
};

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
    void cambiarApariencia();
private:
    void generateDisk(double width, double height, double posX, double posY);
};


#endif //TORRES_DE_HANOI_TORRE_H
