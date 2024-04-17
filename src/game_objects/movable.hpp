#pragma once

#include "game_object.hpp"

namespace Directions {
  static const sf::Vector2i Up    = { 0, -1};
  static const sf::Vector2i Down  = { 0,  1};
  static const sf::Vector2i Left  = {-1,  0};
  static const sf::Vector2i Right = { 1,  0};
  static const sf::Vector2i None  = { 0,  0};
};

class Movable : public GameObject {
public:
    Movable(const sf::Vector2f&, const std::string&);
    ~Movable();
    
    sf::Vector2i getMapPosition() const;
    //inline std::string getName() const { return m_name; }

protected:
    void updateAnimation();

    bool checkDirection(std::vector<std::vector<MapCell>>&);
    bool checkCollisions(const sf::RenderTarget*, std::vector<std::vector<MapCell>>&);

protected:
    float m_speed = 1.0f;
    
    std::string m_name = "";
    // The direction in which the character is/will be moving.
    sf::Vector2i m_next_dir = Directions::None;
    sf::Vector2i m_curr_dir = Directions::None;
};