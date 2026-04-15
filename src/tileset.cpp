#include "tileset.hpp"

namespace laz
{

Tileset::Tileset()
  : _tileSize(0, 0), _tileData(), _texture() {}

Tileset::~Tileset() {}

void Tileset::generate(const v2u& tileSize, const sf::Texture& texture, const std::vector<TileData> tileData)
{
  this->_tileSize = tileSize;
  this->_texture = texture;
  this->_tileData = tileData;
}

const v2u& Tileset::getTileSize() const
{
  return this->_tileSize;
}
const sf::Texture& Tileset::getTexture() const
{
  return this->_texture;
}
const std::vector<Tileset::TileData>& Tileset::getTileData() const
{
  return this->_tileData;
}

const Tileset::TileData& Tileset::getTileData(u16 tileID) const
{
  if (tileID == UINT16_MAX)
    return this->_tileData[0];
  return this->_tileData.at(tileID);
}

}
