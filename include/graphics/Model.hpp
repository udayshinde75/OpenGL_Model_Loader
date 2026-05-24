#pragma once

#include <string>
#include <vector>

#include "Mesh.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

class Shader;

class Model
{
    private:
    // Mesh Data
    std::vector<Mesh> meshes;

    // Texture cache for efficiency
    std::vector<Texture> loadedTextures;

    // Model informatiom
    std::string directory;

    private:
    void LoadModel(const std::string& path);
    void ProcessNode(aiNode* node, const aiScene* scene);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> LoadMaterialTextures(aiMaterial* material, aiTextureType type, const std::string& typeName);

    public:
    explicit Model(const std::string& path);
    ~Model() = default;

    Model(const Model&) = delete;
    Model& operator= (const Model&) = delete;

    Model(Model&&) noexcept = default;
    Model& operator=(Model&&) noexcept = default;
    void Draw(const Shader& shader) const;

    [[GLM_NODISCARD]] const std::vector<Mesh>& GetMeshes() const;
};


