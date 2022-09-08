#pragma once

#include "framedbutton.h"

#define IconButton(X) ImageButton(CONTENT->LoadFontAwesomeIcon(X))
#define IconButtonSized(X, Y) ImageButton(CONTENT->LoadFontAwesomeIcon(X, Y))

class ImageButton : public FramedButton {
    OBJ_DEFINITION(ImageButton, FramedButton);

   public:
    ImageButton(const String& tex_name);
    ImageButton(Texture2D* p_texture);

    void notification(int p_notification) override;

    void set_icon(const String& p_icon);

    void set_image(Texture2D* p_texture);
    Texture2D* get_image() const;

    vec2 get_required_size() const override;

   protected:
    Texture2D* texture;

   private:
    float texture_margins[4];
};
