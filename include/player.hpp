#ifndef LAZ_PLAYER_HPP
#define LAZ_PLAYER_HPP

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Transformable.hpp>

namespace laz
{

class Player : public sf::Drawable, public sf::Transformable
{
public:
  Player();

private:
  sf::RectangleShape _rectangle;

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

}

#endif
