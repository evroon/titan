#include "uicallback.h"

UICallback::UICallback() : is_init(false) {}

UICallback::UICallback(const std::function<void()> &p_on_call)
    : on_call(p_on_call), is_init(true) {}
void UICallback::operator()() {
    if (is_init) on_call();
}

//=========================================================================
// UITimer
//=========================================================================

UITimer::UITimer(UICallback *p_callback, int time)
    : callback(p_callback), timer(time) {
    TIME->register_uitimer(this);
}

void UITimer::update() {
    bool finished = timer.update();

    if (finished) {
        callback->operator()();
        timer.start();
    }
}

void UITimer::set_enabled(bool enabled) { timer.enabled = enabled; }
