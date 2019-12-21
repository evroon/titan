#include "Noise.h"

#include "resources/Texture.h"

#ifndef BYTE
#define BYTE char
#endif

Noise::Noise()
{
}


Noise::~Noise()
{
}

//=========================================================================
//PerlinNoise
//=========================================================================

PerlinNoise::PerlinNoise()
{
	
}

float PerlinNoise::get_value(const vec3& p_pos)
{
	return to_float(perlin_noise.GetValue(p_pos.x, p_pos.y, p_pos.z));
}

Texture2D* PerlinNoise::create_2d_texture()
{
	unsigned id;
	vec2i size = 512;

	struct ColorByte
	{
		BYTE r;
		BYTE g;
		BYTE b;
	};

	ColorByte* colors = new ColorByte[size.x * size.y];

	for (int c = 0; c < size.x * size.y; c++)
	{
		int x = c / size.x;
		int y = c % size.y;

		float value = get_value(vec3(x / 100.0f, y / 100.0f, 0));

		colors[c] = { (BYTE) ((value + 2.0f) / 4.0f * 255), 0, 0 };
	}

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, (GLsizei) GL_RGB, size.x, size.y, 0, (GLsizei) GL_RGB, GL_UNSIGNED_BYTE, colors);

	return new Texture2D(size, to_int(id));
}

Texture3D* PerlinNoise::create_3d_texture()
{
	unsigned id;
	vec3 size = 128;

	struct ColorByte
	{
		BYTE r;
		BYTE g;
		BYTE b;
		BYTE a;
	};

	ColorByte* colors = new ColorByte[to_int(size.x) * to_int(size.y) * to_int(size.z)];

	WorleyNoise w1 = WorleyNoise(8);
	WorleyNoise w2 = WorleyNoise(16);
	WorleyNoise w3 = WorleyNoise(32);

	int c = 0;

	for (int x = 0; x < size.x; x++)
	{
		for (int y = 0; y < size.y; y++)
		{
			for (int z = 0; z < size.z; z++)
			{
				float value1 = get_value(vec3(x / 30.0f, y / 30.0f, z / 30.0f));
				float value2 = w1.get_value(vec3(x, y, z) / size);
				float value3 = w2.get_value(vec3(x, y, z) / size);
				float value4 = w3.get_value(vec3(x, y, z) / size);

				BYTE r = (BYTE)(((value1 + 2.0f) / 4.0f * 255));
				BYTE g = (BYTE)(value2 * 255);
				BYTE b = (BYTE)(value3 * 255);
				BYTE a = (BYTE)(value4 * 255);

				colors[c] = { r, g, b, a };

				c++;
			}
		}
	}

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_3D, id);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glTexImage3D(GL_TEXTURE_3D, 0, (GLsizei)GL_RGBA, size.x, size.y, size.z, 0, (GLsizei)GL_RGBA, GL_UNSIGNED_BYTE, colors);

	return new Texture3D(size, to_int(id));

}

WorleyNoise::WorleyNoise(const vec3& p_size)
{
	size = p_size;

	int c = 0;

	for (int x = 0; x < size.x; x++)
	{
		for (int y = 0; y < size.y; y++)
		{
			for (int z = 0; z < size.z; z++)
			{
				float rand_x = Math::random();
				float rand_y = Math::random();
				float rand_z = Math::random();

				spheres.push_back(vec3(rand_x, rand_y, rand_z));
			}
		}
	}
}

float WorleyNoise::get_value(const vec3& p_pos)
{
	float closest = 1.0f;

	vec3 pos = p_pos * size;

	vec3i cell = vec3i(to_int(pos.x), to_int(pos.y), to_int(pos.z));

	if (cell.x == 0)
		cell.x = 1;

	if (cell.x > size.x - 2.0f)
		cell.x = size.x - 2.0f;

	if (cell.y == 0)
		cell.y = 1;

	if (cell.y > size.y - 2.0f)
		cell.y = size.y - 2.0f;

	if (cell.z == 0)
		cell.z = 1;

	if (cell.z > size.z - 2.0f)
		cell.z = size.z - 2.0f;

	for (int x = -1; x <= 1; x++)
	{
		for (int y = -1; y <= 1; y++)
		{
			for (int z = -1; z <= 1; z++)
			{
				int c = (z + cell.z) + (y + cell.y) * size.z + (x + cell.x) * size.y * size.z;
				
				float dist = (spheres[c] + vec3(cell.x + x, cell.y + y, cell.z + z) - pos).length();
				closest = MIN(closest, dist);
			}
		}
	}

	return 1.0f - closest;
}
