#include "debug_panel.h"
#include "theme.h"

DebugPanel::DebugPanel(tgui::Gui &gui)
{
    panel = tgui::Panel::create();
    panel->getRenderer()->setBackgroundColor(Colors::backgroundColor.tgui());
    panel->getRenderer()->setPadding(tgui::Padding(5));
    panel->setSize({ 100, 50 });

    fpsLabel = tgui::Label::create();
    fpsLabel->getRenderer()->setTextColor(Colors::fontColor.tgui());

    panel->add(fpsLabel);
    gui.add(panel);
}

void DebugPanel::setFPS(float value)
{
    fpsLabel->setText(fmt::format("{:.3f} fps", value));
}