#ifndef LAZ_INPUT_HPP
#define LAZ_INPUT_HPP

#include "common.hpp"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <map>
#include <string>
#include <vector>

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

namespace laz
{

class Input : public sf::Drawable
{
public:
  struct ButtonState
  {
    enum class States : u8
    {
      NOT_PRESSED,
      JUST_PRESSED,
      HELD,
      JUST_RELEASED,
    };

    States   state        = States::NOT_PRESSED;
    sf::Time heldTimer    = sf::Time::Zero;
    u8       pressedCount = 0;
  };

  enum class Buttons : u8
  {
    LEFT,
    DOWN,
    UP,
    RIGHT,
    CONFIRM,
    CANCEL,
    JUMP,
    ATTACK,
    MENU,

    BUTTON_COUNT
  };

  Input();

  bool loadBindingsFromYAML(const std::string& filepath);

  ButtonState::States getButtonState(const Buttons button) const { return this->_buttonStates.at(button).state; }
  void                setButtonState(const Buttons button, const ButtonState::States state);

  bool isButtonDown(const Buttons button) const
  {
    auto state = this->_buttonStates.at(button).state;
    return state == ButtonState::States::JUST_PRESSED || state == ButtonState::States::HELD;
  }

  void handleButtonEvent(const sf::Keyboard::Key key, bool isPressed);
  void handleButtonEvent(const u32 buttonID, bool isPressed);
  void update(const sf::Time& deltaTime);

  bool consumePress(const Buttons button);
  bool consumeRelease(const Buttons button, const sf::Time& heldThreshold = sf::Time::Zero);

private:
  std::map<Buttons, ButtonState> _buttonStates = {};

  std::map<sf::Keyboard::Key, std::vector<Buttons>> _keyboardBindings =
  {
    { sf::Keyboard::Key::Up, { Buttons::UP }},
    { sf::Keyboard::Key::Down, { Buttons::DOWN }},
    { sf::Keyboard::Key::Left, { Buttons::LEFT }},
    { sf::Keyboard::Key::Right, { Buttons::RIGHT }},
    { sf::Keyboard::Key::Z, { Buttons::CONFIRM, Buttons::JUMP }},
    { sf::Keyboard::Key::X, { Buttons::CANCEL, Buttons::ATTACK }},
    { sf::Keyboard::Key::Escape, { Buttons::MENU }},
  };
  std::map<u32, std::vector<Buttons>> _controllerBindings =
  {

  };

  u32 _controllerID = 0;

  std::map<Buttons, sf::RectangleShape> _display = {};

  void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

}

namespace YAML
{

template<>
struct convert<laz::Input::Buttons>
{
  static Node encode(const laz::Input::Buttons& rhs)
  {
    Node node;

    std::string name;
    switch (rhs)
    {
    case laz::Input::Buttons::ATTACK:
      name = "attack";
      break;
    case laz::Input::Buttons::CANCEL:
      name = "cancel";
      break;
    case laz::Input::Buttons::CONFIRM:
      name = "confirm";
      break;
    case laz::Input::Buttons::DOWN:
      name = "down";
      break;
    case laz::Input::Buttons::JUMP:
      name = "jump";
      break;
    case laz::Input::Buttons::LEFT:
      name = "left";
      break;
    case laz::Input::Buttons::MENU:
      name = "menu";
      break;
    case laz::Input::Buttons::RIGHT:
      name = "right";
      break;
    case laz::Input::Buttons::UP:
      name = "up";
      break;
    default:
      throw;
    }

    node = name;
    return node;
  }

  static bool decode(const Node& node, laz::Input::Buttons& rhs)
  {
    if (!node.IsScalar())
      return false;
    std::string str = node.as<std::string>();
    if (str == "attack")
      rhs = laz::Input::Buttons::ATTACK;
    else if (str == "cancel")
      rhs = laz::Input::Buttons::CANCEL;
    else if (str == "confirm")
      rhs = laz::Input::Buttons::CONFIRM;
    else if (str == "down")
      rhs = laz::Input::Buttons::DOWN;
    else if (str == "jump")
      rhs = laz::Input::Buttons::JUMP;
    else if (str == "left")
      rhs = laz::Input::Buttons::LEFT;
    else if (str == "menu")
      rhs = laz::Input::Buttons::MENU;
    else if (str == "right")
      rhs = laz::Input::Buttons::RIGHT;
    else if (str == "up")
      rhs = laz::Input::Buttons::UP;
    else
      throw;

    return true;
  }
};

template<>
struct convert<sf::Keyboard::Key>
{
  static Node encode(const sf::Keyboard::Key& rhs)
  {
    Node node;
    node = (laz::i32)rhs;
    return node;
  }

  static bool decode(const Node& node, sf::Keyboard::Key& rhs)
  {
    if (!node.IsScalar())
      return false;

    rhs = (sf::Keyboard::Key)node.as<laz::i32>();
    return true;
  }
};

}

#endif
