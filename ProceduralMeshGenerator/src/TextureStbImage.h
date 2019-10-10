#pragma once

class TextureStbImage {
private:
	unsigned int m_rendererID;
public:
	TextureStbImage(const char* texturePath, bool flipVertically);
	void SetTexParameteri(unsigned int target, unsigned int pname, int param);
	void SetTexParameterfv(unsigned int target, unsigned int pname, const float* params);

	void Bind() const;
	void Bind(int textureUnit);
	void UnBind() const;
};