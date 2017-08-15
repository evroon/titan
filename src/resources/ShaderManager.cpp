#include "ShaderManager.h"

Shader *ShaderManager::Shader2D, *ShaderManager::SimpleShader;

void ShaderManager::Init()
{
	Shader2D = CONTENT->LoadShader("EngineCore/Shaders/Shader2D");
}