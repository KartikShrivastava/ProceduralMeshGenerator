#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "Shader.h"

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh {
private:
	void SetupMesh();
public:
	unsigned int VBO, EBO;
	std::vector<Vertex> vertices;
	std::vector<Texture> textures;
	std::vector<unsigned int> indices;
	unsigned int VAO;

	Mesh(const std::vector<Vertex>& v,const std::vector<Texture>& t,const std::vector<unsigned int>& i);
	void Draw(const Shader& shader, const GLenum& primitiveType);
	void Draw(const GLenum& primitiveType);
	void Draw(Shader& shader, glm::mat4 projection, glm::mat4 view, glm::mat4 model, const GLenum& primitiveType, unsigned int primID);
};