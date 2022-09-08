#include "consoletab.h"

#include "dock.h"

ConsoleTab::ConsoleTab() {
    textbox = new TextBox("Reading Output...");
    add_child(textbox);

    textbox->set_anchors(ANCHOR_BEGIN, ANCHOR_BEGIN, ANCHOR_END, ANCHOR_END);
    textbox->set_margins(4, 4, 4, 4);

    ERROR_HANDLER->connect(this, "log");

    set_tab_title("Console");
}

ConsoleTab::~ConsoleTab() {}

vec2 ConsoleTab::get_required_size() const { return vec2(150); }

void ConsoleTab::handle_event(UIEvent* ui_event) {}

void ConsoleTab::log(int p_index) {
    TMessage* msg = ERROR_HANDLER->get_message(p_index);

    // if (msg->type != TMessage::T_WARNING)
    textbox->push_back_line(ERROR_HANDLER->get_message(p_index)->description);

    textbox->set_caret_bottom();
}

#undef CLASSNAME
#define CLASSNAME ConsoleTab

void ConsoleTab::bind_methods() { REG_METHOD(log); }
