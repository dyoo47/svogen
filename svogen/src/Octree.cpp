#include "Octree.h"
#include <stdint.h>

using namespace svogen;

Octree::Octree(int memSizeKb, std::vector<int> origin)
{
	buffer = (GLubyte*)malloc(sizeof(GLubyte) * 1024 * memSizeKb);
	memOffset = 0;
	this->origin = origin;
}

void Octree::constructOctree(int size, int curLOD, int maxLOD, std::vector<int> pPos, int pPointer) {
	int cSize = size / 2;
	if (cSize == 0 || curLOD == maxLOD) return;
	
	int* children = (int*)malloc(sizeof(int) * 8);
	uint8_t leafMask = 0;
	for (int n = 0; n < 8; n++) {
		
	}
	free(children);
}

