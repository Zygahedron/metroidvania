using SFML.Graphics;
using SFML.System;

namespace metroidvania;

public class Tileset(Vector2u tileSize, Texture texture, List<Tileset.Tile> tileData)
{
    public Vector2u   TileSize { get; private init; } = tileSize;
    public Texture    Texture  { get; private init; } = texture;
    public List<Tile> TileData { get; private init; } = tileData;

    public class Tile(FloatRect rect, Tile.CollisionTypes collision = Tile.CollisionTypes.None)
    {
        public enum CollisionTypes : byte
        {
            None,
            Solid,
        };

        public FloatRect      Rect          { get; init; } = rect;
        public CollisionTypes CollisionType { get; init; } = collision;
    }
}
