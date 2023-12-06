#pragma once

#include <stdint.h>
#include <GLFW/glfw3.h>

namespace svogen 
{
	class VoxelData
	{
	public:
		GLubyte* data;
		VoxelData();
		GLubyte get(int x, int y, int z);
		void set(int x, int y, int z, GLubyte value);
	};
}