/*
Titan Engine
3D Game Engine using OpenGL
Languages: c++, TitanScript, glsl
Started: november 2015
Platforms: Android, Linux, Mac OS X and Windows
*/

#include "game/GameApp.h"
#include "editor/EditorApp.h"
#include "core/Definitions.h"

#undef main

#define DEBUG 1
#define EDITOR 1

#if PLATFORM == LINUX
#define NEW_PLATFORM new Linux
#include "core/platform/Linux.h"
#else
#define NEW_PLATFORM new Windows
#include "core/platform/Windows.h"
#endif

int main(int argc, char* argv[])
{
#if EDITOR
	EditorApp editor(NEW_PLATFORM);
	editor.Loop();
#else
	GameApp game(NEW_PLATFORM);
	game.Loop();
#endif

	return 0;
}