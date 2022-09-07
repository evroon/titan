#pragma once

#include "core/contentmanager.h"
#include "shader.h"

class ShaderManager
{
public:
	static void Init();

	static Shader *Shader2D, *SimpleShader;
};