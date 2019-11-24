#pragma once

#include <vector>
#include <string>

#include "UpdatingVertex.h"

class BodyPart {
public:
	std::vector<UpdatingVertex> updatingTriangle;
	std::string commonVertex;
	std::vector<PMG::Vector3> commonVertexAddress;
	float adjust = 0.0f;
	glm::vec3 initialPos;
	std::vector<glm::vec3> normalDirs;
	std::vector<glm::vec3> commonNodesPos;

	void FindCommonVertex();
	void Adjust();
};