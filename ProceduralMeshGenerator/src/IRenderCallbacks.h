#pragma once

#include "Shader.h"

class IRenderCallbacks {
public:
	virtual void DrawStartCB(const Shader& pickingShader, unsigned int drawIndex) = 0;
};