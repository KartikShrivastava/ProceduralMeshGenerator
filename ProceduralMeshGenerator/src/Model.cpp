#include <iostream>

#include "expat/stb_image/stb_image.h"
#include "Model.h"
#include "TextureStbImage.h"

Model::Model(const std::string& path){
	LoadModel(path);
}

void Model::LoadModel(const std::string& path){
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
	}
	directory = path.substr(0, path.find_last_of('/'));
	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode * node, const aiScene * scene){
	for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; ++i) {
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh * mesh, const aiScene * scene) {
	std::vector<Vertex> vertices;
	std::vector<Texture> textures;
	std::vector<unsigned int> indices;

	for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
		Vertex vertex;
		glm::vec3 vec;
		vec.x = mesh->mVertices[i].x;
		vec.y = mesh->mVertices[i].y;
		vec.z = mesh->mVertices[i].z;
		vertex.position = vec;

		vec.x = mesh->mNormals[i].x;
		vec.y = mesh->mNormals[i].y;
		vec.z = mesh->mNormals[i].z;
		vertex.normal = vec;

		if (mesh->mTextureCoords[0]) {
			glm::vec2 v;
			v.x = mesh->mTextureCoords[0][i].x;
			v.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = v;
		}
		else
			vertex.texCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}

	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuse = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuse.begin(), diffuse.end());

		std::vector<Texture> specular = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specular.begin(), specular.end());
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j)
			indices.push_back(face.mIndices[j]);
	}

	return Mesh(vertices, textures, indices);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName){
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i) {
		aiString path;
		mat->GetTexture(type, i, &path);
		bool found = false;
		for (unsigned int j = 0; j < texture_loaded.size(); ++j) {
			if (std::strcmp(texture_loaded[j].path.data(), path.C_Str()) == 0) {
				textures.push_back(texture_loaded[j]);
				found = true;
				break;
			}
		}
		if (!found) {
			Texture texture;
			texture.id = LoadTextureFromFile(path.C_Str());
			texture.type = typeName;
			texture.path = path.C_Str();
			textures.push_back(texture);
		}
	}
	return textures;
}

unsigned int Model::LoadTextureFromFile(const char* path) {
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);
	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data) {
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else {
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

void Model::Draw(const Shader& renderShader, const GLenum& primitiveType){
	for (unsigned int i = 0; i < meshes.size(); ++i) {
		meshes[i].Draw(renderShader, primitiveType);
	}
}

void Model::Draw(const GLenum& primitiveType, Shader* trianglePickingShader){
	for (unsigned int i = 0; i < meshes.size(); ++i) {

		trianglePickingShader->SetUniform1ui("u_drawIndex", i);

		meshes[i].Draw(primitiveType);
	}
}

void Model::Draw(const GLenum& primitiveType, unsigned int drawID, unsigned int primID, UpdatingVertex& uv) {
	meshes[drawID].Draw(primitiveType, primID,uv);
}
