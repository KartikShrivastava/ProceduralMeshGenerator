#pragma once

#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.h"
#include "Mesh.h"
#include "UpdatingVertex.h"

class Model {
private:
	std::vector<Texture> texture_loaded;
	std::vector<Mesh> meshes;
	std::string directory;
	void LoadModel(const std::string& path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	unsigned int LoadTextureFromFile(const char* path);

public:
	Model(const std::string& path);
	void Draw(const Shader& renderShader, const GLenum& primitiveType);
	void Draw(const GLenum& primitiveType, Shader* trianglePickingShader);
	void Draw(const GLenum& primitiveType, unsigned int drawID,	unsigned int primID, UpdatingVertex& uv);
};