#ifndef LAZ_PLAYER_HPP
#define LAZ_PLAYER_HPP

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Time.hpp>

#include "common.hpp"

namespace laz
{

class Input;

class Player : public sf::Drawable, public sf::Transformable
{
public:
  Player();

  void update(Input& input, const sf::Time& deltaTime);

private:
  sf::RectangleShape _rectangle;

  // state

  v2f _velocity          = { 0.f, 0.f };
  v2f _maxSpeed          = { 128.f, 64.f };
  f32 _gravityMultiplier = 1.f;
  f32 _acceleration      = this->_maxSpeed.x * 4.f;
  f32 _friction          = this->_maxSpeed.x * 4.f;

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

}

#endif
