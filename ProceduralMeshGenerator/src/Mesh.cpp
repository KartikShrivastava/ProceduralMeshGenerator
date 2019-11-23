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
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

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

void Mesh::Draw(Shader& shader, glm::mat4 projection, glm::mat4 view, glm::mat4 model, const GLenum& primitiveType, unsigned int primID) {		//for drawing picked triangle
	glBindVertexArray(VAO);

	vertices[indices[primID*3]].position = glm::vec3(0.0f, -5.0f, 5.0f);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_DYNAMIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDrawElements(primitiveType, 3, GL_UNSIGNED_INT, (const void*)(primID * 3 * sizeof(unsigned int)));

	glBindVertexArray(0);
}