#pragma once

#include "movable.hpp"

enum class GhostStates {
    Frightened,
    Scatter,
    Chase,
    Escape,
    Dead,
};

class Ghost : public Movable {
public:
    Ghost(const sf::Vector2f&, const std::string&);
    ~Ghost() = default;

    void update(const sf::RenderTarget*, std::vector<std::vector<MapCell>>&) override;
    void render(sf::RenderTarget *) const override;

    void updateState(GhostStates state);
    void setTarget(Movable* target) { m_target = target; }

    GhostStates getState() const { return m_state; }

private:
    bool insideHome(const sf::Vector2i&) const;
    bool canMove(std::vector<std::vector<MapCell>>&, const sf::Vector2i&);

    sf::Vector2i chase(std::vector<std::vector<MapCell>>&, const sf::Vector2i &); 
    sf::Vector2i random(std::vector<std::vector<MapCell>>&); 
    
    void updateAnimation();
    void doMove(std::vector<std::vector<MapCell>>&);
private:
    static sf::Clock frightened_timer;

    Movable* m_target = nullptr;

public:
    GhostStates m_state = GhostStates::Escape;

    const static sf::Vector2i home_position;
    const static sf::Vector2i escape_position;
};