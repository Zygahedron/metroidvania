#include "asset_manager.hpp"

#include <filesystem>
#include <format>
#include <SFML/Graphics/Texture.hpp>
#include <spdlog/spdlog.h>
#include <yaml-cpp/yaml.h>

#include "common.hpp"
#include "tilemap.hpp"
#include "tileset.hpp"

namespace fs = std::filesystem;

namespace laz
{

template <>
Tileset* AssetManager<Tileset>::loadFromFile(const std::string& filepath)
{
  YAML::Node yaml = readYAML(std::format("{}/{}{}", this->_directory, filepath, this->_fileExtension));
  if (yaml.IsNull())
    return nullptr;

  const std::string texturePath = std::format("{}/{}", this->_directory, yaml["texture"].as<std::string>());

  if (!fs::exists(texturePath))
  {
    spdlog::error("Tileset '{}' texture file '{}' does not exist!");
    return nullptr;
  }
  v2u tileSize = v2u(yaml["tile-size"]["x"].as<u32>(), yaml["tile-size"]["y"].as<u32>());

  std::vector<Tileset::TileData> tileData;
  for (YAML::const_iterator it = yaml["tiles"].begin(); it != yaml["tiles"].end(); ++it)
  {
    Tileset::TileData data;
    YAML::Node node = it->as<YAML::Node>();

    using Collision = Tileset::TileData::CollisionTypes;
    std::string collision = node["collision-type"].as<std::string>();
    if (collision == "none")
      data.collisionType = Collision::None;
    else if (collision == "solid")
      data.collisionType = Collision::Solid;

    tileData.push_back(data);
  }

  Tileset* tileset = new Tileset();
  tileset->generate(tileSize, sf::Texture(texturePath), tileData);

  return tileset;
}

}
