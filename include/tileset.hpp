#ifndef LAZ_TILESET_HPP
#define LAZ_TILESET_HPP

#include <vector>
#include <SFML/Graphics/Texture.hpp>

#include "common.hpp"

namespace laz
{

class Tileset
{
public:
  // Forward declaration
  class TileData;

  Tileset();
  ~Tileset();

  void generate(const v2u& tileSize, const sf::Texture& texture, const std::vector<TileData> tileData);

  const v2u& getTileSize() const;
  const sf::Texture& getTexture() const;
  const std::vector<TileData>& getTileData() const;
  const TileData& getTileData(u16 tileID) const;

  struct TileData
  {
  public:
    enum class CollisionTypes : u8
    {
      None,
      Solid,
    };
    CollisionTypes collisionType;
  };

private:
  v2u _tileSize;
  sf::Texture _texture;
  std::vector<TileData> _tileData;
};

}

#endif
