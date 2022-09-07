#pragma once

#include "resources/shader.h"
#include "core/contentmanager.h"
#include "world/light.h"
#include "fbo.h"
#include "fbomanager.h"

#if 0

class Optics
{
public:
	static void Init();
	static void DrawLights();
	static void DrawShadows(IndexedMap<WorldObject> * objects);
	static Light * AddLight(vec2 pos, double radius);
	static void GoDark();

	static Shader *lightshader, *shadowshader;

	static Vector<Light> lights;

	static bool lightingenabled;
};
#endif