#pragma once
#include "game_object.hpp"

class Food : public GameObject {
public:
    Food(const sf::Vector2f&, const std::string&);    
    ~Food() = default;

    void toEatenState();
    void update(const sf::RenderTarget *, std::vector<std::vector<MapCell>>&) override;
    
    bool isEaten() const { return !m_active;}
    bool isPowerPellet() const { return m_name == "power_pellet"; }
    unsigned int getScore() const { return Food::food_score.at(m_name); }

private:
    void updateAnimation();

public:
    static int counter;
    
private:
    std::string m_name;
    bool m_active = true;
    
    static const std::map<std::string, unsigned int> food_score;
};