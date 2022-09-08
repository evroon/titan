#pragma once

#include "propertyview.h"
#include "tab.h"
#include "utility/history.h"

class PropertyTab : public Tab {
    OBJ_DEFINITION(PropertyTab, Tab);

   public:
    PropertyTab(Object *p_object);
    PropertyTab();

    virtual ~PropertyTab();

    vec2 get_required_size() const override;

    void set_property(Object *p_object);
    Object *get_property() const;

    void go_back();
    void go_forward();
    void show_history();

    static void bind_methods();

   private:
    PropertyView *property_view;
    TextField *name_field;
    Container *bar;
    ImageButton *back_button;
    ImageButton *forward_button;
    ImageButton *down_button;

    float buttons_height;
    rect2 bar_area;
    rect2 work_area;

    History<Object *> history;
};
