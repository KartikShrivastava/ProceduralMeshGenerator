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
	PMG::Vector3 v1;
	PMG::Vector3 v2;
	PMG::Vector3 v3;
};