#include "noise.h"

#include <thread>

#include "resources/texture.h"

#ifndef BYTE
#define BYTE char
#endif

Noise::Noise() {}

Noise::~Noise() {}

//=========================================================================
// PerlinNoise
//=========================================================================

PerlinNoise::PerlinNoise() {
    perlin_noise.SetOctaveCount(10);
    perlin_noise.SetFrequency(1);
    perlin_noise.SetPersistence(0.5);
}

float PerlinNoise::get_value(const vec3 &p_pos) {
    return to_float(perlin_noise.GetValue(p_pos.x, p_pos.y, p_pos.z));
}

void PerlinNoise::fill_partition(PerlinNoise *p_noise, vec4 *p_heights, int p_x,
                                 int p_y, const vec2i &p_partition_size,
                                 const vec2i &p_size) {
    for (int x = p_x; x < p_x + p_partition_size.x; x++) {
        for (int y = p_y; y < p_y + p_partition_size.y; y++) {
            float value = p_noise->get_value(vec3(x, y, 0.0f) / 2000.0f) / 2.0f;
            p_heights[x + y * p_size.x] = vec4(vec3(value), 1.0f);
        }
    }
}

Texture2D *PerlinNoise::create_2d_texture(const vec2i &p_size) {
    unsigned id;
    vec2 size = vec2(p_size.x, p_size.y);
    int num_threads = 8;

    vec4 *heights = new vec4[p_size.x * p_size.y];

    std::thread threads[num_threads];

    for (int c = 0; c < num_threads; c++) {
        vec2i partition_size = vec2i(p_size.x, p_size.y / num_threads);
        int y = partition_size.y * c;

        threads[c] = std::thread(fill_partition, this, heights, 0, y,
                                 partition_size, p_size);
    }

    for (int c = 0; c < num_threads; c++) {
        threads[c].join();
    }

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, (GLsizei)p_size.x,
                 (GLsizei)p_size.y, 0, GL_RGBA, GL_FLOAT, heights);

    delete[] heights;
    return new Texture2D(size, to_int(id));
}

Texture3D *PerlinNoise::create_3d_texture() {
    unsigned id;
    vec3 size = 128;

    struct ColorByte {
        BYTE r;
        BYTE g;
        BYTE b;
        BYTE a;
    };

    ColorByte *colors =
        new ColorByte[to_int(size.x) * to_int(size.y) * to_int(size.z)];

    WorleyNoise w1 = WorleyNoise(8);
    WorleyNoise w2 = WorleyNoise(16);
    WorleyNoise w3 = WorleyNoise(32);

    int c = 0;

    for (int x = 0; x < size.x; x++) {
        for (int y = 0; y < size.y; y++) {
            for (int z = 0; z < size.z; z++) {
                float value1 = get_value(vec3(x / 30.0f, y / 30.0f, z / 30.0f));
                float value2 = w1.get_value(vec3(x, y, z) / size);
                float value3 = w2.get_value(vec3(x, y, z) / size);
                float value4 = w3.get_value(vec3(x, y, z) / size);

                BYTE r = (BYTE)(((value1 + 2.0f) / 4.0f * 255));
                BYTE g = (BYTE)(value2 * 255);
                BYTE b = (BYTE)(value3 * 255);
                BYTE a = (BYTE)(value4 * 255);

                colors[c] = {r, g, b, a};

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

    glTexImage3D(GL_TEXTURE_3D, 0, (GLsizei)GL_RGBA, size.x, size.y, size.z, 0,
                 (GLsizei)GL_RGBA, GL_UNSIGNED_BYTE, colors);

    return new Texture3D(size, to_int(id));
}

WorleyNoise::WorleyNoise(const vec3 &p_size) {
    size = p_size;

    int c = 0;

    for (int x = 0; x < size.x; x++) {
        for (int y = 0; y < size.y; y++) {
            for (int z = 0; z < size.z; z++) {
                float rand_x = Math::random();
                float rand_y = Math::random();
                float rand_z = Math::random();

                spheres.push_back(vec3(rand_x, rand_y, rand_z));
            }
        }
    }
}

float WorleyNoise::get_value(const vec3 &p_pos) {
    float closest = 1.0f;

    vec3 pos = p_pos * size;

    vec3i cell = vec3i(to_int(pos.x), to_int(pos.y), to_int(pos.z));

    if (cell.x == 0) cell.x = 1;

    if (cell.x > size.x - 2.0f) cell.x = size.x - 2.0f;

    if (cell.y == 0) cell.y = 1;

    if (cell.y > size.y - 2.0f) cell.y = size.y - 2.0f;

    if (cell.z == 0) cell.z = 1;

    if (cell.z > size.z - 2.0f) cell.z = size.z - 2.0f;

    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            for (int z = -1; z <= 1; z++) {
                int c = (z + cell.z) + (y + cell.y) * size.z +
                        (x + cell.x) * size.y * size.z;

                float dist = (spheres[c] +
                              vec3(cell.x + x, cell.y + y, cell.z + z) - pos)
                                 .length();
                closest = MIN(closest, dist);
            }
        }
    }

    return 1.0f - closest;
}
