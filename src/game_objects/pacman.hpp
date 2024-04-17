#pragma once

#include "movable.hpp"

class Pacman : public Movable {
public:
    Pacman(const sf::Vector2f&);
    ~Pacman() = default;

    void update(const sf::RenderTarget*, std::vector<std::vector<MapCell>>&) override;
    void render(sf::RenderTarget *) const override;
    
    bool isAlive() const { return m_is_alive; }
    
private:
    void pollEvents();
    void checkInteractions(std::vector<std::vector<MapCell>>&);

private:
    int m_score = 0;
    bool m_is_alive = true;
};