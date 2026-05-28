#include "graphics/Texture.hpp"

#include <iostream>

#include <glad/include/glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

unsigned int TextureFromFile(const std::string& path)
{
    unsigned int textureID;

    glGenTextures(1, &textureID);

    int width;
    int height;
    int channelCount;

    stbi_set_flip_vertically_on_load(true);

    unsigned char* data =
        stbi_load(path.c_str(),
                  &width,
                  &height,
                  &channelCount,
                  0);

    if (!data)
    {
        std::cerr << "Failed to load texture: "
                  << path
                  << std::endl;

        return 0;
    }

    GLenum format = GL_RGB;

    if (channelCount == 1)
    {
        format = GL_RED;
    }
    else if (channelCount == 3)
    {
        format = GL_RGB;
    }
    else if (channelCount == 4)
    {
        format = GL_RGBA;
    }

    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 format,
                 width,
                 height,
                 0,
                 format,
                 GL_UNSIGNED_BYTE,
                 data);

    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S,
                    GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T,
                    GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);

    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER,
                    GL_LINEAR);

    stbi_image_free(data);

    std::cout << "Loaded texture: " << path << " (" << width << "x" << height << ", ch=" << channelCount << ") -> ID=" << textureID << std::endl;

    return textureID;
}


unsigned int loadCubeMap(std::vector<std::string> faces)
{
    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

    int w, h, ch;
    for (unsigned int i = 0; i < faces.size(); i++) {
        unsigned char* data = stbi_load(faces[i].c_str(), &w, &h, &ch, 0);
        if (!data) {
            std::cout << "Failed to load cubemap: " << faces[i] << std::endl;
            continue;
        }
        GLenum format = (ch == 4) ? GL_RGBA : GL_RGB;

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                     0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    return texID;
}