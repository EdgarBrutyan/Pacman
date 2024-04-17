#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

#include <SFML/Graphics.hpp>

class GameObject;
using MapCell = std::vector<GameObject*>;

//TODO file-based INI JSON
class Settings {
public:
    static Settings* instance();
    Settings(const Settings&) = delete;
    Settings &operator=(const Settings&) = delete;

    int map_width = 0;
    int map_height = 0;
    int vertical_offset = 40;
    int horizontal_offset = 120;
    const float tile_size = 22.f;   // tile size, in pixels    
    
    int window_width = 0;
    int window_height = 0;
    const int window_framerate = 60;
    const std::string window_title = "Pacman";

    const float ghost_speed = 1.f;
    const float pacman_speed = 2.f;
    const sf::Time ghost_frightened_time = sf::seconds(10);

    std::unordered_map<std::string, sf::Texture*> textures;

    //const std::string res_path = "C:\\Users\\Volodia\\source\\pacman\\res\\";
    const std::string res_path = "..\\..\\res\\";
    const std::string map = res_path + "maps\\test2.map";        

    std::vector<std::string> characters_names = { "pacman", "inky", "pinky", "blinky", "clyde" };
    std::vector<std::string> ghost_state = { "frightened", "dead" };

    std::unordered_map<std::string, const std::string> characher_texture_files = {
        { "_neutral", "\\neutral.png"}, 
        { "_left_0",  "\\left_0.png" },
        { "_left_1",  "\\left_1.png" },
        { "_right_0", "\\right_0.png" },
        { "_right_1", "\\right_1.png" },
        { "_up_0",    "\\up_0.png" },
        { "_up_1",    "\\up_1.png" },
        { "_down_0",  "\\down_0.png" },
        { "_down_1",  "\\down_1.png" },       
    };

    std::unordered_map<std::string, const std::string> map_texture_files = {
        { "empty", res_path + "\\empty.png" },

        { "snack_pellet", res_path + "\\eatable\\snack_pellet.png" },   //.
        { "power_pellet", res_path + "\\eatable\\power_pellet.png" },   //o
        { "cherry",       res_path + "\\eatable\\cherry.png" },         //c
        { "strawberry",   res_path + "\\eatable\\strawberry.png" },     //s
        { "yellow_apple", res_path + "\\eatable\\yellow_apple.png" },   //y
        { "apple",        res_path + "\\eatable\\apple.png" },          //a

        { "1", res_path + "walls\\dbTopLeft.png" },     
        { "2", res_path + "walls\\dbTopRight.png" },   
        { "3", res_path + "walls\\dbBotLeft.png" },    
        { "4", res_path + "walls\\dbBotRight.png" },   
        { "5", res_path + "walls\\dbHorizontal.png" },      
        { "6", res_path + "walls\\dbVertical.png" },       
        { "7", res_path + "walls\\topLeft.png" },          
        { "8", res_path + "walls\\topRight.png" },     
        { "9", res_path + "walls\\botLeft.png" },      
        { "0", res_path + "walls\\botRight.png" },     
        { "-", res_path + "walls\\horizontal.png" },    
        { "+", res_path + "walls\\vertical.png" },     
    };

private:
    static Settings *instance_;
    Settings();
};
