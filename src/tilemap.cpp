#include "tilemap.hpp"

namespace laz
{

const u8 TILESIZE_X = 8;
const u8 TILESIZE_Y = 8;

Tilemap::Tilemap(const v2u& mapSize, const sf::Texture& tileset, const std::vector<u16>& tiles)
  : _mapSize(mapSize), _tiles(tiles), _vertices(), _tileset(tileset)
{
  this->_vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
  this->_vertices.resize(this->_mapSize.x * this->_mapSize.y * 6);

  for (u32 y = 0; y < this->_mapSize.y; y++)
  {
    for (u32 x = 0; x < this->_mapSize.x; x++)
    {
      const u16 tile = this->_tiles[x + (y * this->_mapSize.x)];

      const i32 tu = tile % (this->_tileset.getSize().x / TILESIZE_X);
      const i32 tv = tile / (this->_tileset.getSize().x / TILESIZE_X);

      sf::Vertex* triangles = &this->_vertices[(x + (y * this->_mapSize.y)) * 6];

      triangles[0].position = v2f(x * TILESIZE_X, y * TILESIZE_Y);
      triangles[1].position = v2f((x + 1) * TILESIZE_X, y * TILESIZE_Y);
      triangles[2].position = v2f(x * TILESIZE_X, (y + 1) * TILESIZE_Y);
      triangles[3].position = v2f(x * TILESIZE_X, (y + 1) * TILESIZE_Y);
      triangles[4].position = v2f((x + 1) * TILESIZE_X, y * TILESIZE_Y);
      triangles[5].position = v2f((x + 1) * TILESIZE_X, (y + 1) * TILESIZE_Y);

      triangles[0].texCoords = v2f(tu * TILESIZE_X, tv * TILESIZE_Y);
      triangles[1].texCoords = v2f((tu + 1) * TILESIZE_X, tv * TILESIZE_Y);
      triangles[2].texCoords = v2f(tu * TILESIZE_X, (tv + 1) * TILESIZE_Y);
      triangles[3].texCoords = v2f(tu * TILESIZE_X, (tv + 1) * TILESIZE_Y);
      triangles[4].texCoords = v2f((tu + 1) * TILESIZE_X, tv * TILESIZE_Y);
      triangles[5].texCoords = v2f((tu + 1) * TILESIZE_X, (tv + 1) * TILESIZE_Y);
    }
  }
}

Tilemap::~Tilemap()
{
  this->_tiles.clear();
}

const v2u& Tilemap::getMapSize()                { return this->_mapSize; }
void       Tilemap::setMapSize(const v2u& size) { this->_mapSize = size; }

u16 Tilemap::getTile(u16 x, u16 y)
{
  u16 idx = x + (y * this->_mapSize.x);
  if (x > this->_mapSize.x || y > this->_mapSize.y || idx >= this->_tiles.size())
    return UINT16_MAX;

  return this->_tiles[idx];
}
void Tilemap::setTile(u16 x, u16 y, u16 tile)
{
  u16 oldTile = this->getTile(x, y);
  if (oldTile == tile || oldTile == UINT16_MAX)
    return;

  this->_tiles[x + (y * this->_mapSize.x)] = tile;
}

const sf::Texture& Tilemap::getTileset()                           { return this->_tileset; }
void               Tilemap::setTileset(const sf::Texture& tileset) { this->_tileset = tileset; }

void Tilemap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  states.transform *= this->getTransform();

  states.texture = &this->_tileset;

  target.draw(this->_vertices, states);
}

}
