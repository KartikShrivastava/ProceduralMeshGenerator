#include <glad/glad.h>
#include <iostream>

#include "TrianglePicking.h"

TrianglePicking::TrianglePicking() {
	fbo = 0;
	texturePicking = 0;
	textureDepth = 0;
}

TrianglePicking::~TrianglePicking(){
	if (fbo) {
		glDeleteFramebuffers(1, &fbo);
	}
	if (texturePicking) {
		glDeleteTextures(1, &texturePicking);
	}
	if (textureDepth) {
		glDeleteTextures(1, &textureDepth);
	}
}

bool TrianglePicking::Init(unsigned int width, unsigned int height) {
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenTextures(1, &texturePicking);
	glBindTexture(GL_TEXTURE_2D, texturePicking);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texturePicking, 0);

	glGenTextures(1, &textureDepth);
	glBindTexture(GL_TEXTURE_2D, textureDepth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_COMPONENT, GL_TEXTURE_2D, textureDepth, 0);

	glReadBuffer(GL_NONE);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Framebuffer Error, status: " << status << std::endl;
		return false;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

void TrianglePicking::EnableWriting() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
}

void TrianglePicking::DisableWriting() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}
TrianglePicking::Pixel TrianglePicking::ReadPixel(unsigned int x, unsigned int y) {
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
	glReadBuffer(GL_COLOR_ATTACHMENT0);

	Pixel pixel;
	glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, &pixel);

	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

	return pixel;
}