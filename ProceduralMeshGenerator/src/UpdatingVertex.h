#pragma once

namespace PMG {
	struct Vector3 {
		float* x;
		float* y;
		float* z;
	};
}

class UpdatingVertex {
public:
	PMG::Vector3 vert1Pos;
	PMG::Vector3 vert2Pos;
	PMG::Vector3 vert3Pos;

	PMG::Vector3 vert1Normal;
	PMG::Vector3 vert2Normal;
	PMG::Vector3 vert3Normal;
};