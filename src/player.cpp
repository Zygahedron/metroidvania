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
  bool down = input.isButtonDown(Buttons::DOWN);
  bool jump = input.isButtonDown(Buttons::JUMP);

  f32 accelerationD = this->_acceleration * deltaTime.asSeconds();
  f32 frictionD     = this->_friction * deltaTime.asSeconds();

  f32 horizontal = this->_velocity.x;
  if (left && !right)
  {
    horizontal -= accelerationD;
    if (horizontal < -this->_maxSpeed.x)
      horizontal = -this->_maxSpeed.x;
  }
  else if (this->_velocity.x < 0.f)
  {
    horizontal += frictionD;
    if (horizontal > 0.f)
      horizontal = 0.f;
  }
  if (right && !left)
  {
    horizontal += accelerationD;
    if (horizontal > this->_maxSpeed.x)
      horizontal = this->_maxSpeed.x;
  }
  else if (this->_velocity.x > 0.f)
  {
    horizontal -= frictionD;
    if (horizontal < 0.f)
      horizontal = 0.f;
  }

  f32 vertical = this->_velocity.y;
  f32 gravity = 600.f * this->_gravityMultiplier;
  if (jump && this->_grounded)
  {
    if (vertical > 0.f)
    {
      vertical = 0.f;
    }
    vertical -= this->_jumpStrength;
    gravity = 0.f;
    this->_grounded = false;
  }
  else if (jump && !this->_grounded && vertical < 0.f)
  {
    gravity *= 0.5f;
  }
  else if (down && !this->_grounded)
  {
    gravity *= 2.f;
  }
  vertical += gravity * deltaTime.asSeconds();

  this->_velocity = { horizontal, vertical };

  this->move(this->_velocity * deltaTime.asSeconds());

  // TEMP collision
  v2f pos = this->getPosition();
  if (pos.y >= 0.f)
  {
    this->setPosition({ pos.x, 0.f });
    this->_grounded = true;
  }
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  states.transform *= this->getTransform();
  target.draw(this->_rectangle, states);
}

}
