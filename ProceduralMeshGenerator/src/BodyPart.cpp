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


	for (int i = 0; i < vertexStr.size(); ++i) {
		auto itr = commonNodes.find(vertexStr[i]);
		if (itr != commonNodes.end() && itr->second>2 ) {
			commonNodes.erase(itr);
			if (i % 3 == 0) {
				commonNodesPos.push_back(glm::vec3(*(updatingTriangle[i / 3].vert1Pos.x), *(updatingTriangle[i / 3].vert1Pos.y),
					*(updatingTriangle[i / 3].vert1Pos.z)));
			}
			else if (i % 3 == 1) {
				commonNodesPos.push_back(glm::vec3(*(updatingTriangle[i / 3].vert2Pos.x), *(updatingTriangle[i / 3].vert2Pos.y),
					*(updatingTriangle[i / 3].vert2Pos.z)));
			}
			else if (i % 3 == 2) {
				commonNodesPos.push_back(glm::vec3(*(updatingTriangle[i / 3].vert3Pos.x), *(updatingTriangle[i / 3].vert3Pos.y),
					*(updatingTriangle[i / 3].vert3Pos.z)));
			}
		}
	}
	
	int count = 0;
	for (auto itr : commonNodes) {
		std::cout << itr.first << ":" << itr.second << std::endl;
		if (itr.second > 2)
			count++;
	}
	
	int f = count;

	/*int common1 = -1, common2 = -1, mainCommon = -1;
	for (int i = 0; i < 3; ++i) {
		for (int j = 3; j < 6; j++) {
			if (vertexStr[i] == vertexStr[j]) {
				if (common1 == -1)
					common1 = i;
				else
					common2 = i;
			}
		}
	}

	for (int i = 6; i < 9; ++i) {
		if (vertexStr[i] == vertexStr[common1]) {
			mainCommon = i;
			commonVertex = vertexStr[i];
		}
		if (vertexStr[i] == vertexStr[common2]) {
			mainCommon = i;
			commonVertex = vertexStr[i];
		}
	}

	//Store Common Vertex Index
	commonVertexAddress.clear();

	//store sum of normals and find their average
	float normalx = 0.0f, normaly = 0.0f, normalz = 0.0f;

	for (int i = 0; i < updatingTriangle.size(); ++i) {
		if (commonVertex == vertexStr[i * 3 + 0]) {
			commonVertexAddress.push_back(updatingTriangle[i].vert1Pos);
			normalx += *(updatingTriangle[i].vert1Normal.x);
			normaly += *(updatingTriangle[i].vert1Normal.y);
			normalz += *(updatingTriangle[i].vert1Normal.z);
		}
		else if (commonVertex == vertexStr[i * 3 + 1]) {
			commonVertexAddress.push_back(updatingTriangle[i].vert2Pos);
			normalx += *(updatingTriangle[i].vert2Normal.x);
			normaly += *(updatingTriangle[i].vert2Normal.y);
			normalz += *(updatingTriangle[i].vert2Normal.z);
		}
		else if (commonVertex == vertexStr[i * 3 + 2]) {
			commonVertexAddress.push_back(updatingTriangle[i].vert3Pos);
			normalx += *(updatingTriangle[i].vert3Normal.x);
			normaly += *(updatingTriangle[i].vert3Normal.y);
			normalz += *(updatingTriangle[i].vert3Normal.z);
		}
	}

	initialPos = glm::vec3(*(commonVertexAddress[0].x), *(commonVertexAddress[0].y), *(commonVertexAddress[0].z));

	int sz = commonVertexAddress.size();

	normalDirs.push_back(glm::vec3(normalx / sz, normaly / sz, normalz / sz));*/
}

void BodyPart::Adjust() {

	for (int i = 0; i < commonVertexAddress.size(); ++i) {
		*(commonVertexAddress[i].x) = initialPos.x + adjust * normalDirs[0].x;
		*(commonVertexAddress[i].y) = initialPos.y + adjust * normalDirs[0].y;
		*(commonVertexAddress[i].z) = initialPos.z + adjust * normalDirs[0].z;
	}
}
