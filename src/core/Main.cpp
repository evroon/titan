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

#define DEBUG 1

int main(int argc, char* argv[])
{
	GameApp game(new Windows);
	game.Loop();

	//EditorApp editor(new Windows);
	//editor.Loop();

	return 0;
} 