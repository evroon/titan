#include "particlesystem.h"

//=========================================================================
// ParticleSystem
//=========================================================================

ParticleSystem::ParticleSystem() { ubo = new UBO; }

ParticleSystem::~ParticleSystem() {}

void ParticleSystem::init() {}

void ParticleSystem::start() {}

void ParticleSystem::update() {}

void ParticleSystem::draw() {}

#undef CLASSNAME
#define CLASSNAME ParticleSystem

void ParticleSystem::bind_methods() {}
