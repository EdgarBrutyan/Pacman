#include "game_object.hpp"

void GameObject::initSprite() {
    m_sprite = sf::Sprite();
    loadTextures();
    m_sprite.setPosition(m_position);
}

void GameObject::loadTextures() {
    if (!m_textures.empty()) 
        m_textures.clear();
    
    for (std::string &name : m_texture_list) {
        m_textures.push_back(Settings::instance()->textures[name] );
    }
}

void GameObject::updateSprite() {
    const int animation = static_cast<int>(m_frame_counter / m_animation_speed) % m_textures.size();
    m_sprite.setTexture(*(m_textures[animation]));

    m_frame_counter = (m_frame_counter + 1) % Settings::instance()->window_framerate;

    m_sprite.setPosition({
        m_position.x + Settings::instance()->horizontal_offset,
        m_position.y + Settings::instance()->vertical_offset,
    });
}

void GameObject::render(sf::RenderTarget *target) const {
    target->draw(m_sprite);
}
