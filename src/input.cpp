#include "input.hpp"

#include <filesystem>
#include <fstream>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <spdlog/spdlog.h>
#include <yaml-cpp/yaml.h>

namespace laz
{

Input::Input()
{
  for (u8 i = 0; i < (u8)Buttons::BUTTON_COUNT; i++)
  {
    Buttons btn = (Buttons)i;

    this->_buttonStates[(Buttons)btn] = {};

    sf::RectangleShape rect = sf::RectangleShape({ 8.f, 8.f });
    rect.setFillColor(sf::Color::Black);
    rect.setOutlineColor(sf::Color::White);
    rect.setOutlineThickness(1.f);
    rect.setPosition({ 8.f * i, 0.f });
    this->_display[(Buttons)btn] = rect;
  }
}

bool Input::loadBindingsFromYAML(const std::string& filepath)
{
  if (!std::filesystem::exists(filepath))
  {
    YAML::Node defaultYAML;
    defaultYAML["kb"] = this->_keyboardBindings;
    defaultYAML["js"] = this->_controllerBindings;
    defaultYAML["jsid"] = this->_controllerID;

    std::ofstream ofs = std::ofstream(filepath);
    ofs << defaultYAML;
    ofs.close();
  }

  YAML::Node yaml = readYAML(filepath);

  if (yaml.IsNull())
    return false;

  auto keyboard = yaml["kb"].as<std::map<sf::Keyboard::Key, std::vector<Buttons>>>();
  auto controller = yaml["js"].as<std::map<u32, std::vector<Buttons>>>();
  auto controllerID = yaml["jsid"].as<u32>();

  this->_keyboardBindings = keyboard;
  this->_controllerBindings = controller;
  this->_controllerID = controllerID;

  return true;
}

void Input::setButtonState(const Buttons button, const ButtonState::States state)
{
  ButtonState& fullState = this->_buttonStates[button];
  fullState.state = state;
  switch (state)
  {
  case ButtonState::States::NOT_PRESSED:
    this->_display[button].setFillColor(sf::Color::Black);
    fullState.heldTimer = sf::Time::Zero;
    break;
  case ButtonState::States::JUST_PRESSED:
    this->_display[button].setFillColor(sf::Color::Green);
    break;
  case ButtonState::States::HELD:
    this->_display[button].setFillColor(sf::Color::White);
    break;
  case ButtonState::States::JUST_RELEASED:
    this->_display[button].setFillColor(sf::Color::Red);
    break;
  }
}

void Input::handleButtonEvent(const sf::Keyboard::Key key, bool isPressed)
{
  if (!this->_keyboardBindings.contains(key))
    return;

  std::vector<Buttons> bindings = this->_keyboardBindings[key];
  for (Buttons btn : bindings)
  {
    if (!this->_buttonStates.contains(btn))
      this->_buttonStates[btn] = ButtonState();

    ButtonState& state = this->_buttonStates[btn];
    if (isPressed)
    {
      state.pressedCount++;
      if (state.state == ButtonState::States::NOT_PRESSED)
        this->setButtonState(btn, ButtonState::States::JUST_PRESSED);
    }
    else if (state.pressedCount > 0)
    {
      state.pressedCount--;
      if (state.pressedCount == 0)
        this->setButtonState(btn, ButtonState::States::JUST_RELEASED);
    }
  }
}

void Input::update(const sf::Time& deltaTime)
{
  for (auto& pair : this->_buttonStates)
  {
    Buttons button = pair.first;
    ButtonState& state = pair.second;

    switch (state.state)
    {
    case ButtonState::States::JUST_PRESSED:
      this->setButtonState(button, ButtonState::States::HELD);
      break;
    case ButtonState::States::HELD:
      state.heldTimer += deltaTime;
      break;
    case ButtonState::States::JUST_RELEASED:
      this->setButtonState(button, ButtonState::States::NOT_PRESSED);
      break;
    default:
      break;
    }
  }
}

bool Input::consumePress(const Buttons button)
{
  ButtonState state = this->_buttonStates[button];
  if (state.state != ButtonState::States::JUST_PRESSED)
    return false;

  this->setButtonState(button, ButtonState::States::HELD);
  return true;
}

bool Input::consumeRelease(const Buttons button, const sf::Time& heldThreshold)
{
  ButtonState& state = this->_buttonStates[button];

  if (state.state != ButtonState::States::JUST_RELEASED ||
      state.heldTimer < heldThreshold)
    return false;

  this->setButtonState(button, ButtonState::States::NOT_PRESSED);
  return true;
}

void Input::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  for (auto pair : this->_display)
  {
    target.draw(pair.second, states);
  }
}

}
