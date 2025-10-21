#pragma once
#include <string>

struct SimulationSettings {
  bool load_config(const std::string &filename = "data/settings.ini");

  int num_balls = 12;
};