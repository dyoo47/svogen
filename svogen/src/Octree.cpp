#include "Octree.h"
#include <stdint.h>
#include <cstdlib>
#include <iostream>

using namespace svogen;

Octree::Octree(int memSizeMb)
{
	buffer = (GLubyte*)malloc(sizeof(GLubyte) * 1024 * 1024 * memSizeMb);
	memOffset = 0;
}

void Octree::constructOctree(int size, int curLOD, int maxLOD, int* pPos, int pPointer, VoxelData* voxelData) {
	int cSize = size / 2;
	if (cSize == 0 || curLOD == maxLOD) return;
	
	int* children = (int*)malloc(sizeof(int) * 8);
	int cPos[8][3];
	uint8_t leafMask = 0;
	for (int n = 0; n < 8; n++) {
		cPos[n][0] = pPos[0] + childOffsets[n][0] * cSize;
		cPos[n][1] = pPos[1] + childOffsets[n][1] * cSize;
		cPos[n][2] = pPos[2] + childOffsets[n][2] * cSize;
		GLubyte first = voxelData->get(cPos[n][0], cPos[n][1], cPos[n][2]);
		GLubyte value = first;
		bool leaf = true;
		if (curLOD + 1 != maxLOD) {
			for (int i = 0; i < cPos[n][0]; i++) {
				for (int j = 0; j < cPos[n][1]; j++) {
					for (int k = 0; k < cPos[n][2]; k++) {
						GLubyte sample = voxelData->get(i, j, k);
						if (sample != 0) value = sample;
						if (sample != first) {
							if (first == 0) first = sample;
							value = first;
							leaf = false;
							break;
						}
					}
					if (!leaf) break;
				}
				if (!leaf) break;
			}
		}

		if (leaf) {
			//calculate normal here
			GLshort packed = 0;
			children[n] = createLeafNode(value, packed);
		}
		else children[n] = createNode(value);
		if (leaf) leafMask |= (0x01 << n);
	}
	setChildPointer(pPointer, children[0]);
	setLeafMask(pPointer, leafMask);
	for (int n = 0; n < 8; n++) {
		if (getValue(children[n]) != 0 && (leafMask & (0x01 << n)) == 0) {
			constructOctree(cSize, curLOD + 1, maxLOD, cPos[n], children[n], voxelData);
		}
	}
	free(children);
}

int Octree::createLeafNode(GLubyte value, GLshort normal)
{
	int pointer = memOffset;
	buffer[memOffset++] = value;
	buffer[memOffset++] = (GLubyte) normal;
	buffer[memOffset++] = (GLubyte)(normal >> 8);
	return pointer;
}

int Octree::createNode(GLubyte value) {
	int pointer = memOffset;
	buffer[memOffset++] = value;
	buffer[memOffset++] = 0;
	buffer[memOffset++] = 0;
	buffer[memOffset++] = 0;
	buffer[memOffset++] = 0;
	buffer[memOffset++] = 0;
	return pointer;
}

void Octree::setChildPointer(int parentPointer, int childPointer) {
	memcpy(buffer + parentPointer, &childPointer, 4);
}

void Octree::setLeafMask(int parentPointer, GLubyte leafMask) {
	buffer[parentPointer + 5] = leafMask;
}

GLubyte Octree::getValue(int parentPointer) {
	return buffer[parentPointer];
}


void Octree::printBuffer(int start, int end) {
	printf("Octree buffer from %d to %d: \n", start, end);
	for (int i = start; i < end; i++) {
		printf("%d: %d\n", i, (int)buffer[i]);
	}
	printf("End buffer log");
}

Octree::~Octree() {
	free(buffer);
}
