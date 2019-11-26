#pragma once

#include <vector>
#include <string>

#include "UpdatingVertex.h"

class BodyPart {
public:
	std::vector<UpdatingVertex> updatingTriangle;
	std::vector<PMG::Vector3> commonVertexPosAdd;
	float adjust = 0.0f;
	std::vector<glm::vec3> commonVertInitialPos;
	std::vector<glm::vec3> commonVertNormals;

	void FindCommonVertex();
	void Adjust();
	void Clear();
};