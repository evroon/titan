#pragma once

#include "core/Application.h"
#include "Game.h"

class GameApp : public Application
{
public:
	GameApp(Platform *t) : Application(t) { }

	void init() override;
	void update() override;
	void draw() override;

	Game game;
};