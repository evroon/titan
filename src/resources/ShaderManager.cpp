#include "ShaderManager.h"

Shader *ShaderManager::Shader2D, *ShaderManager::SimpleShader;

void ShaderManager::Init()
{
	Shader2D = CONTENT->LoadShader("engine/shaders/Shader2D");
}