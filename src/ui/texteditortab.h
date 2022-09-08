#pragma once

#include "container.h"
#include "dialog.h"
#include "imagebutton.h"
#include "tab.h"
#include "textbox.h"

class TextEditorTab : public Tab {
    OBJ_DEFINITION(TextEditorTab, Tab);

   public:
    TextEditorTab();
    TextEditorTab(const File &p_file);
    virtual ~TextEditorTab();

    vec2 get_required_size() const override;

    void handle_event(UIEvent *ui_event) override;

    void notification(int p_notification) override;

    void show_dialog();

    void open_file(const String &text);

    void savebutton_pressed();

    static void bind_methods();

   protected:
    ImageButton *save_button;
    ImageButton *open_button;
    ImageButton *new_button;

    TextBox *textbox;
    Container *buttons;

    rect2 textbox_area;
    TextFile *textfile;

    FileDialog *dialog;
};

class ShaderEditorTab : public TextEditorTab {
    OBJ_DEFINITION(ShaderEditorTab, TextEditorTab);

   public:
    ShaderEditorTab();
    ShaderEditorTab(const File &p_file);
    ShaderEditorTab(Shader *p_shader);
    virtual ~ShaderEditorTab();

    void init();

    void compile();

    static void bind_methods();

   private:
    Shader *shader;

    ImageButton *compile_button;
};
