#pragma once

#include "contentview.h"
#include "tab.h"

class ContentViewTab : public Tab {
   public:
    ContentViewTab();
    ~ContentViewTab();

    vec2 get_required_size() const override;

   private:
    ContentView *content_view;
};
