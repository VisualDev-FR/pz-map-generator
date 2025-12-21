#include "debug_panel.h"
#include "TGUI/Widgets/Label.hpp"
#include "theme.h"

DebugPanel::DebugPanel(tgui::Gui &gui)
{
    panel = tgui::Panel::create();
    panel->getRenderer()->setBackgroundColor(Colors::backgroundColor.tgui());
    panel->getRenderer()->setPadding(tgui::Padding(5));
    panel->setSize({ 200, 80 });

    fpsLabel = tgui::Label::create();
    fpsLabel->getRenderer()->setTextColor(Colors::fontColor.tgui());
    fpsLabel->setPosition({ 0, 0 });

    drawCallsLabel = tgui::Label::create();
    drawCallsLabel->getRenderer()->setTextColor(Colors::fontColor.tgui());
    drawCallsLabel->setPosition({ 0, 20 });

    timerLabel = tgui::Label::create();
    timerLabel->getRenderer()->setTextColor(Colors::fontColor.tgui());
    timerLabel->setPosition({ 0, 40 });

    panel->add(fpsLabel);
    panel->add(timerLabel);
    panel->add(drawCallsLabel);

    gui.add(panel);
}

void DebugPanel::setFPS(float value)
{
    fpsLabel->setText(fmt::format("{:.0f} fps", value));
}

void DebugPanel::setTimer(float value)
{
    timerLabel->setText(fmt::format("{:.3f} ms/frame", value));
}

void DebugPanel::setDrawCalls(int value)
{
    drawCallsLabel->setText(fmt::format("{} draw calls", value));
}