#include "VoxelData.h"
#include <stdint.h>

using namespace svogen;

class VoxelData
{
public:
	uint8_t* data;

	VoxelData() {
		data = new uint8_t[1024 * 1024 * 1024];
	}

	uint8_t get(int x, int y, int z)
	{
		return data[x + y * 1024 + z * 1048576];
	}
	void set(int x, int y, int z, uint8_t value)
	{
		data[x + y * 1024 + z * 1048576] = value;
	}
};