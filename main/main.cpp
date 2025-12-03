#include <crtdbg.h>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "imgui-SFML.h"
#include "imgui.h"
#include <fmt/format.h>
#include <lodepng.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <cpptrace/from_current.hpp>

#include "constants.h"
#include "files/lotheader.h"
#include "files/lotpack.h"
#include "files/texturepack.h"
#include "files/tiledefinition.h"
#include "io/binary_reader.h"
#include "io/file_reader.h"
#include "math/md5.h"
#include "services/game_files_service.h"

sf::Vector2u getPNGSize(const BytesBuffer &data)
{
    if (data.size() < 24)
        throw std::runtime_error("Not enough data for PNG");

    const uint8_t pngSig[8] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };
    for (int i = 0; i < 8; ++i)
        if (data[i] != pngSig[i])
            throw std::runtime_error("Invalid PNG signature");

    uint32_t width = (data[16] << 24) | (data[17] << 16) | (data[18] << 8) | data[19];
    uint32_t height = (data[20] << 24) | (data[21] << 16) | (data[22] << 8) | data[23];

    return sf::Vector2u{ width, height };
}

TexturePack::Texture *getTextureByName(TexturePack::Page &page, std::string textureName)
{
    TexturePack::Texture *textureData = nullptr;

    for (int i = 0; i < page.textures.size(); i++)
    {
        if (page.textures[i].name == textureName)
        {
            textureData = &page.textures[i];
            break;
        }
    }

    if (textureData == nullptr)
    {
        throw std::runtime_error("texture not found");
    }

    return textureData;
}

TexturePack::Texture *getTextureByPosition(TexturePack::Page &page, sf::Vector2f pos)
{
    for (int i = 0; i < page.textures.size(); i++)
    {
        TexturePack::Texture text = page.textures[i];

        if (pos.x >= text.x && pos.y >= text.y && pos.x < text.x + text.width && pos.y < text.y + text.height)
        {
            return &page.textures[i];
        }
    }

    return nullptr;
}

sf::RectangleShape getOutlineRectangle(TexturePack::Texture *textureData, sf::Transform transform)
{
    sf::Vector2f size = transform.transformPoint({ static_cast<float>(textureData->width), static_cast<float>(textureData->height) });
    sf::Vector2f position = transform.transformPoint({ static_cast<float>(textureData->x), static_cast<float>(textureData->y) });

    sf::RectangleShape rectangle(size);
    rectangle.setPosition(position);
    rectangle.setOutlineColor(sf::Color::Red);
    rectangle.setOutlineThickness(1.0f);
    rectangle.setFillColor(sf::Color::Transparent);

    return rectangle;
}

sf::Texture loadTexture(TexturePack::Page &page)
{
    sf::Texture texture;

    if (!texture.loadFromMemory(page.png.data(), page.png.size()))
    {
        throw std::runtime_error("Erreur : Échec du chargement de la texture depuis le buffer PNG.");
    }

    return texture;
}

void drawSpriteOutline(sf::RenderWindow &window, sf::Sprite &sprite, TexturePack::Page &page, TexturePack::Texture *&hoveredTexture)
{
    sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    sf::Transform inverse = sprite.getInverseTransform();
    sf::Vector2f local = inverse.transformPoint(mouse);

    hoveredTexture = getTextureByPosition(page, local);

    if (hoveredTexture != nullptr)
    {
        sf::RectangleShape spriteOutiline = getOutlineRectangle(hoveredTexture, sprite.getTransform());
        window.draw(spriteOutiline);
    }
}

void drawDebugWindow(const sf::RenderWindow &window, TexturePack::Texture *hoveredTexture, bool firstFrame)
{
    const float panelWidth = 350.0;

    sf::Vector2u winSize = window.getSize();

    // fmt::println("pos: [{}, {}], size: [{}, {}]", winSize.x - panelWidth, winSize.y, panelWidth, winSize.y);

    ImGuiWindowFlags winflags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;

    ImGui::SetNextWindowPos(ImVec2(winSize.x - panelWidth, 0));
    ImGui::SetNextWindowSize(ImVec2(panelWidth, winSize.y));
    ImGui::Begin("Debug", nullptr, winflags);

    if (hoveredTexture != nullptr)
    {
        ImGui::Text("name: %s", hoveredTexture->name.c_str());
        ImGui::Text("x: %d", hoveredTexture->x);
        ImGui::Text("y: %d", hoveredTexture->y);
        ImGui::Text("width: %d", hoveredTexture->width);
        ImGui::Text("height: %d", hoveredTexture->height);
        ImGui::Text("ox: %d", hoveredTexture->ox);
        ImGui::Text("oy: %d", hoveredTexture->oy);
        ImGui::Text("ow: %d", hoveredTexture->ow);
        ImGui::Text("oh: %d", hoveredTexture->oh);
    }

    ImGui::End();
}

void drawExplorerWindow(const sf::RenderWindow &window, const GameFilesService &gameFileService, bool firstFrame)
{
    const float panelWidth = 250.0;

    sf::Vector2u winSize = window.getSize();

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(panelWidth, winSize.y));
    ImGui::Begin("TileSheets");

    // Liste de données
    std::vector<std::string> items = gameFileService.getPageNames();

    // Buffer pour le filtre
    static char filterBuffer[128] = "";

    ImGui::InputText("Filter", filterBuffer, IM_ARRAYSIZE(filterBuffer));
    ImGui::BeginChild("ListBox", ImVec2(0, 0), true);

    for (const auto &item : items)
    {
        if (filterBuffer[0] == '\0' || item.find(filterBuffer) != std::string::npos)
        {
            if (ImGui::Selectable(item.c_str()))
            {
                // Action lorsque l'item est sélectionné
                fmt::println("Item sélectionné: {}", item);
            }
        }
    }

    ImGui::EndChild();
    ImGui::End();
}

void main_window()
{
    sf::RenderWindow window(sf::VideoMode({ 1920, 1080 }), "My window");
    sf::Clock deltaClock;

    if (!ImGui::SFML::Init(window))
    {
        throw std::runtime_error("failed intilializing ImGui.");
    }

    GameFilesService gamefileService(constants::GAME_PATH);
    TexturePack::Page page = gamefileService.getPageByName("Tiles1x1");

    sf::Vector2u textureSize = getPNGSize(page.png);
    sf::Texture texture = loadTexture(page);
    sf::Sprite sprite(texture);
    const float size = 1.0f;
    sprite.setScale({ size, size });

    TexturePack::Texture *hoveredTexture = nullptr;

    bool firstFrame = true;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            ImGui::SFML::ProcessEvent(window, *event);

            if (!ImGui::GetIO().WantCaptureMouse)
            {
                if (event->is<sf::Event::Closed>())
                {
                    window.close();
                }
                else if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>())
                {
                    if (keyPressed->scancode == sf::Keyboard::Scancode::Up)
                        window.close();
                }
                else if (const auto *resized = event->getIf<sf::Event::Resized>())
                {
                    sf::FloatRect visibleArea({ 0.f, 0.f }, { static_cast<float>(resized->size.x), static_cast<float>(resized->size.y) });
                    window.setView(sf::View(visibleArea));
                }
            }
        }

        // UI drawings
        ImGui::SFML::Update(window, deltaClock.restart());

        // debug window
        drawExplorerWindow(window, gamefileService, firstFrame);
        drawDebugWindow(window, hoveredTexture, firstFrame);

        // viewport drawings
        window.clear();
        window.draw(sprite);

        drawSpriteOutline(window, sprite, page, hoveredTexture);

        ImGui::SFML::Render(window);
        window.display();
        firstFrame = false;
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