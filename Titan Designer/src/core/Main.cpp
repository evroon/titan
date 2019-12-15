/*
Titan Engine
3D Game Engine using OpenGL
Languages: c++, TitanScript, glsl
Started: november 2015
Platforms: Android, Linux, Mac OS X and Windows
*/

#include "game/GameApp.h"
#include "editor/EditorApp.h"
#include "core/platform/Windows.h"

#undef main

#define PLATFORM LINUX

#define DEBUG 1
#define EDITOR 1

int main(int argc, char* argv[])
{
#if EDITOR
	EditorApp editor(new Windows);
	editor.Loop();
#else
	GameApp game(new Windows);
	game.Loop();
#endif

	return 0;
}