/*
Titan Engine
3D Game Engine using OpenGL
Languages: c++, TitanScript, glsl
Started: november 2015
Platforms: Android, Linux, Mac OS X and Windows
*/

#include "game/GameApp.h"
#include "editor/EditorApp.h"
#include "core/titanscript/ScriptApp.h"
#include "core/Definitions.h"

#if PLATFORM == LINUX
#define NEW_PLATFORM new Linux
#include "core/platform/Linux.h"
#else
#define NEW_PLATFORM new Windows
#include "core/platform/Windows.h"
#endif

#if !TESTING

#undef main

int main(int argc, char* argv[])
{
	Array<String> args = Array<String>();
	
	for (int c = 1; c < argc; c++)
		args.push_back(argv[c]);

#if APP_TYPE == APP_TYPE_EDITOR
	EditorApp editor(NEW_PLATFORM);
	editor.Loop();
#elif APP_TYPE == APP_TYPE_GAME
	GameApp game(NEW_PLATFORM);
	game.Loop();
#else
	ScriptApp scriptapp(NEW_PLATFORM);
	scriptapp.execute(args);
#endif
	return 0;
}

#endif