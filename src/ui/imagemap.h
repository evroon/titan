#pragma once

#include "control.h"
#include "image.h"

struct SplitProperties {
    struct TexCoords {
        Array<float> h_off, v_off;
    };

    struct Positions {
        Array<float> horiz_pos, vert_pos;
    };

    TexCoords tex_coords;
    Positions positions;
};

class ImageMap : public Control {
    OBJ_DEFINITION(ImageMap, Control)

   public:
    ImageMap();
    ImageMap(Texture2D *p_texture, SplitProperties::TexCoords p_tex_coords,
             SplitProperties::Positions p_positions);
    ~ImageMap() {}

    void notification(int p_notification) override;

    void build(Texture2D *p_texture, SplitProperties::TexCoords p_tex_coords,
               SplitProperties::Positions p_positions);
    void build();

    void set_texture(Texture2D *p_texture);
    void set_color(const Color &p_color);
    Texture2D *get_texture() const;

   private:
    Vector<Image> images;
    Color color;
    Texture2D *texture;
    SplitProperties properties;
};
