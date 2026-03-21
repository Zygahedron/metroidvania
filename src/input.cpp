#include "input.hpp"

namespace laz
{

void Input::setButtonState(const Input::Buttons button, const Input::ButtonState state)
{
  this->_buttonStates[button] = state;
}

void Input::handleButtonEvent(const sf::Keyboard::Key key, bool isPressed)
{
  if (!this->_keyboardBindings.contains(key))
    return;

  std::vector<Input::Buttons> bindings = this->_keyboardBindings[key];
  for (Input::Buttons btn : bindings)
  {
    if (!this->_buttonStates.contains(btn))
      this->_buttonStates[btn] = ButtonState();

    ButtonState& state = this->_buttonStates[btn];
    if (state.state == ButtonState::States::NOT_PRESSED && isPressed)
    {
      state.state = ButtonState::States::JUST_PRESSED;
    }
    else if (state.state == ButtonState::States::HELD && !isPressed)
    {
      state.state = ButtonState::States::JUST_RELEASED;
    }

    if (isPressed)
    {
      state.pressedCount++;
      if (state.state == ButtonState::States::NOT_PRESSED)
        state.state = ButtonState::States::JUST_PRESSED;
    }
    else if (state.pressedCount > 0)
    {
      state.pressedCount--;
      if (state.pressedCount == 0)
        state.state = ButtonState::States::JUST_RELEASED;
    }
  }
}

void Input::update(const sf::Time& deltaTime)
{
  for (auto& pair : this->_buttonStates)
  {
    Input::ButtonState& state = pair.second;

    switch (state.state)
    {
    case ButtonState::States::JUST_PRESSED:
      state.state = ButtonState::States::HELD;
      break;
    case ButtonState::States::HELD:
      state.heldTimer += deltaTime;
      break;
    case ButtonState::States::JUST_RELEASED:
      state.state = ButtonState::States::NOT_PRESSED;
      break;
    default:
      break;
    }
  }
}

}
