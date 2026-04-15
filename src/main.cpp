#include <SFML/System/Clock.hpp>

#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <spdlog/spdlog.h>

#include "common.hpp"
#include "asset_manager.hpp"
#include "input.hpp"
#include "player.hpp"
#include "tilemap.hpp"
#include "tileset.hpp"

using namespace laz;

const v2u RESOLUTION = { 640, 360 };
const u32 RESOLUTION_SCALE = 2;

const auto* tiles = new std::vector<std::vector<u16>>{
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
  { 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, },
  { 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, },
  { 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, },
  { 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, },
  { 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, },
  { 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, },
  { 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, },
  { 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, },
  { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, },
};

sf::RenderWindow* window;
sf::View view({ 0.f, 0.f }, (v2f)RESOLUTION);

AssetManager<Tileset>* tilesetManager = new AssetManager<Tileset>("assets/tilesets", ".yaml");

Input* input = new Input();

void onClosed(const sf::Event::Closed& event)
{
  window->close();
}

void onKeyPressed(const sf::Event::KeyPressed& event)
{
  input->handleButtonEvent(event.code, true);
}

void onKeyReleased(const sf::Event::KeyReleased& event)
{
  input->handleButtonEvent(event.code, false);
}

int main(int argc, char* argv[])
{
  spdlog::set_level(spdlog::level::debug);
  // Initialize window
  window =
    new sf::RenderWindow(
      sf::VideoMode(RESOLUTION * RESOLUTION_SCALE),
      "test",
      sf::Style::Titlebar | sf::Style::Close,
      sf::State::Windowed
    );
  window->setKeyRepeatEnabled(false);
  window->setView(view);
  window->setFramerateLimit(60);

  tilesetManager->load("test");

  Player player;
  // player.setOrigin({ 4.0f, 4.0f });
  player.setPosition({ 0.f, -10.1f });

  Tilemap* tilemap = new Tilemap({ 10, 10 }, &tilesetManager->get("test"), *tiles);
  // tilemap->setOrigin({ 40.f, 32.f });

  input->loadBindingsFromYAML("p1.yaml");

  sf::Clock clock;
  while (window->isOpen())
  {
    sf::Time deltaTime = clock.restart();

    // Update input first
    input->update(deltaTime);

    // Then handle events
    window->handleEvents(
      onClosed,
      onKeyPressed,
      onKeyReleased
    );

    // Then udpate game objects
    player.update(*input, deltaTime, *tilemap);

    // And then draw everything
    window->clear(sf::Color::Magenta);
    window->draw(*tilemap);
    window->draw(player);
    window->draw(*input);
    window->display();
  }

  delete tilemap;
  delete tiles;
  delete window;

  return 0;
}
