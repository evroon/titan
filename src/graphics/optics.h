#pragma once

#include "core/contentmanager.h"
#include "fbo.h"
#include "fbomanager.h"
#include "resources/shader.h"
#include "world/light.h"

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
