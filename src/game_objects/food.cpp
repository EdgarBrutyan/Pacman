#include "food.hpp"

int Food::counter = 0;
const std::map<std::string, unsigned int> Food::food_score = {
    { "snack_pellet", 10 },
    { "power_pellet",  50 },
    { "cherry",        100 },
    { "strawberry",    300 },
    { "yellow_apple",  500 },
    { "apple",         700 }
};

Food::Food(const sf::Vector2f& position, const std::string& name) {
    m_position = position;
    counter++;

    switch (name.at(0))
    {
        case '.': m_name = "snack_pellet"; break;
        case 'o': m_name = "power_pellet";  break;
        case 'c': m_name = "cherry";        break;
        case 's': m_name = "strawberry";    break;
        case 'y': m_name = "yellow_apple";  break;
        case 'a': m_name = "apple";         break;
    }

    m_texture_list = {  m_name };
    GameObject::initSprite();
}

void Food::update(const sf::RenderTarget *target, std::vector<std::vector<MapCell>>& map) {
    updateAnimation();
    GameObject::updateSprite();
};

void Food::toEatenState() {
    m_texture_list = { "empty" };
    m_active = false;

    //change texture to empty
    GameObject::loadTextures();
}

void Food::updateAnimation() {
    if(m_name == "power_pellet" && m_active) {
        m_animation_speed = 25;
        m_texture_list = {
            "empty",
            "power_pellet"
        };
    }
    
    GameObject::loadTextures();
}