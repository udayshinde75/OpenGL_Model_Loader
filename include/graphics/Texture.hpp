#pragma once

#include <string>

unsigned int TextureFromFile(const std::string& path);

unsigned int LoadCubeMap(std::vector<std::string> faces);