#pragma once

#include "tab.h"

class Container;
class TreeView;
class ImageButton;
class World;
class TreeElement;
class Viewport;
class TextField;

class ExplorerTab : public Tab {
    OBJ_DEFINITION(ExplorerTab, Tab);

   public:
    ExplorerTab();
    ExplorerTab(Viewport *p_viewport);

    virtual ~ExplorerTab();

    void add_file(const String &p_path);

    Node *get_selected_object() const;

    void delete_element();
    void add_button_clicked();
    void right_clicked(TreeElement *p_element);

    void update_items();

    vec2 get_required_size() const override;
    void handle_event(UIEvent *ui_event) override;
    void notification(int p_notification) override;

    void set_viewport(Viewport *p_viewport);
    Viewport *get_viewport() const;

    void add_object(const String &p_type);

    void emit_selected(TreeElement *p_element);

    void select(WorldObject *p_object);

    static void bind_methods();

   private:
    TreeView *tree_view;
    Container *buttons;

    Viewport *viewport;

    ImageButton *add_button;

    TextField *search_field;
};
