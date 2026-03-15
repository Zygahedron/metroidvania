#include "tilemap.hpp"

#include "tileset.hpp"

namespace laz
{

Tilemap::Tilemap(const v2u& mapSize, const Tileset* tileset, const std::vector<u16>& tiles)
  : _mapSize(mapSize), _tiles(tiles), _vertices(), _tileset(tileset)
{
  this->_vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
  this->_vertices.resize(this->_mapSize.x * this->_mapSize.y * 6);

  for (u32 y = 0; y < this->_mapSize.y; y++)
  {
    for (u32 x = 0; x < this->_mapSize.x; x++)
    {
      const u16 tile = this->_tiles[x + (y * this->_mapSize.x)];
      const v2u tileSize = this->_tileset->getTileSize();
      const v2u tilesetSize = this->_tileset->getTexture().getSize();

      const i32 tu = tile % (tilesetSize.x / tileSize.x);
      const i32 tv = tile / (tilesetSize.x / tileSize.x);

      sf::Vertex* triangles = &this->_vertices[(x + (y * this->_mapSize.x)) * 6];

      triangles[0].position = v2f(x * tileSize.x, y * tileSize.y);
      triangles[1].position = v2f((x + 1) * tileSize.x, y * tileSize.y);
      triangles[2].position = v2f(x * tileSize.x, (y + 1) * tileSize.y);
      triangles[3].position = v2f(x * tileSize.x, (y + 1) * tileSize.y);
      triangles[4].position = v2f((x + 1) * tileSize.x, y * tileSize.y);
      triangles[5].position = v2f((x + 1) * tileSize.x, (y + 1) * tileSize.y);

      triangles[0].texCoords = v2f(tu * tileSize.x, tv * tileSize.y);
      triangles[1].texCoords = v2f((tu + 1) * tileSize.x, tv * tileSize.y);
      triangles[2].texCoords = v2f(tu * tileSize.x, (tv + 1) * tileSize.y);
      triangles[3].texCoords = v2f(tu * tileSize.x, (tv + 1) * tileSize.y);
      triangles[4].texCoords = v2f((tu + 1) * tileSize.x, tv * tileSize.y);
      triangles[5].texCoords = v2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
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

const Tileset& Tilemap::getTileset()                       { return *this->_tileset; }
void           Tilemap::setTileset(const Tileset* tileset) { this->_tileset = tileset; }

void Tilemap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  states.transform *= this->getTransform();

  states.texture = &this->_tileset->getTexture();

  target.draw(this->_vertices, states);
}

}
