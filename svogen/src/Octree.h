#pragma once

#include <vector>
#include <GLFW/glfw3.h>

namespace svogen {
	class Octree {
	public:
		std::vector<int> origin;
		int memOffset;
		GLubyte* buffer;
		const int childOffsets[8][3] = {
			{0, 0, 0},
			{1, 0, 0},
			{0, 1, 0},
			{1, 1, 0},
			{0, 0, 1},
			{1, 0, 1},
			{0, 1, 1},
			{1, 1, 1}
		};

		Octree(int memSizeKb, std::vector<int> origin);

		void constructOctree(int size, int curLOD, int maxLOD, std::vector<int> pPos, int pPointer);
	};
}
