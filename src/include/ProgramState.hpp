#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "MusicalController/notesDetector.h"
#include <chrono>

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

// Estadísticas de la última partida

struct LastGameStats {
    int moves;
    std::string time;
};


// Declaraciones de variables globales
extern const std::map<Appearance, std::string> AppearanceStrings;
extern const std::unordered_map<Appearance, std::string> AppearancePaths;
extern const std::map<ControlMode, std::string> ControlModeStrings;
extern NotesDetector notesDetector;
extern LastGameStats lastGameStats;

class ProgramState {
protected:
    // Configuración global del juego
    static unsigned int level;
    static Appearance currentAppearance;
    static ControlMode currentControlMode;
    static NotesDetector notesDetector;
    static LastGameStats lastGameStats;
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
    static void setControlMode(ControlMode newMode);

    static SelectedTower getSelectedTower();
    static std::string* getTriggerInputNames();
    static LastGameStats getLastGameStats() { return lastGameStats; }
    static void setLastGameStats(const LastGameStats& stats) { lastGameStats = stats; }
}; 