#include "audio.h"

// Audio
void Audio::init() {
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
        SDL_Log("SDL MIXER failed to initialize!");
}
void Audio::Free() { Mix_CloseAudio(); }

// SoundEffect
SoundEffect::~SoundEffect() { Mix_FreeChunk(effect); }

void SoundEffect::Load(const String &filename) {
    effect = Mix_LoadWAV(filename.c_str());
}

void SoundEffect::Play() {
    if (Mix_PlayChannel(-1, effect, 0) == -1) T_ERROR("Error playing sound!");
}

#undef CLASSNAME
#define CLASSNAME SoundEffect

void SoundEffect::bind_methods() {
    REG_CSTR_OVRLD_1(String);

    REG_METHOD(Load);
    REG_METHOD(Play);
}

//=========================================================================
// Music
//=========================================================================

Music::~Music() { Mix_FreeMusic(music); }

void Music::Load(const String &filename) {
    music = Mix_LoadMUS(filename.c_str());
}

void Music::Play() {
    if (!Mix_PlayMusic(music, -1)) T_ERROR("Error playing music!");
}

void Music::Pause() { Mix_PauseMusic(); }

#undef CLASSNAME
#define CLASSNAME Music

void Music::bind_methods() {
    REG_CSTR_OVRLD_1(String);

    REG_METHOD(Load);
    REG_METHOD(Play);
    REG_METHOD(Pause);
}
