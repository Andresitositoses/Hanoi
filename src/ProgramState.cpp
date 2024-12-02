#include "include/ProgramState.hpp"

using namespace std;

// Inicialización de las variables globales
const std::map<Appearance, std::string> AppearanceStrings = {
    {COLORS, "Colores aleatorios"},
    {WOODEN, "Madera"},
    {BRICK, "Ladrillos"},
    {ICE, "Hielo"},
    {ROCKS, "Rocas"},
    {UNIVERSE, "Universo"}
};

const std::unordered_map<Appearance, std::string> AppearancePaths = {
    {COLORS, ""},
    {WOODEN, "img/appearances/wooden_disk.png"}, 
    {BRICK, "img/appearances/brick_disk.png"},
    {ICE, "img/appearances/ice_disk.png"},
    {ROCKS, "img/appearances/rocks_disk.png"},
    {UNIVERSE, "img/appearances/universe_disk.png"}
};

const std::map<ControlMode, std::string> ControlModeStrings = {
    {KEYBOARD, "Teclado"},
    {FLUTE, "Flauta"}
};

LastGameStats ProgramState::lastGameStats;

// Definiciones e inicialización de variables estáticas
unsigned int ProgramState::level = 3;  // Nivel por defecto
Appearance ProgramState::currentAppearance = Appearance::COLORS;  // Apariencia por defecto
ControlMode ProgramState::currentControlMode = ControlMode::KEYBOARD;  // Modo de control por defecto 
NotesDetector ProgramState::notesDetector;

void ProgramState::setControlMode(ControlMode newMode) {
    // Detención del mecanismo de entrada
    switch (currentControlMode) {
        case KEYBOARD:
            break;
        case FLUTE:
            notesDetector.stop();
            break;
        default:
            break;
    }
    // Inicialización del mecanismo de entrada
    switch(newMode) {
        case KEYBOARD:
            break;
        case FLUTE:
            notesDetector.start();
            break;
    }
    currentControlMode = newMode;
}

SelectedTower ProgramState::getSelectedTower() { 
    switch (currentControlMode) {
        case KEYBOARD:
            return sf::Keyboard::isKeyPressed(sf::Keyboard::J) ? SelectedTower::LEFT_TOWER :
                   sf::Keyboard::isKeyPressed(sf::Keyboard::K) ? SelectedTower::MIDDLE_TOWER :
                   sf::Keyboard::isKeyPressed(sf::Keyboard::L) ? SelectedTower::RIGHT_TOWER : SelectedTower::NO_TOWER;
        case FLUTE:
            Note detectedNote = notesDetector.getDetectedNote();
            return detectedNote == Note::SI ? SelectedTower::LEFT_TOWER :
                   detectedNote == Note::SOL ? SelectedTower::MIDDLE_TOWER :
                   detectedNote == Note::MI ? SelectedTower::RIGHT_TOWER : SelectedTower::NO_TOWER;
    }
    return SelectedTower::NO_TOWER;
}

std::string* ProgramState::getTriggerInputNames() {
    std::string* triggerInputNames = new std::string[3];
    switch (currentControlMode)
    {
        case KEYBOARD:
            triggerInputNames[0] = "j";
            triggerInputNames[1] = "k";
            triggerInputNames[2] = "l";
            break;
        case FLUTE:
            triggerInputNames[0] = "si";
            triggerInputNames[1] = "sol";
            triggerInputNames[2] = "mi";
            break;
        default:
            break;
    }
    return triggerInputNames;
}