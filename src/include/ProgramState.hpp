#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "MusicalController/notesDetector.h"

// Apariencias

enum Appearance {
    COLORS,
    WOODEN,
    BRICK, 
    ICE,
    ROCKS,
    UNIVERSE
};

// Controles

enum ControlMode {
    KEYBOARD,
    FLUTE
};

// Torre seleccionada

enum SelectedTower {
    NO_TOWER,
    LEFT_TOWER,
    MIDDLE_TOWER,
    RIGHT_TOWER
};


// Declaraciones de variables globales
extern const std::map<Appearance, std::string> AppearanceStrings;
extern const std::unordered_map<Appearance, std::string> AppearancePaths;
extern const std::map<ControlMode, std::string> ControlModeStrings;
extern NotesDetector notesDetector;

class ProgramState {
protected:
    // Configuración global del juego
    static unsigned int level;
    static Appearance currentAppearance;
    static ControlMode currentControlMode;
    static NotesDetector notesDetector;
public:
    virtual ~ProgramState() = default;
    virtual void init(sf::RenderWindow& window) = 0;
    virtual void run(sf::RenderWindow& window, int& state) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;

    // Getters y setters estáticos
    static unsigned int getLevel() { return level; }
    static void setLevel(unsigned int newLevel) { level = newLevel; }
    
    static Appearance getAppearance() { return currentAppearance; }
    static std::string getAppearanceString() { return AppearanceStrings.at(currentAppearance); }
    static void setAppearance(Appearance newAppearance) { currentAppearance = newAppearance; }

    static ControlMode getControlMode() { return currentControlMode; }
    static std::string getControlModeString() { return ControlModeStrings.at(currentControlMode); }
    static void setControlMode(ControlMode newMode) {
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

    //TODO: Añadir método para obtener la torre seleccionada en función del modo de control
    static SelectedTower getSelectedTower() { 
        switch (currentControlMode) {
            case KEYBOARD:
                return sf::Keyboard::isKeyPressed(sf::Keyboard::J) ? SelectedTower::LEFT_TOWER :
                       sf::Keyboard::isKeyPressed(sf::Keyboard::K) ? SelectedTower::MIDDLE_TOWER :
                       sf::Keyboard::isKeyPressed(sf::Keyboard::L) ? SelectedTower::RIGHT_TOWER : SelectedTower::NO_TOWER;
            case FLUTE:
                Note detectedNote = notesDetector.getDetectedNote();
                if (notesDetector.getDetectedNoteString() != "NONE") {
                    std::cout << "Nota detectada: " << notesDetector.getDetectedNoteString() << std::endl;
                }
                return detectedNote == Note::SI ? SelectedTower::LEFT_TOWER :
                       detectedNote == Note::LA ? SelectedTower::MIDDLE_TOWER :
                       detectedNote == Note::SOL ? SelectedTower::RIGHT_TOWER : SelectedTower::NO_TOWER;
        }
        return SelectedTower::NO_TOWER;
    }
}; 