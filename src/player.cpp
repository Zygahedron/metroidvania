#include "player.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <cmath>

#include "input.hpp"
#include "tilemap.hpp"

namespace laz
{

using Buttons = Input::Buttons;
using ButtonStates = Input::ButtonState::States;

Player::Player()
{
  this->_collider = sf::FloatRect({ 0.f, 0.f }, { 8.f, 8.f });
  this->_rectangle = sf::RectangleShape(this->_collider.size);
}

void Player::update(Input& input, const sf::Time& deltaTime, const Tilemap& tilemap)
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

  horizontal *= deltaTime.asSeconds();
  vertical *= deltaTime.asSeconds();
  // Collision
  while (horizontal > 0.f)
  {
    f32 step = std::min(horizontal, 1.f);
    if (this->checkTileCollision(tilemap, { step, 0.f }))
    {
      horizontal = 0.f;
      this->_velocity.x = 0.f;
      v2f pos = this->getPosition();
      pos.x = std::ceil(pos.x);
      this->setPosition(pos);
      break;
    }

    this->move(v2f(step, 0.f));
    horizontal -= step;
  }
  while (horizontal < 0.f)
  {
    f32 step = std::max(horizontal, -1.f);
    if (this->checkTileCollision(tilemap, { step, 0.f }))
    {
      horizontal = 0.f;
      this->_velocity.x = 0.f;
      v2f pos = this->getPosition();
      pos.x = std::floor(pos.x);
      this->setPosition(pos);
      break;
    }

    this->move(v2f(step, 0.f));
    horizontal -= step;
  }

  while (vertical > 0.f)
  {
    f32 step = std::min(vertical, 1.f);
    if (this->checkTileCollision(tilemap, { 0.f, step }))
    {
      vertical = 0.f;
      this->_velocity.y = 0.f;
      v2f pos = this->getPosition();
      pos.y = std::ceil(pos.y);
      this->setPosition(pos);
      this->_grounded = true;
      break;
    }

    this->move(v2f(0.f, step));
    vertical -= step;
  }
  while (vertical < 0.f)
  {
    f32 step = std::max(vertical, -1.f);
    if (this->checkTileCollision(tilemap, { 0.f, step }))
    {
      vertical = 0.f;
      this->_velocity.y = 0.f;
      v2f pos = this->getPosition();
      pos.y = std::floor(pos.y);
      this->setPosition(pos);
      break;
    }

    this->move(v2f(0.f, step));
    vertical -= step;
  }
}

bool Player::checkTileCollision(const Tilemap& tilemap, const v2f offset)
{
  sf::Rect<u16> tileRect = this->getTileRect(tilemap, offset);
  for (u16 y = 0; y < tileRect.size.y; y++)
  {
    for (u16 x = 0; x < tileRect.size.x; x++)
    {
      v2<u16> t = { (u16)(tileRect.position.x + x), (u16)(tileRect.position.y + y) };

      using Collision = Tileset::TileData::CollisionTypes;
      const Tileset::TileData& tileData = tilemap.getTileData(t.x, t.y);
      switch (tileData.collisionType)
      {
      case Collision::Solid:
        return true;
      default:
        break;
      }
    }
  }

  return false;
}

const sf::Rect<u16> Player::getTileRect(const Tilemap& tilemap, const v2f offset) const
{
  v2f size = this->_collider.size;
  v2f tl = this->getPosition() + offset;
  v2f br = tl + size - v2f(0.01f, 0.01f);

  v2<u16> tlTile = tilemap.getTileCoordAtPosition(tl);
  v2<u16> brTile = tilemap.getTileCoordAtPosition(br);

  v2<u16> tileSize = brTile - tlTile + v2<u16>(1, 1);

  return sf::Rect<u16>(tlTile, tileSize);
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  states.transform *= this->getTransform();
  target.draw(this->_rectangle, states);
}

}
