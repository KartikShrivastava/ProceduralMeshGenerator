#pragma once

class TextureStbImage {
private:
	unsigned int m_rendererID;
public:
	TextureStbImage(const char* texturePath, bool flipVertically);
	void SetTexParameteri(unsigned int target, unsigned int pname, int param);
	void SetTexParameterfv(unsigned int target, unsigned int pname, const float* params);

	void Bind(unsigned int textureUnit) const;
	void UnBind() const;
};