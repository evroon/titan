#include "vectorfield.h"

//=========================================================================
// Vec2Field
//=========================================================================

Vec2Field::Vec2Field() : Vec2Field(vec2()) {}

Vec2Field::Vec2Field(const vec2& p_variant) : PropertyControl(p_variant) {
    init();
}

Vec2Field::Vec2Field(const Variable& p_variable) : PropertyControl(p_variable) {
    init();
}

void Vec2Field::init() {
    add_child(&x);
    add_child(&y);

    margin[0] = 0;
    margin[1] = 2;
    margin[2] = 0;

    position_items();
    value_changed();

    x.connect("value_entered", this, "value_input");
    y.connect("value_entered", this, "value_input");
}

void Vec2Field::value_changed() {
    vec2 v = get_value();

    x.set_text(String(v.x));
    y.set_text(String(v.y));
}

void Vec2Field::value_input() {
    float v_x = x.get_value();
    float v_y = y.get_value();

    value_set(vec2(v_x, v_y));
    emit_signal("value_entered");
}

vec2 Vec2Field::get_required_size() const {
    vec2 size_x = x.get_required_size();
    vec2 size_y = y.get_required_size();

    return vec2(size_x.x + size_y.x + margin[0] + margin[1] * 2.0f + margin[2],
                size_x.y);
}

void Vec2Field::position_items() {
    float top = area.get_top();
    float bottom = area.get_bottom();
    float left = area.get_left();
    float right = area.get_right();

    float x1 = left + (right - left) * 0.5f;

    x.set_anchors(ANCHOR_BEGIN, ANCHOR_BEGIN, ANCHOR_CENTER, ANCHOR_END);
    x.set_margins(margin[0], 0, -margin[1], 0);

    y.set_anchors(ANCHOR_CENTER, ANCHOR_BEGIN, ANCHOR_END, ANCHOR_END);
    y.set_margins(margin[1], 0, margin[2], 0);
}

#undef CLASSNAME
#define CLASSNAME Vec2Field

void Vec2Field::bind_methods() {
    REG_CSTR_OVRLD_1(vec2);

    REG_METHOD(value_changed);
    REG_METHOD(value_input);

    REG_SIGNAL("value_entered");
}

//=========================================================================
// Vec3Field
//=========================================================================

Vec3Field::Vec3Field() : Vec3Field(vec3()) {}

Vec3Field::Vec3Field(const vec3& p_variant) : PropertyControl(p_variant) {
    init();
}

Vec3Field::Vec3Field(const Variable& p_variable) : PropertyControl(p_variable) {
    init();
}

void Vec3Field::init() {
    add_child(&x);
    add_child(&y);
    add_child(&z);

    margin[0] = 0;
    margin[1] = 2;
    margin[2] = 2;
    margin[3] = 0;

    position_items();
    value_changed();

    x.connect("value_entered", this, "value_input");
    y.connect("value_entered", this, "value_input");
    z.connect("value_entered", this, "value_input");
}

void Vec3Field::value_changed() {
    vec3 v = get_value();

    x.set_value(v.x);
    y.set_value(v.y);
    z.set_value(v.z);
}

void Vec3Field::value_input() {
    float v_x = x.get_value();
    float v_y = y.get_value();
    float v_z = z.get_value();

    value_set(vec3(v_x, v_y, v_z));
    emit_signal("value_entered");
}

vec2 Vec3Field::get_required_size() const {
    vec2 size_x = x.get_required_size();
    vec2 size_y = y.get_required_size();
    vec2 size_z = z.get_required_size();

    return vec2(size_x.x + size_y.x + size_z.x + margin[0] + margin[1] * 2.0f +
                    margin[2] * 2.0f + margin[3],
                size_x.y);
}

void Vec3Field::position_items() {
    x.set_custom_anchors(0.0f, 0.0f, 0.333f, 0.0f);
    y.set_custom_anchors(0.333f, 0.0f, 0.667f, 0.0f);
    z.set_custom_anchors(0.667f, 0.0f, 0.0f, 0.0f);

    x.set_anchors(ANCHOR_BEGIN, ANCHOR_BEGIN, ANCHOR_CUSTOM, ANCHOR_END);
    y.set_anchors(ANCHOR_CUSTOM, ANCHOR_BEGIN, ANCHOR_CUSTOM, ANCHOR_END);
    z.set_anchors(ANCHOR_CUSTOM, ANCHOR_BEGIN, ANCHOR_END, ANCHOR_END);

    x.set_margins(0, 0, -2, 0);
    y.set_margins(2, 0, -2, 0);
    z.set_margins(2, 0, 0, 0);
}

#undef CLASSNAME
#define CLASSNAME Vec3Field

void Vec3Field::bind_methods() {
    REG_CSTR_OVRLD_1(vec3);

    REG_METHOD(value_input);

    REG_SIGNAL("value_entered");
}

//=========================================================================
// Vec4Field
//=========================================================================

Vec4Field::Vec4Field() : Vec4Field(vec4()) {}

Vec4Field::Vec4Field(const vec4& p_variant) : PropertyControl(p_variant) {
    init();
}

Vec4Field::Vec4Field(const Variable& p_variable) : PropertyControl(p_variable) {
    init();
}

void Vec4Field::init() {
    add_child(&x);
    add_child(&y);
    add_child(&z);
    add_child(&w);

    margin[0] = 0;
    margin[1] = 2;
    margin[2] = 2;
    margin[3] = 2;
    margin[4] = 0;

    position_items();
    value_changed();

    x.connect("value_entered", this, "value_input");
    y.connect("value_entered", this, "value_input");
    z.connect("value_entered", this, "value_input");
    w.connect("value_entered", this, "value_input");
}

void Vec4Field::value_changed() {
    vec4 v = get_value();

    x.set_text(String(v.x));
    y.set_text(String(v.y));
    z.set_text(String(v.z));
    w.set_text(String(v.w));
}

void Vec4Field::value_input() {
    float v_x = x.get_value();
    float v_y = y.get_value();
    float v_z = z.get_value();
    float v_w = w.get_value();

    value_set(vec4(v_x, v_y, v_z, v_w));
    emit_signal("value_entered");
}

vec2 Vec4Field::get_required_size() const {
    vec2 size_x = x.get_required_size();
    vec2 size_y = y.get_required_size();
    vec2 size_z = z.get_required_size();
    vec2 size_w = w.get_required_size();

    return vec2(size_x.x + size_y.x + size_z.x + size_w.x + margin[0] +
                    margin[1] * 2.0f + margin[2] * 2.0f + margin[3] * 2.0f +
                    margin[4],
                size_x.y);
}

void Vec4Field::position_items() {
    x.set_custom_anchors(0.0f, 0.0f, 0.25f, 0.0f);
    y.set_custom_anchors(0.25f, 0.0f, 0.5f, 0.0f);
    z.set_custom_anchors(0.5f, 0.0f, 0.75f, 0.0f);
    w.set_custom_anchors(0.75f, 0.0f, 0.0f, 0.0f);

    x.set_anchors(ANCHOR_BEGIN, ANCHOR_BEGIN, ANCHOR_CUSTOM, ANCHOR_END);
    y.set_anchors(ANCHOR_CUSTOM, ANCHOR_BEGIN, ANCHOR_CUSTOM, ANCHOR_END);
    z.set_anchors(ANCHOR_CUSTOM, ANCHOR_BEGIN, ANCHOR_CUSTOM, ANCHOR_END);
    w.set_anchors(ANCHOR_CUSTOM, ANCHOR_BEGIN, ANCHOR_END, ANCHOR_END);

    x.set_margins(0, 0, -2, 0);
    y.set_margins(2, 0, -2, 0);
    z.set_margins(2, 0, -2, 0);
    w.set_margins(2, 0, 0, 0);
}

#undef CLASSNAME
#define CLASSNAME Vec4Field

void Vec4Field::bind_methods() {
    REG_CSTR_OVRLD_1(vec4);

    REG_METHOD(value_input);

    REG_SIGNAL("value_entered");
}

//=========================================================================
// TransformField
//=========================================================================

TransformField::TransformField() : TransformField(Transform()) {}

TransformField::TransformField(const Transform& p_variant)
    : PropertyControl(p_variant) {
    init();
}

TransformField::TransformField(const Variable& p_variable)
    : PropertyControl(p_variable) {
    init();
}

void TransformField::init() {
    margin[0] = 0.0f;
    margin[1] = 4.0f;
    margin[2] = 4.0f;
    margin[3] = 0.0f;

    add_child(&pos);
    add_child(&size);
    add_child(&rotation);

    position_items();
    value_changed();

    pos.connect("value_entered", this, "value_input");
    size.connect("value_entered", this, "value_input");
    rotation.connect("value_entered", this, "value_input");
}

void TransformField::value_changed() {
    Transform v = get_value();

    pos.set_value(v.get_pos());
    size.set_value(v.get_size());
    rotation.set_value(v.get_rotation());
}

void TransformField::value_input() {
    vec3 v_pos = pos.get_value();
    vec3 v_size = size.get_value();
    vec3 v_rotation = rotation.get_value();

    set_value(Transform(v_pos, v_size, v_rotation));
}

vec2 TransformField::get_required_size() const {
    vec2 size_pos = pos.get_required_size();
    vec2 size_size = size.get_required_size();
    vec2 size_rotation = rotation.get_required_size();

    float max_x = MAX(MAX(size_pos.x, size_size.x), size_rotation.x);

    return vec2(max_x, size_pos.y + size_size.y + size_rotation.y + margin[0] +
                           margin[1] + margin[2] + margin[3]);
}

void TransformField::position_items() {
    vec2 size_pos = pos.get_required_size();
    vec2 size_size = size.get_required_size();
    vec2 size_rotation = rotation.get_required_size();

    pos.set_anchors(ANCHOR_BEGIN, ANCHOR_END, ANCHOR_END, ANCHOR_END);
    size.set_anchors(ANCHOR_BEGIN, ANCHOR_END, ANCHOR_END, ANCHOR_END);
    rotation.set_anchors(ANCHOR_BEGIN, ANCHOR_END, ANCHOR_END, ANCHOR_END);

    pos.set_margins(0, size_pos.y + margin[0], 0, margin[0]);
    size.set_margins(0, size_size.y + margin[1] + size_pos.y + margin[0], 0,
                     margin[1] + size_pos.y + margin[0]);
    rotation.set_margins(
        0,
        size_rotation.y + margin[2] + size_size.y + margin[1] + size_pos.y +
            margin[0],
        0, margin[2] + size_size.y + margin[1] + size_pos.y + margin[0]);
}

#undef CLASSNAME
#define CLASSNAME TransformField

void TransformField::bind_methods() {
    REG_METHOD(value_changed);
    REG_METHOD(value_input);
}
