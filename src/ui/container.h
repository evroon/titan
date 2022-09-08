#pragma once

#include "control.h"
#include "frame.h"

class Container : public Control {
   public:
    Container();
    ~Container();

    void set_glue_vert(bool p_glue_vert);

    virtual void position_childs();

    vec2 get_required_size() const override;

    void notification(int p_notification) override;

    virtual void add_child(Control* p_child);
    void remove_child(Control* p_child);

    int get_child_count() const;

    void clean();

   private:
    bool is_vert;

    float left_margin;
    float right_margin;
    float top_margin;
    float bottom_margin;

    float space = 0;
};

class FramedContainer : public Container {
   public:
    FramedContainer() {
        add_child(&frame);
        frame.set_color(Color::FromRGB(vec3i(82)));
    }

    void draw() {
        frame.draw();
        Container::draw();
    }

   protected:
    Frame frame;
};
