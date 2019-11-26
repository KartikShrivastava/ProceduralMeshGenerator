#include <iostream>
#include <iomanip>
#include <sstream>
#include <glm/glm.hpp>
#include <unordered_map>

#include "BodyPart.h"

void BodyPart::FindCommonVertex(){
	if (updatingTriangle.size() == 0)
		return;

	std::vector<std::string> vertexStr;

	//atleat 3 faces should be compulsarily selected
	for (int i = 0; i < updatingTriangle.size(); ++i) {
		std::stringstream ss;

		ss << std::fixed << std::setprecision(3) << *(updatingTriangle[i].vert1Pos.x);
		ss << std::fixed << std::setprecision(3) << *(updatingTriangle[i].vert1Pos.y);
		ss << std::fixed << std::setprecision(3) << *(updatingTriangle[i].vert1Pos.z);

		vertexStr.push_back(ss.str());
		ss.str(std::string());

		ss << std::fixed << std::setprecision(3) << *(updatingTriangle[i].vert2Pos.x);
		ss << std::fixed << std::setprecision(3) << *(updatingTriangle[i].vert2Pos.y);
		ss << std::fixed << std::setprecision(3) << *(updatingTriangle[i].vert2Pos.z);

		vertexStr.push_back(ss.str());
		ss.str(std::string());

		ss << std::fixed << std::setprecision(3) << *(updatingTriangle[i].vert3Pos.x);
		ss << std::fixed << std::setprecision(3) << *(updatingTriangle[i].vert3Pos.y);
		ss << std::fixed << std::setprecision(3) << *(updatingTriangle[i].vert3Pos.z);

		vertexStr.push_back(ss.str());
	}

	if (vertexStr.size() < 6) {
		__debugbreak();
		return;
	}

	std::unordered_map<std::string, int> commonNodes;
	
	for (int i = 0; i < vertexStr.size() + 1 - 6; i += 3) {
		for (int j = i; j < i + 3; ++j) {
			if (vertexStr[j] == vertexStr[i+3]) {
				if (commonNodes.find(vertexStr[i]) == commonNodes.end()) {
					commonNodes[vertexStr[i]] = 2;
				}
				else {
					commonNodes[vertexStr[i]] += 1;
				}
			}
			else if (vertexStr[j] == vertexStr[i+4]) {
				if (commonNodes.find(vertexStr[i]) == commonNodes.end()) {
					commonNodes[vertexStr[i]] = 2;
				}
				else {
					commonNodes[vertexStr[i]] += 1;
				}
			}
			else if (vertexStr[j] == vertexStr[i+5]) {
				if (commonNodes.find(vertexStr[i]) == commonNodes.end()) {
					commonNodes[vertexStr[i]] = 2;
				}
				else {
					commonNodes[vertexStr[i]] += 1;
				}
			}
		}
	}
	

	float normalx = 0.0f, normaly = 0.0f, normalz = 0.0f;
	std::unordered_map<std::string, glm::vec3> normalMap;
	
	int count = 0;
	int pickThreshold = 3;

	for (auto itr : commonNodes) {
		if (itr.second > pickThreshold) {
			count++;
			normalMap[itr.first] = glm::vec3(0.0f, 0.0f, 0.0f);
		}
	}

	for (int i = 0; i < vertexStr.size(); ++i) {
		if (commonNodes.find(vertexStr[i]) != commonNodes.end()) {
			if (i % 3 == 0) {
				normalMap[vertexStr[i]] += glm::vec3(*(updatingTriangle[i / 3].vert1Normal.x), *(updatingTriangle[i / 3].vert1Normal.y),
					*(updatingTriangle[i / 3].vert1Normal.z));
			}
			else if (i % 3 == 1) {
				normalMap[vertexStr[i]] += glm::vec3(*(updatingTriangle[i / 3].vert2Normal.x), *(updatingTriangle[i / 3].vert2Normal.y),
					*(updatingTriangle[i / 3].vert2Normal.z));
			}
			else if (i % 3 == 2) {
				normalMap[vertexStr[i]] += glm::vec3(*(updatingTriangle[i / 3].vert3Normal.x), *(updatingTriangle[i / 3].vert3Normal.y),
					*(updatingTriangle[i / 3].vert3Normal.z));
			}
		}
	}

	for (int i = 0; i < vertexStr.size(); ++i) {
		auto itr = commonNodes.find(vertexStr[i]);
		if (itr != commonNodes.end() && itr->second> pickThreshold) {
			//commonNodes.erase(itr);
			if (i % 3 == 0) {
				commonVertexPosAdd.push_back(updatingTriangle[i / 3].vert1Pos);
				commonVertInitialPos.push_back(glm::vec3(*(updatingTriangle[i / 3].vert1Pos.x), *(updatingTriangle[i / 3].vert1Pos.y),
					*(updatingTriangle[i / 3].vert1Pos.z)));
				commonVertNormals.push_back(glm::vec3(normalMap[vertexStr[i]].x / count, normalMap[vertexStr[i]].y / count,
					normalMap[vertexStr[i]].z / count));
			}
			else if (i % 3 == 1) {
				commonVertexPosAdd.push_back(updatingTriangle[i / 3].vert2Pos);
				commonVertInitialPos.push_back(glm::vec3(*(updatingTriangle[i / 3].vert2Pos.x), *(updatingTriangle[i / 3].vert2Pos.y),
					*(updatingTriangle[i / 3].vert2Pos.z)));
				commonVertNormals.push_back(glm::vec3(normalMap[vertexStr[i]].x / count, normalMap[vertexStr[i]].y / count,
					normalMap[vertexStr[i]].z / count));
			}
			else if (i % 3 == 2) {
				commonVertexPosAdd.push_back(updatingTriangle[i / 3].vert3Pos);
				commonVertInitialPos.push_back(glm::vec3(*(updatingTriangle[i / 3].vert3Pos.x), *(updatingTriangle[i / 3].vert3Pos.y),
					*(updatingTriangle[i / 3].vert3Pos.z)));
				commonVertNormals.push_back(glm::vec3(normalMap[vertexStr[i]].x / count, normalMap[vertexStr[i]].y / count,
					normalMap[vertexStr[i]].z / count));
			}
		}
	}
}

void BodyPart::Adjust() {

	for (int i = 0; i < commonVertexPosAdd.size(); ++i) {
		*(commonVertexPosAdd[i].x) = commonVertInitialPos[i].x + adjust * commonVertNormals[i].x;
		*(commonVertexPosAdd[i].y) = commonVertInitialPos[i].y + adjust * commonVertNormals[i].y;
		*(commonVertexPosAdd[i].z) = commonVertInitialPos[i].z + adjust * commonVertNormals[i].z;
	}
}

void BodyPart::Clear() {
	updatingTriangle.clear();
	commonVertexPosAdd.clear();
	commonVertInitialPos.clear();
	commonVertNormals.clear();
}