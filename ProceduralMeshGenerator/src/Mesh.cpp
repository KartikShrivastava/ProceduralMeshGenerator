#include <iostream>
#include <glad/glad.h>

#include "CheckGLErrors.h"
#include "Mesh.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Mesh::Mesh(const std::vector<Vertex>& v, const std::vector<Texture>& t, const std::vector<unsigned int>& i) {
	vertices = v;
	textures = t;
	indices = i;

	SetupMesh();
}

void Mesh::SetupMesh() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void Mesh::Draw(const Shader& shader, const GLenum& primitiveType) {
	unsigned int diffuseN = 1;
	unsigned int specularN = 1;
	for (unsigned int i = 0; i < textures.size(); ++i) {
		glActiveTexture(GL_TEXTURE0 + i);
		std::string number;
		std::string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseN++);
		else if (name == "texture_specular")
			number = std::to_string(specularN++);

		//shader.SetUniform1i(("u_material." + name + number).c_str(), i);
		const GLchar* ch = ("u_material." + name + number).c_str();
		glUniform1i(glGetUniformLocation(shader.GetID(), ch), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}

	glBindVertexArray(VAO);

	glDrawElements(primitiveType, indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}

void Mesh::Draw(const GLenum& primitiveType) {	//for picking
	glBindVertexArray(VAO);

	glDrawElements(primitiveType, indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}

void Mesh::Draw(const GLenum& primitiveType, unsigned int primID, UpdatingVertex& uv) {		//for drawing picked triangle
	
	glBindVertexArray(VAO);

	//fill address of updating vertex with right vertices
	uv.vert1Pos.x = &(vertices[indices[primID * 3 + 0]].position.x);
	uv.vert1Pos.y = &(vertices[indices[primID * 3 + 0]].position.y);
	uv.vert1Pos.z = &(vertices[indices[primID * 3 + 0]].position.z);

	uv.vert2Pos.x = &(vertices[indices[primID * 3 + 1]].position.x);
	uv.vert2Pos.y = &(vertices[indices[primID * 3 + 1]].position.y);
	uv.vert2Pos.z = &(vertices[indices[primID * 3 + 1]].position.z);

	uv.vert3Pos.x = &(vertices[indices[primID * 3 + 2]].position.x);
	uv.vert3Pos.y = &(vertices[indices[primID * 3 + 2]].position.y);
	uv.vert3Pos.z = &(vertices[indices[primID * 3 + 2]].position.z);

	uv.vert1Normal.x = &(vertices[indices[primID * 3 + 0]].normal.x);
	uv.vert1Normal.y = &(vertices[indices[primID * 3 + 0]].normal.y);
	uv.vert1Normal.z = &(vertices[indices[primID * 3 + 0]].normal.z);

	uv.vert2Normal.x = &(vertices[indices[primID * 3 + 1]].normal.x);
	uv.vert2Normal.y = &(vertices[indices[primID * 3 + 1]].normal.y);
	uv.vert2Normal.z = &(vertices[indices[primID * 3 + 1]].normal.z);

	uv.vert3Normal.x = &(vertices[indices[primID * 3 + 2]].normal.x);
	uv.vert3Normal.y = &(vertices[indices[primID * 3 + 2]].normal.y);
	uv.vert3Normal.z = &(vertices[indices[primID * 3 + 2]].normal.z);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDrawElements(primitiveType, 3, GL_UNSIGNED_INT, (const void*)(primID * 3 * sizeof(unsigned int)));

	glBindVertexArray(0);
}
