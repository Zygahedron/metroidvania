#include <iostream>

#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "common.hpp"
#include "player.hpp"
#include "tilemap.hpp"

int main(int argc, char* argv[])
{
  sf::RenderWindow* window =
    new sf::RenderWindow(
      sf::VideoMode({ 800, 600 }),
      "test",
      sf::Style::Titlebar | sf::Style::Close
    );
  sf::View* view = new sf::View({ 0.f, 0.f }, { 80.f, 60.f });
  window->setView(*view);

  laz::Player player;
  player.setOrigin({ 4.0f, 4.0f });
  player.setPosition({ 0.f, 0.f });

  sf::Texture* tileset = new sf::Texture("assets/tileset.png");
  std::vector<laz::u16>* tiles = new std::vector<laz::u16>{
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  };
  laz::Tilemap* tilemap = new laz::Tilemap({10, 8}, *tileset, *tiles);
  tilemap->setOrigin({ 40.f, 32.f });

  std::cout << "Hello world!" << std::endl;

  while (window->isOpen())
  {
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
  delete tileset;
  delete view;
  delete window;

  return 0;
}
