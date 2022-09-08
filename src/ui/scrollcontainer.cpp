#include "scrollcontainer.h"

#include "graphics/renderer.h"
#include "textbox.h"

ScrollContainer::ScrollContainer() { use_vert_slider(true); }

ScrollContainer::~ScrollContainer() {}

// void ScrollContainer::init(const rect2 &p_render_area)
//{
//	Container::init(p_render_area);
//}

void ScrollContainer::use_vert_slider(bool value) {
    if (value) {
        vert_slider = new Slider;
    } else
        vert_slider = NULL;
}

// void ScrollContainer::resize(const rect2 &p_render_area)
//{
//	area = p_render_area;
//
//	if (vert_slider)
//	{
//		work_area = area.crop(0, 14, 0, 0);
//		vert_slider_area = area.align_full_right(7);
//
//		vert_slider->init(vert_slider_area);
//	}
//
//	Container::resize(work_area);
//
//	set_position(vec2());
//	visible_area = area;
//}

void ScrollContainer::draw() {
    if (vert_slider) vert_slider->draw();

    if (use_scissor) RENDERER->use_scissor(area);

    /*for (Control *c : children)
    {
            if (c->area.is_overlapping(visible_area))
                    c->draw();
    }*/

    if (use_scissor) RENDERER->stop_scissor();
}

// Control* ScrollContainer::raycast(const vec2 &pos) const
//{
//	if (work_area.is_in_box(pos))
//		return Container::raycast(pos);
//	else if (vert_slider_area.is_in_box(pos))
//		return vert_slider;
//	else
//		return nullptr;
//}

void ScrollContainer::set_position(const vec2& p_position) {
    position = p_position;
    // vec2 o = (area - vec2(work_area.size.x, work_area.size.y) * 4.0) *
    // position / 2.0;

    vec2 prev_offset = offset;

    // offset = vec2(o.x, o.y);

    // vec2 translation = offset - prev_offset;

    /*for (Control *c : children)
    {
            TextLine *tl = dynamic_cast<TextLine*>(c);

            if (tl)
                    tl->translate(translation);
            else
                    c->resize(rect2(c->area.pos + translation, c->area.size));
    }*/
}

void ScrollContainer::set_use_scissor(bool value) { use_scissor = value; }
