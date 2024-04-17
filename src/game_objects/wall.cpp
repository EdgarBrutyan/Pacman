#include "wall.hpp"

Wall::Wall(const sf::Vector2f &position, const char type) {
    m_position = position;
    
    m_texture_list = { std::string(1, type) };
    GameObject::initSprite();
}

Wall::~Wall() {}

void Wall::update(const sf::RenderTarget* target, std::vector<std::vector<MapCell>>& map) {
    GameObject::updateSprite();
}
