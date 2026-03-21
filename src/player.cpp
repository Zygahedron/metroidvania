#include "player.hpp"

#include <SFML/Graphics/RectangleShape.hpp>

#include "input.hpp"

namespace laz
{

using Buttons = Input::Buttons;
using ButtonStates = Input::ButtonState::States;

Player::Player()
{
  this->_rectangle = sf::RectangleShape({ 8, 8 });
}

void Player::update(Input& input, const sf::Time& deltaTime)
{
  bool left  = input.isButtonDown(Buttons::LEFT);
  bool right = input.isButtonDown(Buttons::RIGHT);

  f32 accelerationD = this->_acceleration * deltaTime.asSeconds();
  f32 frictionD     = this->_friction * deltaTime.asSeconds();

  f32 newVelocity = this->_velocity.x;
  if (left && !right)
  {
    newVelocity -= accelerationD;
    if (newVelocity < -this->_maxSpeed.x)
      newVelocity = -this->_maxSpeed.x;
  }
  else if (this->_velocity.x < 0.f)
  {
    newVelocity += frictionD;
    if (newVelocity > 0.f)
      newVelocity = 0.f;
  }
  if (right && !left)
  {
    newVelocity += accelerationD;
    if (newVelocity > this->_maxSpeed.x)
      newVelocity = this->_maxSpeed.x;
  }
  else if (this->_velocity.x > 0.f)
  {
    newVelocity -= frictionD;
    if (newVelocity < 0.f)
      newVelocity = 0.f;
  }
  this->_velocity.x = newVelocity;

  this->move(this->_velocity * deltaTime.asSeconds());
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  states.transform *= this->getTransform();
  target.draw(this->_rectangle, states);
}

}
