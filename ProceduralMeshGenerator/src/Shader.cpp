#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "Shader.h"
#include "CheckGLErrors.h"

Shader::Shader(const char* vertexFilePath, const char* fragFilePath) {

	unsigned int vs = CompileShader(vertexFilePath, GL_VERTEX_SHADER);
	unsigned int fs = CompileShader(fragFilePath, GL_FRAGMENT_SHADER);

	GLCall(m_RendererID = glCreateProgram());
	GLCall(glAttachShader(m_RendererID, vs));
	GLCall(glAttachShader(m_RendererID, fs));
	GLCall(glLinkProgram(m_RendererID));

	CheckShaderErrors(m_RendererID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS, GL_PROGRAM, NULL);
}

unsigned int Shader::CompileShader(const char* filePath, GLenum type) {
	std::string temp = GetShaderCode(filePath);
	const char* shaderCode = temp.c_str();
	GLCall(unsigned int id = glCreateShader(type));
	GLCall(glShaderSource(id, 1, &shaderCode, NULL));
	GLCall(glCompileShader(id));
	if (!CheckShaderErrors(id, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS, type, filePath))
		ASSERT(false);
	return id;
}

std::string Shader::GetShaderCode(const char* path) {
	std::ifstream file;
	std::stringstream ss;
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		file.open(path);
		ss << file.rdbuf();
		file.close();
	}
	catch (std::ifstream::failure e) {
		std::cout << "Error! Shader file " << path << " can't be loaded successfully" << std::endl;
	}
	return std::string(ss.str());
}

bool Shader::CheckShaderErrors(const unsigned int& id, PFNGLGETSHADERIVPROC GetShaderiv, PFNGLGETSHADERINFOLOGPROC GetShaderInfoLog, 
								GLenum query, GLenum type, const char* filePath) {
	int status;
	GLCall(GetShaderiv(id, query, &status));
	if (!status) {
		int len;
		GLCall(GetShaderiv(id, GL_INFO_LOG_LENGTH, &len));
		char message[512];
		GLCall(GetShaderInfoLog(id, 512, &len, message));
		std::cout << "Compilation failed for " << ((type == GL_PROGRAM) ? "Shader Program." :
			((type == GL_VERTEX_SHADER) ? "Vertex shader.\n" + std::string(filePath) : "Fragment shader.\n" + std::string(filePath)))
			<< "\n" << message << std::endl;
		return false;
	}
	return true;
}

void Shader::Bind() const {
	GLCall(glUseProgram(m_RendererID));
}

void Shader::UnBind() const {
	GLCall(glUseProgram(0));
}

int Shader::GetUniformLocation(const char* name) {
	if (m_UniformLocations.find(name) != m_UniformLocations.end())
		return m_UniformLocations[name];

	GLCall(int location = glGetUniformLocation(m_RendererID, name));
	if (location == -1)
		std::cout << "Warning: uniform " << name << " doesn't exist." << std::endl;
	m_UniformLocations[name] = location;
	return location;
}

void Shader::SetUniform1i(const char * uniformName,unsigned int v){
	GLCall(glUniform1i(GetUniformLocation(uniformName), v));
}

void Shader::SetUniform1ui(const char * uniformName, unsigned int v){
	GLCall(glUniform1ui(GetUniformLocation(uniformName), v));
}

void Shader::SetUniform1f(const char* uniformName, float v) {
	GLCall(glUniform1f(GetUniformLocation(uniformName), v));
}

void Shader::SetUniform3f(const char* uniformName, float v1, float v2, float v3) {
	GLCall(glUniform3f(GetUniformLocation(uniformName), v1, v2, v3));
}

void Shader::SetUniform3fv(const char* uniformName, float* v) {
	glUniform3fv(GetUniformLocation(uniformName), 1, v);
}

void Shader::SetUniform4f(const char* uniformName, float v1, float v2, float v3, float v4) {
	GLCall(glUniform4f(GetUniformLocation(uniformName), v1, v2, v3, v4));
}

void Shader::SetUniform4fv(const char* uniformName, float* v) {
	GLCall(glUniform4fv(GetUniformLocation(uniformName), 1, v));
}

void Shader::SetUniformMat4fv(const char* uniformName, float* v) {
	GLCall(glUniformMatrix4fv(GetUniformLocation(uniformName), 1, GL_FALSE, v));
}
