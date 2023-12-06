#include "VoxelData.h"
#include <stdint.h>

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