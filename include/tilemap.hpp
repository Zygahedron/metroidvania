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

namespace laz
{
class Tileset;

class Tilemap : public sf::Drawable, public sf::Transformable
{
public:
  Tilemap(const v2u& mapSize, const Tileset* tileset, const std::vector<u16>& tiles);
  ~Tilemap();

  const v2u& getMapSize();
  void       setMapSize(const v2u& size);

  u16  getTile(u16 x, u16 y);
  void setTile(u16 x, u16 y, u16 tile);

  const Tileset& getTileset();
  void           setTileset(const Tileset* tileset);

private:
  v2u _mapSize;
  std::vector<u16> _tiles;

  sf::VertexArray _vertices;
  const Tileset* _tileset;

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

}

#endif
