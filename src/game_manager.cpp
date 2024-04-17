#include <fstream>

#include "game_manager.hpp"

#include "game_objects/game_object.hpp"
#include "game_objects/wall.hpp"
#include "game_objects/food.hpp"
#include "game_objects/pacman.hpp"
#include "game_objects/ghost.hpp"

GameManager::GameManager() {
    loadTextures();
    initMap();
    initWindow();
}

GameManager::~GameManager() {
    if (m_window != nullptr) {
        delete m_window;
    }

    for (auto& vec2 : m_map) {
        for(auto& cell : vec2) {
            for (GameObject* obj : cell) {
                if (obj != nullptr) 
                    delete obj;
            }
        }
    }

    for (auto& texture : Settings::instance()->textures) {
        delete texture.second;
    }
}

void GameManager::pollEvents() {
    sf::Event event;

    while (m_window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_window->close();
        }

        if (game_over && event.type == sf::Event::KeyPressed) {
            //m_window->close();
        }
    }
}

void GameManager::update() {
    pollEvents();
    
    if (Food::counter < 0) {
        game_over = true;
        sf::sleep(sf::seconds(2.f));
    }
    else{ 
        for (auto& vec2 : m_map) {
            for(auto& cell : vec2) {
                for (GameObject* obj : cell) {
                    if (obj != nullptr) {
                        obj->update(m_window, m_map);

                        if(dynamic_cast<Pacman*>(obj) != nullptr) {
                            game_over = !dynamic_cast<Pacman*>(obj)->isAlive();
                        }
                    }
                }
            }
        }
    }
}

void GameManager::render() const {
    m_window->clear();
    
    if (game_over) {
        //TODO print something
    }
    for (auto& vec2 : m_map) {
        for(auto& cell : vec2) {
            for (GameObject* obj : cell) {
                if (obj != nullptr) {
                    obj->render(m_window);
                }
            }
        }
    }

    m_window->display();
}

void GameManager::initWindow() {
    auto settings = Settings::instance();
    settings->window_width = settings->map_width * static_cast<int>(settings->tile_size) + settings->horizontal_offset * 2;
    settings->window_height = settings->map_height * static_cast<int>(settings->tile_size) + settings->vertical_offset * 2;

    const sf::VideoMode video_mode = sf::VideoMode(settings->window_width, settings->window_height);
    const sf::Uint32 style = sf::Style::Close | sf::Style::Titlebar;

    m_window = new sf::RenderWindow(video_mode, settings->window_title, style);
    m_window->setFramerateLimit(settings->window_framerate);
    m_window->setVerticalSyncEnabled(true);

    m_window->setVisible(true);
}

void GameManager::loadTextures() {
    auto settings = Settings::instance();

    for (auto &file : settings->map_texture_files) {
        sf::Texture *texture = new sf::Texture();
        if (texture->loadFromFile(file.second) == false) {
            std::cout << "ERROR: Failed to load " << file.second << std::endl;
            abort();
        }
        Settings::instance()->textures.emplace(file.first, texture);
    }

    for (auto& char_name : Settings::instance()->characters_names) {
        std::string prefix = std::string(1, toupper(char_name[0]));
        if(char_name == "pacman") prefix = "X";

        for(auto& tail : settings->characher_texture_files) {
            const std::string name = prefix + tail.first; 
            const std::string file = settings->res_path + "characters\\" + char_name + tail.second;

            sf::Texture *texture = new sf::Texture();
            if (texture->loadFromFile(file) == false) {
                std::cout << "ERROR: Failed to load " << file << std::endl;
                abort();
            }
            Settings::instance()->textures.emplace(name, texture);
        }
    }

    for (auto& ghost_state : Settings::instance()->ghost_state) {
        for(auto& tail : settings->characher_texture_files) {
            if(tail.first== "_neutral") continue;

            const std::string file = settings->res_path + "\\characters\\" + ghost_state + tail.second;
            const std::string name = ghost_state + tail.first; 

            sf::Texture *texture = new sf::Texture();
            if (texture->loadFromFile(file) == false) {
                std::cout << "ERROR: Failed to load " << file << std::endl;
                abort();
            }
            Settings::instance()->textures.emplace(name, texture);
        }
    }
} 

std::vector<std::vector<char>> GameManager::loadMap() {
    std::ifstream file(Settings::instance()->map, std::ios::binary);
    std::string line= "";
    std::size_t line_count = 0;
    std::size_t line_length = 0;

    std::vector<std::vector<char>> char_map = {};

    if (!file.is_open()) {
        std::cout << "ERROR: Cannot find map file" << std::endl;
        abort();
    }

    while (std::getline(file, line)) {
        if (line.size() > line_length) line_length = line.size();

        std::vector<char> temp = {};
        for (long unsigned int i = 0; i < line.size(); i++) {
            temp.push_back(line[i]);
        }
        char_map.push_back( temp );
        line_count++;
    }

    // Set map sizes.
    Settings::instance()->map_height = static_cast<int>(line_count);
    Settings::instance()->map_width = static_cast<int>(line_length);

    //TODO: fix
    //Settings::instance()->map_width = line_length % 2 ? line_length-1 : line_length;

    //std::cout<<"map width: "<< Settings::instance()->map_width <<"\n";
    //std::cout<<"map height: "<< Settings::instance()->map_height <<"\n";

    return char_map;
}

void GameManager::initMap() {
    Pacman* pacman = nullptr;
    std::vector<Ghost*> ghosts = {};

    auto char_map = loadMap();
    
    int i = 0, j = 0;
    for (auto &line : char_map) {
        //in one cell can be multiple object
        std::vector<MapCell> cell_list = {};
        
        for (char &c : line) {
            sf::Vector2f position = {
                i * Settings::instance()->tile_size,
                j * Settings::instance()->tile_size,
            };
            
            GameObject *game_obj = nullptr;

            switch (c) {
            case '1': case '2': case '3': case '4': case '5': case '6':
            case '7': case '8': case '9': case '0': case '-': case '+':
                game_obj = new Wall(position, c);
                break;

            case 'X':
                game_obj = new Pacman(position);
                pacman = dynamic_cast<Pacman*>(game_obj);
                break;

            case 'I':  case 'P': case 'B': case 'C':
                game_obj = new Ghost(position, std::string(1, c));
                ghosts.push_back(dynamic_cast<Ghost*>(game_obj));
                break;

            case '.': case 'o': case 'c': case 's': case 'y': case 'a':
                game_obj = new Food(position, std::string(1, c));
                break;
            }

            cell_list.push_back( {game_obj} );
            i++;
        }
        j++; i = 0;
        m_map.push_back(cell_list);
    }
    
    for (auto& ghost : ghosts) {
        ghost->setTarget(pacman);
    }
}

void GameManager::run() {
    while (m_window->isOpen()) {
        update();
        render();
    }
}