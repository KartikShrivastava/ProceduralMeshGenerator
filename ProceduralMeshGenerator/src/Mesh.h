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
	unsigned int VBO, EBO;
	void SetupMesh();
public:
	std::vector<Vertex> vertices;
	std::vector<Texture> textures;
	std::vector<unsigned int> indices;
	unsigned int VAO;

	Mesh(std::vector<Vertex> v, std::vector<Texture> t, std::vector<unsigned int> i);
	void Draw(Shader shader);
};