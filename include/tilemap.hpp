#ifndef LAZ_TILEMAP_HPP
#define LAZ_TILEMAP_HPP

#include <vector>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include "common.hpp"
#include "tileset.hpp"

namespace laz
{

class Tilemap : public sf::Drawable, public sf::Transformable
{
public:
  Tilemap(const v2u& mapSize, const Tileset* tileset, const std::vector<std::vector<u16>>& tiles);
  ~Tilemap();

  const v2u& getMapSize() const;
  void       setMapSize(const v2u& size);

  u16  getTile(u16 x, u16 y) const;
  void setTile(u16 x, u16 y, u16 tile);

  v2<u16> getTileCoordAtPosition(v2f pos, bool ceiling = false) const;
  u16 getTileAtPosition(v2f pos) const;

  const Tileset::TileData& getTileData(u16 x, u16 y) const;

  const Tileset& getTileset() const;
  void           setTileset(const Tileset* tileset);

private:
  v2u _mapSize;
  std::vector<std::vector<u16>> _tiles;

  sf::VertexArray _vertices;
  const Tileset* _tileset;

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

}

#endif
