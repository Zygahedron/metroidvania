#include "player.hpp"
#include <SFML/Graphics/RectangleShape.hpp>

namespace laz
{

Player::Player()
{
  this->_rectangle = sf::RectangleShape({ 8, 8 });
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  states.transform *= this->getTransform();
  target.draw(this->_rectangle, states);
}

}
