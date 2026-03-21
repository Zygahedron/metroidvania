#include <SFML/System/Clock.hpp>
#include <iostream>

#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "common.hpp"
#include "asset_manager.hpp"
#include "input.hpp"
#include "player.hpp"
#include "tilemap.hpp"
#include "tileset.hpp"

const laz::u16 RESOLUTION_X = 640;
const laz::u16 RESOLUTION_Y = 360;
const laz::u16 RESOLUTION_SCALE = 2;

int main(int argc, char* argv[])
{
  sf::RenderWindow* window =
    new sf::RenderWindow(
      sf::VideoMode({ RESOLUTION_X * RESOLUTION_SCALE, RESOLUTION_Y * RESOLUTION_SCALE }),
      "test",
      sf::Style::Titlebar | sf::Style::Close,
      sf::State::Windowed
    );
  sf::View* view = new sf::View({ 0.f, 0.f }, { RESOLUTION_X, RESOLUTION_Y });
  window->setView(*view);

  laz::Player player;
  player.setOrigin({ 4.0f, 4.0f });
  player.setPosition({ 0.f, 0.f });

  laz::AssetManager<laz::Tileset> tilesetManager = laz::AssetManager<laz::Tileset>("assets/tilesets", ".yaml");
  tilesetManager.load("test");

  std::vector<laz::u16>* tiles = new std::vector<laz::u16>{
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 0, 0, 0, 0, 0, 0, 0, 0, 2,
    2, 1, 1, 1, 1, 1, 1, 1, 1, 2,
    2, 0, 0, 0, 0, 0, 0, 0, 0, 2,
    2, 1, 1, 1, 1, 1, 1, 1, 1, 2,
    2, 0, 0, 0, 0, 0, 0, 0, 0, 2,
    2, 1, 1, 1, 1, 1, 1, 1, 1, 2,
    2, 0, 0, 0, 0, 0, 0, 0, 0, 2,
    2, 1, 1, 1, 1, 1, 1, 1, 1, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  };
  laz::Tilemap* tilemap = new laz::Tilemap({10, 10}, &tilesetManager.get("test"), *tiles);
  tilemap->setOrigin({ 40.f, 32.f });

  laz::Input input;

  std::cout << "Hello world!" << std::endl;

  sf::Clock clock;
  while (window->isOpen())
  {
    sf::Time deltaTime = clock.restart();

    input.update(deltaTime);

    while (const std::optional event = window->pollEvent())
    {
      if (event->is<sf::Event::Closed>())
        window->close();
    }

    window->clear(sf::Color::Magenta);
    window->draw(*tilemap);
    window->draw(player);
    window->display();
  }

  delete tilemap;
  delete tiles;
  delete view;
  delete window;

  return 0;
}
