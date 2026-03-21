#ifndef LAZ_INPUT_HPP
#define LAZ_INPUT_HPP

#include "common.hpp"

#include <map>
#include <vector>

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

namespace laz
{

class Input
{
public:
  struct ButtonState;

  enum class Buttons : u8
  {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    CONFIRM,
    CANCEL,
    JUMP,
    ATTACK,
    MENU,
  };

  ButtonState getButtonState(const Buttons button) const { return this->_buttonStates.at(button); }
  void        setButtonState(const Buttons button, const ButtonState state);

  void handleButtonEvent(const sf::Keyboard::Key key, bool isPressed);
  void handleButtonEvent(const u32 buttonID, bool isPressed);
  void update(const sf::Time& deltaTime);

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

private:
  std::map<Buttons, ButtonState> _buttonStates;

  std::map<sf::Keyboard::Key, std::vector<Buttons>> _keyboardBindings;
  std::map<u32, std::vector<Buttons>>               _controllerBindings;

  u32 _joystickID;
};

}

#endif
