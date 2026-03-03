#include <iostream>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>

int main(int argc, char* argv[])
{
  sf::RenderWindow window =
    sf::RenderWindow(
      sf::VideoMode({ 800, 600 }),
      "test"
    );

  std::cout << "Hello world!" << std::endl;

  while (window.isOpen())
  {
    while (const std::optional event = window.pollEvent())
    {
      if (event->is<sf::Event::Closed>())
        window.close();
    }

    window.clear(sf::Color::Magenta);
    window.display();
  }

  return 0;
}
