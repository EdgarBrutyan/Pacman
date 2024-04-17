#include "ghost.hpp"

#include <random>

#include "wall.hpp"
#include "food.hpp"

const sf::Vector2i Ghost::escape_position = {13, 11};
const sf::Vector2i Ghost::home_position = {13, 13};
sf::Clock Ghost::frightened_timer = sf::Clock();

Ghost::Ghost(const sf::Vector2f& position, const std::string& name)
 : Movable(position, name) 
 {
    m_position = position;
    Movable::m_speed = Settings::instance()->ghost_speed;

    GameObject::initSprite();
}

bool Ghost::canMove(std::vector<std::vector<MapCell>>& map, const sf::Vector2i& dir) {
    // Ghosts cannot turn around 180 degrees
    if (m_curr_dir == -dir) { return false; }
    
    // check wall collisions
    const auto new_pos = Movable::getMapPosition() + dir;
    auto& cell = map[new_pos.y][new_pos.x];
    for (auto* obj : cell) {
        if (obj != nullptr && dynamic_cast<Wall*>(obj) != nullptr) {
            return false;
        }
    }

    return true;
}

bool Ghost::insideHome(const sf::Vector2i& pos) const {
    return pos.x >= 11 && pos.x <= 16 && pos.y >= 13 && pos.y <= 15;
}

sf::Vector2i Ghost::chase(std::vector<std::vector<MapCell>>& map, const sf::Vector2i& dest) {
    std::vector<unsigned int> weights(4, ((uint32_t)-1));
    std::vector<sf::Vector2i> all_dirs = {Directions::Up, Directions::Down, Directions::Left, Directions::Right};

    for (unsigned int i = 0; i < 4; i++) {
        auto dir = all_dirs[i];
        auto new_pos = Movable::getMapPosition() + dir;
        
        if(!canMove(map, dir)) { continue; }

        const int deltaX = dest.x - new_pos.x;
        const int deltaY = dest.y - new_pos.y;

        weights[i] = (deltaX * deltaX) + (deltaY * deltaY);
    }
    
    return all_dirs[std::distance(weights.begin(), std::min_element(weights.begin(), weights.end()))];
}   

sf::Vector2i Ghost::random(std::vector<std::vector<MapCell>>& map) {
    std::vector<sf::Vector2i> available_directions = {};
    std::vector<sf::Vector2i> all_dirs = {Directions::Up, Directions::Down, Directions::Left, Directions::Right};

    for (auto dir : all_dirs) {
        if(!canMove(map, dir)) { continue; }
        available_directions.push_back(dir);
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    
    auto random = [&rd, &gen](unsigned int size) -> unsigned int {
        std::uniform_int_distribution<> dist(0, size-1);
        return dist(gen);
    };
    
    if (available_directions.empty()) { return m_curr_dir; }

    return available_directions.at(random(available_directions.size()));
}

void Ghost::doMove(std::vector<std::vector<MapCell>> &map) {
    sf::Vector2i dest = {};

    switch (m_state) {
        case GhostStates::Escape: 
            dest = Ghost::escape_position;
            m_next_dir = chase(map, dest);
            break;

        case GhostStates::Chase:
            dest = m_target->getMapPosition();
            m_next_dir = chase(map, dest);
            break;

        case GhostStates::Scatter:
            dest = m_target->getMapPosition();
            m_next_dir = chase(map, dest);
            break;

        case GhostStates::Frightened:
            m_next_dir = random(map);
            break;

        case GhostStates::Dead:
            dest = Ghost::home_position;
            m_next_dir = chase(map, dest);
            break;
    }

    auto pos = Movable::getMapPosition();
    if ((pos.x == 13 && pos.y == 11) || (pos.x == 14 && pos.y == 11)) {
        if (m_next_dir == Directions::Down && m_state != GhostStates::Dead)
            m_next_dir = m_curr_dir;
    }
}

void Ghost::updateState(GhostStates state) {
    if (state != m_state) {
        switch (state)
        {
            case GhostStates::Dead: 
                if(m_state == GhostStates::Frightened) { m_state = state; }
                break;

            case GhostStates::Frightened: 
                if(m_state == GhostStates::Dead || m_state == GhostStates::Escape) { break; }
                m_state = state; 
                frightened_timer.restart();
                Movable::m_speed = 0.7f;
                break;

            case GhostStates::Scatter:
            case GhostStates::Escape:
            case GhostStates::Chase:
                Movable::m_speed = Settings::instance()->ghost_speed;
                break;
        }
    }
 
    switch (m_state)
    {
        case GhostStates::Frightened:
            Movable::m_speed = 0.7f;
            if(Ghost::frightened_timer.getElapsedTime().asSeconds() <= Settings::instance()->ghost_frightened_time.asSeconds()) { break; }
            m_state = GhostStates::Scatter;
            break;

        case GhostStates::Scatter:
            Movable::m_speed = Settings::instance()->ghost_speed;
            break;

        case GhostStates::Chase:
            Movable::m_speed = Settings::instance()->ghost_speed;
            break;

        case GhostStates::Escape:
            // Ghost can't change state until it's inside the house
            if (Movable::getMapPosition() != Ghost::escape_position) { break; }
            m_state = GhostStates::Scatter;
            break;

        case GhostStates::Dead:
            Movable::m_speed = 4.0f;
            // Dead ghost cannot change state until it arrives home
            if (!insideHome(Movable::getMapPosition())) { break; }
            m_state = GhostStates::Escape;
            break;
    }
}

void Ghost::update(const sf::RenderTarget* rt, std::vector<std::vector<MapCell>>& map) {
    updateState(m_state);
    doMove(map);

    if (this->m_state == GhostStates::Dead) {
        std::cout<<"hi\n";
    }

    const auto map_pos = Movable::getMapPosition();    
    Movable::checkDirection(map);

    m_position = { 
        m_position.x + m_curr_dir.x * Movable::m_speed,
        m_position.y + m_curr_dir.y * Movable::m_speed
    };

    Movable::checkCollisions(rt, map);

    const auto new_pos = getMapPosition();
    if (map_pos != new_pos) { 
        auto& cell = map[map_pos.y][map_pos.x];
        //remove from current position
        if(!cell.empty()) {
            for (auto it = cell.begin(); it != cell.end(); it++) {
                if (*it == this) {
                    cell.erase(it);
                    break;
                }
            }
        }
        //add to new position
        map[new_pos.y][new_pos.x].push_back(this);
    }

    updateAnimation();
    GameObject::updateSprite();
}

void Ghost::updateAnimation() {
    m_animation_speed = 8;
    std::string direction_name = "right";
    if(m_curr_dir.x > 0) direction_name = "right";
    else if(m_curr_dir.x < 0) direction_name = "left";
    else if(m_curr_dir.y > 0) direction_name = "down";
    else if(m_curr_dir.y < 0) direction_name = "up";

    if(m_state == GhostStates::Dead) {
        m_texture_list = { "dead_" + direction_name + "_0", "dead_" + direction_name + "_1", };
    }
    else if(m_state == GhostStates::Frightened) {
        m_texture_list = { "frightened_" + direction_name + "_0", };

        if(Ghost::frightened_timer.getElapsedTime().asSeconds() >= Settings::instance()->ghost_frightened_time.asSeconds()*0.6) {
            m_animation_speed = 4; 
            m_texture_list = { "frightened_" + direction_name + "_1", "frightened_" + direction_name + "_0" };
        }
    }
    else {
        m_texture_list = {
            Movable::m_name + "_" + "neutral",             
            Movable::m_name + "_" + direction_name + "_1", 
            Movable::m_name + "_" + direction_name + "_0", 
            Movable::m_name + "_" + direction_name + "_1"  
        };
    }

    GameObject::loadTextures();    
}

void Ghost::render(sf::RenderTarget *target) const {
    GameObject::render(target);
}
