#pragma once

#include <GLFW/glfw3.h>
#include "VoxelData.h"

namespace svogen {
	class Octree {
	public:
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

		Octree(int memSizeKb);
		~Octree();

		void constructOctree(int size, int curLOD, int maxLOD, int* pPos, int pPointer, VoxelData* voxelData);

		int createLeafNode(GLubyte value, GLshort normal);
		int createNode(GLubyte value);
		void setChildPointer(int parentPointer, int childPointer);
		void setLeafMask(int parentPointer, GLubyte leafMask);
		GLubyte getValue(int parentPointer);

		void printBuffer(int start, int end);
	};
}
