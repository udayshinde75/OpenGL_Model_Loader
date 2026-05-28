#include "graphics/Mesh.hpp"
#include "graphics/Shader.hpp"

#include <iostream>
#include <utility>

#include <glad/include/glad/glad.h>


// Constructor
Mesh::Mesh(const std::vector<Vertex>& vertices,
           const std::vector<unsigned int>& indices,
           const std::vector<Texture>& textures)
    : vertices(vertices),
      indices(indices),
      textures(textures)
{
    SetupMesh();
}

// destructor
Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

// Move constructor
Mesh::Mesh(Mesh&& other) noexcept
{
    vertices = std::move(other.vertices);
    indices = std::move(other.indices);
    textures = std::move(other.textures);

    VAO = other.VAO;
    VBO = other.VBO;
    EBO = other.EBO;

    other.VAO = 0;
    other.VBO = 0;
    other.EBO = 0;
}

// Move Assignment operator
Mesh& Mesh::operator=(Mesh&& other) noexcept
{
    if (this != &other)
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);

        vertices = std::move(other.vertices);
        indices = std::move(other.indices);
        textures = std::move(other.textures);

        VAO = other.VAO;
        VBO = other.VBO;
        EBO = other.EBO;

        other.VAO = 0;
        other.VBO = 0;
        other.EBO = 0;
    }

    return *this;
}

// Private helped
void Mesh::SetupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Vertex Buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER,
                 vertices.size() * sizeof(Vertex),
                 vertices.data(),
                 GL_STATIC_DRAW);

    // Element Buffer

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices.size() * sizeof(unsigned int),
                 indices.data(),
                 GL_STATIC_DRAW);

    // Position Attribute
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          (void*)0);

    // Normal Attribute

    glEnableVertexAttribArray(1);

    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          (void*)offsetof(Vertex, Normal));

    // Texture Coordinate Attribute

    glEnableVertexAttribArray(2);

    glVertexAttribPointer(2,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}

// Draw call
void Mesh::Draw(const Shader& shader) const
{
    // Ensure the shader program is active before setting sampler uniforms.
    shader.Use();

    unsigned int diffuseNumber = 1;
    unsigned int specularNumber = 1;

    for (unsigned int i = 0; i < textures.size(); ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);

        std::string textureType = textures[i].Type;
        std::string uniformName;

        if (textureType == "texture_diffuse")
        {
            uniformName = "material.diffuse" + std::to_string(diffuseNumber++);
        }
        else if (textureType == "texture_specular")
        {
            uniformName = "material.specular" + std::to_string(specularNumber++);
        }

        if (!uniformName.empty())
        {
            shader.SetInt(uniformName, i);
        }

        if (textures[i].ID == 0)
        {
            //std::cout << "Warning: texture " << textures[i].Path << " has ID 0\n";
        }
        else
        {
            //std::cout << "Binding texture unit " << i << " -> " << uniformName << " (ID=" << textures[i].ID << ")\n";
        }

        glBindTexture(GL_TEXTURE_2D, textures[i].ID);
    }

    shader.SetFloat("material.shininess", 32.0f);

    glBindVertexArray(VAO);

    glDrawElements(GL_TRIANGLES,static_cast<unsigned int>(indices.size()),GL_UNSIGNED_INT,nullptr);

    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}

// Getters
const std::vector<Vertex>& Mesh::GetVertices() const
{
    return vertices;
}

const std::vector<unsigned int>& Mesh::GetIndices() const
{
    return indices;
}

const std::vector<Texture>& Mesh::GetTextures() const
{
    return textures;
}

unsigned int Mesh::GetVAO() const
{
    return VAO;
}