#include "movable.hpp"

#include "wall.hpp"

Movable::Movable(const sf::Vector2f& position, const std::string& name) {
    m_position = position;
    m_name = name;

    GameObject::m_texture_list = {
        name + "_neutral",
        name + "_left_0", 
        name + "_left_1", 
        name + "_right_0",
        name + "_right_1",
        name + "_up_0",
        name + "_up_1",
        name + "_down_0", 
        name + "_down_1", 

        "dead_left_0", 
        "dead_left_1", 
        "dead_right_0",
        "dead_right_1",
        "dead_up_0",
        "dead_up_1",
        "dead_down_0", 
        "dead_down_1", 
    };

    GameObject::initSprite();
}

Movable::~Movable() {}

sf::Vector2i Movable::getMapPosition() const {
    sf::Vector2i map_position;
    const float tile_size = Settings::instance()->tile_size;
    map_position.x = static_cast<int>(m_position.x / tile_size);
    map_position.y = static_cast<int>(m_position.y / tile_size);
    return map_position;
}

bool Movable::checkCollisions(const sf::RenderTarget* target, std::vector<std::vector<MapCell>>& map) {
    const sf::Vector2u window_size = target->getSize();
    const sf::Vector2f bounds = { m_sprite.getGlobalBounds().width, m_sprite.getGlobalBounds().height };

    // Collision with X axis
    if (m_position.x < 0.f) { m_position.x = 0.f; } 
    else if (m_position.x + bounds.x > window_size.x) { m_position.x = window_size.x - bounds.x; }
    // Collision with Y axis
    if (m_position.y < 0.f) { m_position.y = 0.f; } 
    else if (m_position.y + bounds.y > window_size.y) { m_position.y = window_size.y - bounds.y; }

    const sf::Vector2i delta = {
        (m_curr_dir.x > 0) ? 1 : 0,
        (m_curr_dir.y > 0) ? 1 : 0
    };
    const sf::Vector2i curr_pos = getMapPosition() + delta;

    // Collision with walls
    auto& cell = map[curr_pos.y][curr_pos.x];
    for (auto* obj : cell) {
        if (obj != nullptr && dynamic_cast<Wall*>(obj) != nullptr) {
            const int tile_size = static_cast<int>(Settings::instance()->tile_size);
            m_position = { round(m_position.x / tile_size) * tile_size, round(m_position.y / tile_size) * tile_size, };
            return false;
        }
    }

    return true;
}

bool Movable::checkDirection(std::vector<std::vector<MapCell>>& map) {
    auto settings = Settings::instance();
    const auto tile_size = settings->tile_size;
    const auto curr_map_pos = getMapPosition();
    const auto next_map_pos = curr_map_pos + m_next_dir;

    const sf::Vector2f in_cell = { 
        curr_map_pos.x * tile_size, 
        curr_map_pos.y * tile_size 
    };

    const float distance_threshold = m_speed * 0.5f;
    // Object can change direction only if it's fully entered into a cell
    // Adding some offset, because object can move too fast and skip the cell
    // Can be a potential bug nest for high movement speed
    if(abs(m_position.x - in_cell.x) >= distance_threshold || abs(m_position.y - in_cell.y) >= distance_threshold) { return false; }

    auto& cell = map[next_map_pos.y][next_map_pos.x];
    // Pacman cannot change direction to the wall 
    for (auto* obj : cell) {
        if (obj != nullptr && dynamic_cast<Wall*>(obj) != nullptr) {
            return false;
        }
    }

    m_curr_dir = m_next_dir;
    return true;
}

void Movable::updateAnimation() {
    std::string direction_name = "";

    if(m_curr_dir.x > 0) direction_name = "right";
    else if(m_curr_dir.x < 0) direction_name = "left";
    else if(m_curr_dir.y > 0) direction_name = "down";
    else if(m_curr_dir.y < 0) direction_name = "up";
    else direction_name = "right";

    m_texture_list = {
        m_name + "_" + "neutral",             
        m_name + "_" + direction_name + "_1", 
        m_name + "_" + direction_name + "_0", 
        m_name + "_" + direction_name + "_1"  
    };

    GameObject::loadTextures();
}
