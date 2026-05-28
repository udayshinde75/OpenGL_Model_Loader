#include <graphics/Model.hpp>
#include <graphics/Shader.hpp>
#include <graphics/Texture.hpp>

#include <iostream>

// Constructor
Model::Model(const std::string& path)
{
    LoadModel(path);
}

// Private helpers
void Model::LoadModel(const std::string& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile (
        path,
        aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals
    );

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "ASSIMP ERROR: " << importer.GetErrorString() << std::endl;

        return;
    }

    directory = path.substr(0, path.find_last_of('/'));

    ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    // Process Node Meshes
    for (unsigned int i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(ProcessMesh(mesh, scene));
    }

    // Process Children
    for (unsigned int i = 0; i < node->mNumChildren; ++i)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    // Vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
    {
        Vertex vertex;

        // Position
        vertex.Position.x = mesh->mVertices[i].x;
        vertex.Position.y = mesh->mVertices[i].y;
        vertex.Position.z = mesh->mVertices[i].z;

        // Normal;
        vertex.Normal.x = mesh->mNormals[i].x;
        vertex.Normal.y = mesh->mNormals[i].y;
        vertex.Normal.z = mesh->mNormals[i].z;

        // Tex Coords
        if (mesh->mTextureCoords[0])
        {
            vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
            vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
        }
        else
        {
            vertex.TexCoords = glm::vec2(0.0f);
        }

        vertices.push_back(vertex);
    }

    //Indices
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j)
        {
            indices.push_back(face.mIndices[j]);
        }
    }
    
    // Materials
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<Texture> diffuseMaps = LoadMaterialTextures(
            material,
            aiTextureType_DIFFUSE,
            "texture_diffuse"
        );

        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector<Texture> specularMaps = LoadMaterialTextures(
            material, 
            aiTextureType_SPECULAR,
            "texture_specular"
        );

        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* material, aiTextureType type, const std::string& typeName)
{
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < material->GetTextureCount(type); ++i)
    {
        aiString path;
        material->GetTexture(type, i, &path);

        bool alreadyLoaded = false;

        // Texture cache check
        for (const auto& loadedTexture : loadedTextures)
        {
            if (loadedTexture.Path == path.C_Str())
            {
                textures.push_back(loadedTexture);
                alreadyLoaded = true;
                break;
            }
        }
        
        // Load new texture
        if (!alreadyLoaded)
        {
            Texture texture;
            std::string fullPath = directory + "/" + std::string(path.C_Str());

            texture.ID = TextureFromFile(fullPath);
            texture.Type = typeName;
            texture.Path = path.C_Str();
            textures.push_back(texture);
            loadedTextures.push_back(texture);
        }
    }
    
    return textures;
}

void Model::Draw(const Shader& shader) const
{
    for (const auto& mesh : meshes)
    {
        mesh.Draw(shader);
    }
}

const std::vector<Mesh>& Model::GetMeshes() const
{
    return meshes;
}

