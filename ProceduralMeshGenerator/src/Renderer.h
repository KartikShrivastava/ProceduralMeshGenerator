#pragma once

#include <unordered_set>

struct PixelIDS {
	unsigned int objID;
	unsigned int drawID;
	unsigned int primitiveID;
};

class Renderer {
private:

public:
	int width = 1080;
	int height = 700;
	float mix = 0.2f;
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	double lastX = 300, lastY = 400;
	float initialMouse = true;
	bool mouseRelease = true;
	bool isNotOverAnyWindow = false;
	bool mouseButtonPressed = false;
	std::unordered_set<std::string> primitiveIDStored;
	std::vector<PixelIDS> pixelID;
};