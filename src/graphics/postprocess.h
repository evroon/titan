#pragma once

#include "core/contentmanager.h"
#include "fbo.h"
#include "fbomanager.h"
#include "resources/shader.h"
#include "world/primitives.h"

#define VISUALEFFECT PostProcess::get_singleton()

class PostProcess : public Object {
    OBJ_DEFINITION(PostProcess, Object);

   public:
    PostProcess();
    PostProcess(Shader *p_shader);
    virtual ~PostProcess();

    void set_area(const rect2 &p_area);
    rect2 get_area() const;

    void set_fbo(FBO2D *p_fbo);
    FBO2D *get_fbo() const;

    virtual void post_process();

   protected:
    Shader *shader;
    FBO2D *fbo;

    rect2 area;
};

class WorldPostProcess : public PostProcess {
    OBJ_DEFINITION(WorldPostProcess, PostProcess);

   public:
    WorldPostProcess();
    virtual ~WorldPostProcess();

    void post_process() override;
};

class BlurPostProcess : public PostProcess {
    OBJ_DEFINITION(BlurPostProcess, PostProcess);

   public:
    BlurPostProcess();
    virtual ~BlurPostProcess();

    void post_process() override;

   private:
    Shader *blurshader;
    FBO2D *blurbuffer;
};
