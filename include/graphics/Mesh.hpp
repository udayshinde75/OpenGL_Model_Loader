#pragma once

#include <string>
#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class Shader;

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture
{
    unsigned int ID;
    std::string Type;
    std::string Path;
};

class Mesh
{
private:
    // CPU side Data;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector <Texture> textures;

    // GPU side data
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

private:
    void SetupMesh();

public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures);
    ~Mesh();

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

    void Draw(const Shader& Shader) const;

    const std::vector<Vertex>& GetVertices() const;
    const std::vector<unsigned int>& GetIndices() const;
    const std::vector<Texture>& GetTextures() const;
    unsigned int GetVAO() const;
};