#pragma once

#include "IRenderCallbacks.h"
#include "Shader.h"

class TrianglePicking {
private:
	unsigned int fbo;
	unsigned int texturePicking;
	unsigned int textureDepth;
public:
	TrianglePicking();

	~TrianglePicking();

	bool Init(unsigned int width, unsigned int height);

	void EnableWriting();

	void DisableWriting();

	struct Pixel {
		float objID;
		float drawID;
		float primitiveID;

		Pixel() {
			objID = 0.0f;
			drawID = 0.0f;
			primitiveID = 0.0f;
		}
	};

	Pixel ReadPixel(unsigned int x, unsigned int y);

};