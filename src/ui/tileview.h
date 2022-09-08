#pragma once

#include "control.h"
#include "uicallback.h"

class Slider;
class TileView;

struct TileElement {
    TileElement(const String& p_text);
    ~TileElement();

    void bind_tileview(TileView* p_tileview);

    void set_index(int p_index);
    int get_index();

    rect2 get_area() const;
    void set_area(const rect2& p_area);

    bool is_in_area(const vec2& p_pos);

    String get_text() const;
    void set_text(const String& p_text);

    Texture2D* get_icon() const;
    void set_icon(Texture2D* p_icon);

    void set_visible(bool p_visible);
    bool get_visible() const;

    float get_height() const;

    int get_index(float x) const;
    float get_position_x(int index) const;

   private:
    int index;
    TileView* tileview;
    Texture2D* icon;

    String text;

    rect2 area;

    bool visible;
};

class TileView : public Control {
    OBJ_DEFINITION(TileView, Control);

   public:
    TileView();
    ~TileView();

    vec2 get_required_size() const override;

    void handle_event(UIEvent* ui_event) override;

    void position_items();

    void check_slider_necessity();

    void notification(int p_notification) override;

    // unique methods
    void clear();
    void push_back_item(TileElement& item);

    int get_position(const vec2& pos);

    TileElement* get_item(int row);
    int get_item_size(int row);

    void set_item_numbers_enabled(bool p_value);

    void add_slider();
    void remove_slider();

    void set_selected(int p_row);
    int get_selected() const;

    void move_selected_begin();
    void move_selected_end();
    void move_selected_up();
    void move_selected_down();

    void set_selection(int p_selected);

    void remove_item(int p_index);

    void push_back_item(const String& p_text, Texture2D* p_icon = NULL);
    void insert_item_at_index(int index, TileElement& item);

    void set_selection(int p_begin, int p_end);
    void delete_selection();
    void set_selection_begin(int p_begin);
    void set_selection_end(int p_begin);
    void stop_selecting();

    void select_all();
    void select_none();

    bool multiple_selection() const;

    void make_visible(int p_index);

    void search(const String& p_src);
    void stop_search();

    void slider_value_changed();

    void set_font(Font* p_font);
    Font* get_font() const;

    static void bind_methods();

   private:
    int highlighted;
    int selected;

    // in case of multiple selections
    int selection_begin, selection_end;
    int selection_first;
    bool selecting;

    float scroll_offset;

    // the space in y direction not drawn on screen (if scrolling)
    float extra_space;

    bool item_numbers_enabled;

    Color background_color;
    Color selection_color;
    Color item_numbers_color;
    Color multi_select_color;

    rect2 box;

    vec2 tilesize;
    vec2i tilecount;

    float top_margin;
    float left_margin;
    float right_margin;
    float internal_left_margin;

    Array<TileElement> items;

    Font* font;
    Slider* slider;

    rect2 selection_box;
};
