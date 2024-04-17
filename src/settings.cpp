#include "settings.hpp"

Settings* Settings::instance_ = nullptr;

Settings::Settings() {}

Settings* Settings::instance() { 
    if (instance_ == nullptr) 
        instance_ = new Settings();
    return instance_;
}