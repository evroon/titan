#include "uibox.h"

#include "core/contentmanager.h"
#include "graphics/renderer.h"

UIBox::UIBox() {
    mesh_2d = MeshHandler::get_singleton()->get_plane();
    shader = CONTENT->LoadShader("engine/shaders/SimpleShader");
    transform = Transform(vec2(), vec2(100));
    set_color(Color::Blue);
}

UIBox::~UIBox() {}

void UIBox::set_color(const Color &p_color) { color = p_color; }
Color UIBox::get_color() const { return color; }

void UIBox::prepare_draw() {
    shader->bind();
    shader->set_uniform("color", color);
}

void UIBox::draw_mesh() {
    shader->set_uniform("model",
                        RENDERER->get_final_matrix() * transform.get_model());
    mesh_2d->draw();
}
void UIBox::draw() {
    prepare_draw();
    draw_mesh();
}
