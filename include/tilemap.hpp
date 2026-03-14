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

class Tilemap : public sf::Drawable, public sf::Transformable
{
public:
  Tilemap(const v2u& mapSize, const sf::Texture& tilemap, const std::vector<u16>& tiles);
  ~Tilemap();

  const v2u& getMapSize();
  void       setMapSize(const v2u& size);

  u16  getTile(u16 x, u16 y);
  void setTile(u16 x, u16 y, u16 tile);

  const sf::Texture& getTileset();
  void               setTileset(const sf::Texture& tileset);

private:
  v2u _mapSize;
  std::vector<u16> _tiles;

  sf::VertexArray _vertices;
  sf::Texture _tileset;

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

}

#endif
