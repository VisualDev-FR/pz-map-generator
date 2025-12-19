#pragma once

#include "TGUI/Backend/SFML-Graphics.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "TGUI/Widgets/Panel.hpp"

#include <fmt/format.h>

class DebugPanel
{
private:
    tgui::Panel::Ptr panel;
    tgui::Label::Ptr fpsLabel;

public:
    DebugPanel(tgui::Gui &gui);

    void setFPS(float value);
};