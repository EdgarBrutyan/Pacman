#include "pacman.hpp"

#include "wall.hpp"
#include "food.hpp"
#include "ghost.hpp"

Pacman::Pacman(const sf::Vector2f& position)
 : Movable(position, "X") 
 {
    m_position = position;
    Movable::m_speed = Settings::instance()->pacman_speed;
    Movable::m_curr_dir = Directions::None;

    GameObject::initSprite();
}

void Pacman::pollEvents() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) Movable::m_next_dir = Directions::Up;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) Movable::m_next_dir = Directions::Down;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) Movable::m_next_dir = Directions::Left;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) Movable::m_next_dir = Directions::Right;
}

void Pacman::checkInteractions(std::vector<std::vector<MapCell>>& map) {
    auto map_position = getMapPosition();
    auto& cell = map[map_position.y][map_position.x];
    
    for (auto* obj : cell) {
        if(obj == nullptr) { continue; }

        if(dynamic_cast<Food*>(obj) != nullptr && !dynamic_cast<Food*>(obj)->isEaten()) {  
            auto* food_obj = dynamic_cast<Food*>(obj);
            
            if (food_obj->isPowerPellet()) {
                for (auto& row : map) {
                    for(auto& cell : row) {
                        for(auto* obj : cell) {
                            if(dynamic_cast<Ghost*>(obj) != nullptr) { 
                                dynamic_cast<Ghost*>(obj)->updateState(GhostStates::Frightened);
                            }
                        }
                    }
                }
            }

            m_score += food_obj->getScore();
            food_obj->toEatenState();
            Food::counter--;
            return;
        }

        if(dynamic_cast<Ghost*>(obj) != nullptr) { 
            auto* ghost = dynamic_cast<Ghost*>(obj);
            auto state = ghost->getState();

            if (state == GhostStates::Frightened) {
                ghost->updateState(GhostStates::Dead);
                //sf::sleep(sf::seconds(1.f));
                return;
            }
            
            if (state != GhostStates::Dead && this->m_is_alive) {
                m_is_alive = false;
                sf::sleep(sf::seconds(2.f));
            }
            return;
        }
    }
}

void Pacman::update(const sf::RenderTarget* rt, std::vector<std::vector<MapCell>>& map) {
    pollEvents();
    const auto map_pos = Movable::getMapPosition();
    Movable::checkDirection(map);

    m_position = { 
        m_position.x + m_curr_dir.x * Movable::m_speed,
        m_position.y + m_curr_dir.y * Movable::m_speed
    };

    Movable::checkCollisions(rt, map);
    checkInteractions(map);

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

    Movable::updateAnimation();
    GameObject::updateSprite();
}

void Pacman::render(sf::RenderTarget *target) const {
    GameObject::render(target);
}
