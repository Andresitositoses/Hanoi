#include "ProgramState.hpp"

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
    {WOODEN, "img/wooden_disk.png"}, 
    {BRICK, "img/brick_disk.png"},
    {ICE, "img/ice_disk.png"},
    {ROCKS, "img/rocks_disk.png"},
    {UNIVERSE, "img/universe_disk.png"}
};

const std::map<ControlMode, std::string> ControlModeStrings = {
    {KEYBOARD, "Teclado"},
    {FLUTE, "Flauta"}
};

// Definiciones e inicialización de variables estáticas
unsigned int ProgramState::level = 3;  // Nivel por defecto
Appearance ProgramState::currentAppearance = Appearance::COLORS;  // Apariencia por defecto
ControlMode ProgramState::currentControlMode = ControlMode::KEYBOARD;  // Modo de control por defecto 
NotesDetector ProgramState::notesDetector;