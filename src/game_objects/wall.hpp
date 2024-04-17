#pragma once

#include "game_object.hpp"

class Wall : public GameObject {
public:
    Wall(const sf::Vector2f&, const char);
    ~Wall();

    void update(const sf::RenderTarget*, std::vector<std::vector<MapCell>>&) override;
};