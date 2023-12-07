#include "VoxelData.h"
#include <stdint.h>
#include <iostream>

using namespace svogen;

VoxelData::VoxelData() {
	data = new GLubyte[1024 * 1024 * 1024];
}

GLubyte VoxelData::get(int x, int y, int z)
{
	return data[x + y * 1024 + z * 1048576];
}
void VoxelData::set(int x, int y, int z, GLubyte value)
{
	data[x + y * 1024 + z * 1048576] = value;
}

void VoxelData::printBuffer(int start, int end) {
	printf("Voxel data buffer from %d to %d: \n", start, end);
	for (int i = start; i < end; i++) {
		printf("%d: %d\n", i, (int)data[i]);
	}
	printf("End buffer log");
}
