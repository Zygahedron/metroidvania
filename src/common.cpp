#include "common.hpp"
#include <spdlog/spdlog.h>
#include <yaml-cpp/yaml.h>

namespace laz
{

YAML::Node readYAML(const std::string& filepath)
{
  YAML::Node yaml;
  try
  {
    yaml = YAML::LoadFile(filepath);
  }
  catch (std::exception& e)
  {
    spdlog::error("Failed to read YAML file '{}': {}", filepath, e.what());
  }
  return yaml;
}


}
