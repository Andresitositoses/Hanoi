#include "include/EndState.hpp"
#include "include/interface.hpp"

#define FONTS_PATH "Fonts\\"

EndState::EndState(unsigned int width, unsigned int height) 
    : width(width), height(height) {
    font = new sf::Font();
}

EndState::~EndState() {
    // Limpieza de memoria
    for(auto& [id, pair] : texts) {
        delete pair.first;
        delete pair.second;
    }
}

void EndState::init(sf::RenderWindow& window) {
    // Crear y almacenar la fuente de manera persistente
    if(font->loadFromFile((std::string)FONTS_PATH + "arial.ttf")) {
        sf::Text* text = new sf::Text();
        text->setFont(*font);
        text->setString("Torre resuelta");
        text->setCharacterSize(60);
        text->setPosition(width/2.0f, height/2.0f);
        text->setOrigin(text->getLocalBounds().width/2.0f, text->getLocalBounds().height/2.0f);
        
        texts[TextId::TITLE] = {font, text};
    }

    // Limpiar el texto de estadísticas anterior si existe
    auto it = texts.find(TextId::STATS);
    if (it != texts.end()) {
        delete it->second.second;
        texts.erase(it);
    }

    // Obtener las estadísticas del último juego
    LastGameStats stats = getLastGameStats();
    
    // Convertir el tiempo de string a float y formatear
    float seconds = std::stof(stats.time);
    int days = static_cast<int>(seconds) / (24 * 3600);
    seconds = std::fmod(seconds, 24 * 3600);
    int hours = static_cast<int>(seconds) / 3600;
    seconds = std::fmod(seconds, 3600);
    int minutes = static_cast<int>(seconds) / 60;
    int remainingSeconds = static_cast<int>(std::fmod(seconds, 60));

    // Construir el string del tiempo
    std::string timeStr;
    if (days > 0) {
        timeStr += std::to_string(days) + "d ";
    }
    if (hours > 0 || days > 0) {
        timeStr += std::to_string(hours) + "h ";
    }
    timeStr += std::to_string(minutes) + ":" + 
               (remainingSeconds < 10 ? "0" : "") + std::to_string(remainingSeconds);

    // Crear y configurar los textos de estadísticas
    sf::Text* statsText = new sf::Text();
    statsText->setFont(*font);
    statsText->setCharacterSize(30);
    statsText->setString("Movimientos realizados: " + std::to_string(stats.moves) + "\n" +
                        "Tiempo transcurrido: " + timeStr);
    statsText->setPosition(
        width/2.0f - statsText->getLocalBounds().width/2.0f,
        height/2.0f + 50.0f
    );
    
    texts[TextId::STATS] = {font, statsText};

    // Generar estrellas
    estrellas = generarEstrellas(width, height, 200, ColorMode::RANDOM);  // 200 estrellas
}

void EndState::run(sf::RenderWindow& window, int& state) {
    static bool escReleased = false;

    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        escReleased = true;
    }
    
    if (escReleased && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        // Limpiar estrellas
        estrellas.clear();

        state = MENU;
        escReleased = false;
    }
}

void EndState::draw(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
    
    // Actualizar y dibujar las estrellas
    float deltaTime = estrellasReloj.restart().asSeconds();
    actualizarEstrellas(estrellas, width, height, deltaTime);
    for(const auto& estrella : estrellas) {
        window.draw(estrella.forma);
    }
        
    // Dibujar los textos
    for(const auto& [id, pair] : texts) {
        window.draw(*pair.second);
    }
}