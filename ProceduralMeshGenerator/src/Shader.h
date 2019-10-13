#pragma once

#include <unordered_map>
#include <glad/glad.h>

class Shader {
private:
	unsigned int m_RendererID;
	std::unordered_map<const char*, int> m_UniformLocations;
public:
	Shader(const char* vertexFilePath, const char* fragFilePath);

	inline const unsigned int& GetID() const { return m_RendererID; }
	unsigned int CompileShader(const char* filePath, GLenum type);
	std::string GetShaderCode(const char* path);
	bool CheckShaderErrors(const unsigned int& id, PFNGLGETSHADERIVPROC GetShaderiv, PFNGLGETSHADERINFOLOGPROC GetShaderInfoLog, 
							GLenum query, GLenum type, const char* filePath);
	int GetUniformLocation(const char* name);
	void SetUniform1i(const char* uniformName, unsigned int v);
	void SetUniform1f(const char* uniformName, float v);
	void SetUniform3f(const char* uniformName, float v1, float v2, float v3);
	void SetUniform3fv(const char* uniformName, float* v);
	void SetUniform4f(const char* uniformName, float v1, float v2, float v3, float v4);
	void SetUniform4fv(const char* uniformName, float* v);
	void SetUniformMat4fv(const char* uniformName, float* v);

	void Bind() const;
	void UnBind() const;
};