#include <iostream>
#include <glad/glad.h>

#include "TextureStbImage.h"
#include "CheckGLErrors.h"
#include "expat/stb_image/stb_image.h"

TextureStbImage::TextureStbImage(const char * texturePath, bool flipVertically){
	GLCall(glGenTextures(1, &m_rendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererID));

	stbi_set_flip_vertically_on_load(flipVertically);
	int width, height, nChannels;
	unsigned char* image = stbi_load(texturePath, &width, &height, &nChannels, 0);
	GLenum format;
	if (nChannels == 1)
		format = GL_RED;
	else if (nChannels == 3)
		format = GL_RGB;
	else if (nChannels == 4)
		format = GL_RGBA;
	if (image) {
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
		//float bv[] = { 0.2f, 1.0f, 0.5f, 1.0f };
		//GLCall(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, bv));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));
	}
	else {
		std::cout << "Unable to load texture man!" << std::endl;
	}
	stbi_image_free(image);
}

void TextureStbImage::SetTexParameteri(unsigned int target, unsigned int pname, int param){
	GLCall(glTexParameteri(target, pname, param));
}

void TextureStbImage::SetTexParameterfv(unsigned int target, unsigned int pname, const float * params){
	GLCall(glTexParameterfv(target, pname, params));
}

void TextureStbImage::Bind() const{
	GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererID));
}

void TextureStbImage::Bind(int textureUnit){
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, m_rendererID);
}

void TextureStbImage::UnBind() const{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
