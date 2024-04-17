#pragma once

#include "../settings.hpp"

class GameObject {
public:
    GameObject() {}
    virtual ~GameObject() {}

    sf::Vector2f getPosition() const { return m_position; }

    virtual void update(const sf::RenderTarget *, std::vector<std::vector<MapCell>>&) = 0;
    virtual void render(sf::RenderTarget *) const;

protected:
    void initSprite();
    void loadTextures();
    void updateSprite();

protected:
    int m_frame_counter = 0;
    float m_animation_speed = 8;

    sf::Sprite m_sprite;
    sf::Vector2f m_position;
    std::vector<sf::Texture*> m_textures;
    std::vector<std::string> m_texture_list;
};