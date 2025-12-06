#include <crtdbg.h>
#include <iostream>
#include <string>

#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>

#include <fmt/format.h>
#include <lodepng.h>
#include <cpptrace/from_current.hpp>

#include "constants.h"
#include "platform.h"
#include "services/game_files_service.h"
#include "theme.h"
#include "tiles_browser/tiles_browser.h"

void main_window()
{
    sf::RenderWindow window(sf::VideoMode({ 1920, 1080 }), "PZ Map Generator");
    sf::Vector2u winsize = window.getSize();
    sf::Clock deltaClock;

    window.setFramerateLimit(60);

    sf::Texture texture;
    sf::Sprite sprite(texture);

    platform::windows::setWindowDarkMode(window);

    GameFilesService gamefileService(constants::GAME_PATH);
    tgui::Gui gui{ window };

    TilesBrowser tilesBrowser(gui, window, gamefileService);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            gui.handleEvent(*event);

            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (const auto *resized = event->getIf<sf::Event::Resized>())
            {
                sf::FloatRect visibleArea({ 0.f, 0.f }, { static_cast<float>(resized->size.x), static_cast<float>(resized->size.y) });
                window.setView(sf::View(visibleArea));
            }
        }

        // viewport drawings
        window.clear(Colors::backgroundColor.sfml());

        tilesBrowser.update(window);

        gui.draw();
        window.display();
    }
}

int main()
{
    CPPTRACE_TRY
    {
        main_window();
    }
    CPPTRACE_CATCH(const std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
        cpptrace::from_current_exception().print();
    }
}