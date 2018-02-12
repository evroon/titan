#include "Game.h"

#include "core/NodeManager.h"

Game *Game::activegame;

void Game::Start()
{
	LoadScene("main");
}

void Game::update()
{
	activescene->update();
}

Scene* Game::LoadScene(const String &name)
{
	Scene *s = SCENEMANAGER->get_scene(name);

	StartScene(s);
	s->Init();

	return s;
}

void Game::StartScene(Scene *s)
{
	activescene = s;
}

void Game::handle_event(Event *e)
{
	activescene->handle_event(e);
}

void Game::Draw()
{
	activescene->Draw();
}

Game* Game::GetGame()
{
	return activegame;
}