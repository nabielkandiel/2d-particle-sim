#pragma once
#include <string>

struct SimulationSettings {
    bool load_config(const std::string& filename = "conf/settings.json");

    int num_balls = 12; 
};