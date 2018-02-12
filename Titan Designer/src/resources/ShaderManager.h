#pragma once

#include "core/ContentManager.h"
#include "Shader.h"

class ShaderManager
{
public:
	static void Init();

	static Shader *Shader2D, *SimpleShader;
};