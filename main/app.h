#pragma once

#include <imgui-SFML.h>
#include <imgui.h>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

struct AppContext
{
    // windowSize -> pilote le layout global de la fenetre (taille et position du viewport et de l'ui)
    // filterKeyword -> pilote l'affichage d'une liste de tilesheet
    // selectedTilesheet -> mise à jour via imgui, depuis la liste des tilesheet (filtrée par le filterKeyword), elle pilote la tilesheet à afficher
    // hoveredTexture -> dépend de la position de la souris dans la fenetre, pilote la fenetre de debug qui affiche les infos de la texture extraits de la tilesheet courante
};

class App
{
public:
    sf::RenderWindow &window;

    App(sf::RenderWindow &window) : window(window) {}

    void run() {}
    void handleEvents() {}
    void update() {}
    void render() {}
};