#include "GameApp.h"

void GameApp::init()
{
	game.Start();
}

void GameApp::update()
{
	for (Event *e : INPUT->events)
		game.handle_event(e);

	game.update();
}

void GameApp::draw()
{
	game.Draw();
}