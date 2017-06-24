#include "ParticleSystem.h"

ParticleSystem::ParticleSystem()
{
	Texture2D * tex = CONTENT->LoadTexture("earthlogo.png");
	sprite = new Sprite(tex);
	particles.push_back(Particle(vec2()));

	//func = new Polynomial<double>();
	//((Polynomial<double>*) func)->AddDegree(2.0);
	//((Polynomial<double>*) func)->AddDegree(8.0);
}


ParticleSystem::~ParticleSystem()
{
	delete sprite;
}

void ParticleSystem::update()
{
	//for (int c = 0; c < particles.size(); c++)
	//	particles[c].velocity = func->GetValue(particles[c].lifetime);
}

void ParticleSystem::draw()
{
	//for (int c = 0; c < particles.size(); c++)
	//	sprite->Draw(particles[c].pos);
}