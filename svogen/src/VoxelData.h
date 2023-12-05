#pragma once

#include <stdint.h>

namespace svogen 
{
	class VoxelData
	{
	public:
		VoxelData();
		uint8_t get(int x, int y, int z);
		void set(int x, int y, int z, uint8_t value);
	};
}