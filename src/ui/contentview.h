#pragma once

#include "control.h"
#include "core/stack.h"
#include "imagebutton.h"
#include "listview.h"
#include "scrollcontainer.h"
#include "uibox.h"

class ContentTile : public ImageButton {
   public:
    ContentTile(const String& filepath);

    vec2 get_required_size() const override;
};

class ContentView : public Control {
   public:
    ContentView();
    ~ContentView();

    vec2 get_required_size() const override;
    vec2 get_full_size() const;

    void draw();

    void add_item(const String& path);

    void browse_to(const String& path);

    void clear_history();

    void go_up();
    void go_home();
    void go_back();
    void go_forward();

   private:
    rect2 work_area, buttons_area;

    Container buttons;

    ImageButton* back;
    ImageButton* forward;

    ListView list;

    String currentdir;

    Stack<String> history;
    Stack<String> future;
};
