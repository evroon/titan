#pragma once

#include "core/time.h"
#include "frame.h"
#include "label.h"
#include "propertycontrol.h"
#include "uibox.h"
#include "uicallback.h"

class EditableLabel : public PropertyControl {
    OBJ_DEFINITION(EditableLabel, PropertyControl);

   public:
    EditableLabel();
    EditableLabel(const String& name);
    EditableLabel(const Variable& p_variable);

    void init();

    void switch_blink();

    vec2 get_required_size() const override;

    void handle_event(UIEvent* e) override;
    void notification(int p_notification) override;

    void move_cursor_begin();
    void move_cursor_end();
    void move_cursor_left();
    void move_cursor_right();
    void set_cursor_index(int p_cursor_index);

    void insert_at_index(int index, char kar);
    void insert_at_index(int index, const String& kar);
    void remove_at_index(int index);

    bool cursor_is_at_end();

    void value_changed() override;

    inline String get_text() const;
    virtual void set_text(const String& p_text);

    String get_empty_text() const;
    void set_empty_text(const String& p_empty_text);

    int get_index(float x) const;
    float get_position_x(int index) const;

    String get_selected_text();

    void select_all();
    void delete_selection();
    void set_selection_begin(int begin);
    void set_selection_end(int end);
    void stop_selecting();

    void update_selection_box();

    static void bind_methods();

   protected:
    void insert_at_selection(char c);

    virtual void handle_input(char c);

    String text;
    String empty_text;

   private:
    // cursor data
    int ibeam_blinks;
    bool selecting;
    int selection_start, selection_end;
    int selection_first;
    int cursor_index = 0;

    float start_margin, end_margin;

    rect2 selection_area;
    rect2 cursor_area;

    Color selection_color;

    UITimer* ibeam_blinker;
};
