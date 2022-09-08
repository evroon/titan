#include "cursor.h"

Cursor *CursorManager::active_cursor;

// CursorManager
void CursorManager::set_active_cursor(Cursor *p_cursor) {
    active_cursor = p_cursor;
    SDL_SetCursor(active_cursor->get_SDL_cursor());
}

Cursor::Cursor() {}

Cursor::~Cursor() {}

void Cursor::set_default() { set_cursor(ARROW); }

void Cursor::set_cursor(SystemCursorType p_type) {
    type = p_type;

    switch (type) {
        case ARROW:
            cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
            break;

        case IBEAM:
            cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
            break;

        case WAIT:
            cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_WAIT);
            break;

        case CROSSHAIR:
            cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR);
            break;

        case WAITARROW:
            cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_WAITARROW);
            break;

        case NO:
            cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NO);
            break;

        case HAND:
            cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
            break;
    }
}

void Cursor::activate() { CursorManager::set_active_cursor(this); }

SDL_Cursor *Cursor::get_SDL_cursor() const { return cursor; }
