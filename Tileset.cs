using SFML.Graphics;
using SFML.System;

namespace metroidvania;

public class Tileset(Vector2u tileSize, Texture texture, List<Tileset.Tile> tileData)
{
    public Vector2u   TileSize { get; private init; } = tileSize;
    public Texture    Texture  { get; private init; } = texture;
    public List<Tile> TileData { get; private init; } = tileData;

    public static Tileset Load(string filepath)
    {
        
    }

    public class Tile(string name, Vector2f texturePosition, Tile.CollisionTypes collision = Tile.CollisionTypes.None)
    {
        public enum CollisionTypes : byte
        {
            None,
            Solid,
        }

        public string         Name            { get; } = name;
        public Vector2f       TexturePosition { get; } = texturePosition;
        public CollisionTypes CollisionType   { get; } = collision;
    }
}
