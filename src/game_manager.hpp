#pragma once
#include <vector>
#include "settings.hpp"

class GameManager {
public:
    GameManager();
    ~GameManager();

    void run();

private:
    void update();
    void render() const;
    void pollEvents();

    void loadTextures();
    std::vector<std::vector<char>> loadMap();
    void initMap();
    void initWindow();

private:
    bool game_over = false;

    sf::RenderWindow *m_window;
    std::vector<std::vector<MapCell>> m_map;
};