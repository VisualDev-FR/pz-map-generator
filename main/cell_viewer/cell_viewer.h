#pragma once

#include <unordered_map>
#include <unordered_set>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "files/lotheader.h"
#include "files/lotpack.h"
#include "math/math.h"
#include "services/map_files_service.h"
#include "services/tilesheet_service.h"

class CellViewer
{
public:
    struct ViewState
    {
        sf::Vector2i lastMousePos;
        sf::Vector2f center = { .0f, .0f };
        float zoomLevel = 1.0f;
        bool isDragging = false;

        void applyTo(sf::View &view, sf::RenderWindow &window)
        {
            zoomLevel = Math::fastClamp(zoomLevel, 0.05f, 10.f);

            view.setSize({ window.getSize().x * zoomLevel, window.getSize().y * zoomLevel });
            view.setCenter(center);
        }
    };

    LotHeader lotheader;
    Lotpack lotpack;

    TilesheetService *tilesheetService;

    std::unordered_map<std::string, std::optional<sf::Texture>> texturesByName;
    std::unordered_set<std::string> spritesNotFound;

    ViewState viewState;
    sf::View *view;

    bool firstFrame = true;
    int currentLayer = 2;

    CellViewer(sf::View *view, MapFilesService *mapFileService, TilesheetService *tilesheetService, int x, int y);

    sf::Texture *getOrCreateTexture(const std::string &textureName);
    sf::Sprite createSprite(TexturePack::Texture *textureData);

    void handleEvents(const sf::Event &event, sf::RenderWindow &window);
    void update(sf::RenderWindow &window);
};