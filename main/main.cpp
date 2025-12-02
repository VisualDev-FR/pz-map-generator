#include <crtdbg.h>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <fmt/format.h>
#include <lodepng.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <cpptrace/from_current.hpp>

#include "files/lotheader.h"
#include "files/lotpack.h"
#include "files/texturepack.h"
#include "files/tiledefinition.h"
#include "io/binary_reader.h"
#include "io/file_reader.h"
#include "math/md5.h"

const std::string LOTHEADER_PATH = "data/B42/27_38.lotheader";
const std::string LOTHPACK_PATH = "data/B42/world_27_38.lotpack";
const std::string TILESDEF_PATH = "data/B42/newtiledefinitions.tiles";
const std::string TEXTUREPACK_PATH = "C:/SteamLibrary/steamapps/common/ProjectZomboidB42/media/texturepacks/Tiles2x.pack";

void read_header()
{
    BytesBuffer headerBuffer = FileReader::read(LOTHEADER_PATH);
    BytesBuffer lotpackBuffer = FileReader::read(LOTHPACK_PATH);
    BytesBuffer packBuffer = FileReader::read(TEXTUREPACK_PATH);
    BytesBuffer tileDefBuffer = FileReader::read(TILESDEF_PATH);

    std::string headerHash = MD5::toHash(headerBuffer);
    std::string lotpackHash = MD5::toHash(lotpackBuffer);

    LotHeader header = LotHeader::read(headerBuffer);
    Lotpack lotpack = Lotpack::read(lotpackBuffer, header);
    TexturePack texturePack = TexturePack::read(packBuffer);
    TileDefinition tileDefinition = TileDefinition::read(tileDefBuffer);

    fmt::println("magic: {}, version: {}, md5: {}", header.magic, header.version, headerHash);
    fmt::println("magic: {}, version: {}, md5: {}", lotpack.magic, lotpack.version, lotpackHash);
    fmt::println("magic: {}, version: {}", texturePack.magic, texturePack.version);

    fmt::println("squareMap size: {}", lotpack.squareMap.size());
    fmt::println("texture pages: {}", texturePack.pages.size());
    fmt::println("tilesheets: {}", tileDefinition.tileSheets.size());
}

BytesBuffer create_png(size_t width, size_t height)
{
    BytesBuffer image(width * height * 4); // RGBA

    for (uint8_t y = 0; y < height; ++y)
    {
        for (uint8_t x = 0; x < width; ++x)
        {
            image[4 * (y * width + x) + 0] = x;   // R
            image[4 * (y * width + x) + 1] = y;   // G
            image[4 * (y * width + x) + 2] = 128; // B
            image[4 * (y * width + x) + 3] = 255; // A
        }
    }

    BytesBuffer result;
    unsigned error = lodepng::encode(result, image.data(), width, height);

    if (error)
    {
        std::cout << "Erreur PNG: " << lodepng_error_text(error) << std::endl;
    }

    return result;
}

void main_window()
{
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "My window", sf::Style::Close | sf::Style::Titlebar);

    sf::Vector2i size(128, 128);
    BytesBuffer pngBuffer = create_png(size.x, size.y);

    sf::Texture texture;
    if (!texture.loadFromMemory(pngBuffer.data(), pngBuffer.size()))
    {
        std::cerr << "Erreur : Échec du chargement de la texture depuis le buffer PNG." << std::endl;
        return;
    }

    sf::Sprite sprite1(texture);
    sf::Sprite sprite2(texture);

    sprite2.setPosition({ 128, 128 });
    sprite2.setScale({ 0.5f, 0.5f });

    std::cout << "Texture chargée avec succès." << std::endl;

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (event->is<sf::Event::MouseButtonReleased>())
                fmt::println("Mouse pressed!");
        }

        window.clear();
        window.draw(sprite1);
        window.draw(sprite2);
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