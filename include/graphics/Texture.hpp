#pragma once

#include <string>
#include <vector>

unsigned int TextureFromFile(const std::string& path);

unsigned int LoadCubeMap(std::vector<std::string> faces);