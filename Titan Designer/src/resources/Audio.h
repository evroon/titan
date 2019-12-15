#pragma once

#if PLATFORM == LINUX
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#else
#include "SDL.h"
#include "SDL_mixer.h"
#endif

#include "Resource.h"

class Audio
{
public:
	static void init();
	static void Free();
};

class SoundEffect : public Resource
{
	OBJ_DEFINITION(SoundEffect, Resource)

public:
	SoundEffect(const String &filename) { Load(filename); }
	~SoundEffect();

	void Load(const String &filename);
	void Play();

	static void bind_methods();

private:
	Mix_Chunk * effect;
};

class Music : public Resource
{
	OBJ_DEFINITION(Music, Resource)

public:
	Music(const String &filename) { Load(filename); }
	virtual ~Music();

	void Load(const String &filename);
	void Play();
	void Pause();

	static void bind_methods();

private:
	Mix_Music *music;
};