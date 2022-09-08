#pragma once

#include "core/object.h"
#include "core/platform/platform.h"

class Window;
struct RenderTarget;

class Application : public Object {
    OBJ_DEFINITION(Application, Object);

   public:
    Application(Platform* p_platform);

    virtual void init() {}
    virtual void update() {}
    virtual void draw() {}
    virtual void resize(const vec2i& p_size) {}

    void prepare_draw();
    void FinishDraw();

    void Loop();
    void InitSDL();
    void InitGL();
    void InitPhysics();
    void InitRenderer();
    void InitEngine();
    void Quit();
    void Free();

    Window* window;
    RenderTarget* default_target;
    Platform* platform;
    bool graphics_enabled;
};
