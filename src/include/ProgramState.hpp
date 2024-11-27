#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>


// Apariencias

enum Appearance {
    COLORS,
    WOODEN,
    BRICK, 
    ICE,
    ROCKS,
    UNIVERSE
};

extern const std::map<Appearance, std::string> AppearanceStrings;

// Mapa de apariencias a paths
extern const std::unordered_map<Appearance, std::string> AppearancePaths;

// Controles

enum ControlMode {
    KEYBOARD,
    FLUTE
};

extern const std::map<ControlMode, std::string> ControlModeStrings;

class ProgramState {
protected:
    // Configuración global del juego
    static unsigned int level;
    static Appearance currentAppearance;
    static ControlMode currentControlMode;

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
    static void setControlMode(ControlMode newMode) { currentControlMode = newMode; }
}; 

// Declaraciones de variables globales
extern const std::map<Appearance, std::string> AppearanceStrings;
extern const std::unordered_map<Appearance, std::string> AppearancePaths;
extern const std::map<ControlMode, std::string> ControlModeStrings;