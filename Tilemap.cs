using SFML.Graphics;
using SFML.System;

namespace metroidvania;

public class Tilemap : Transformable, IDrawable
{
    private VertexArray _vertices;

    public Vector2u        MapSize { get; private init; }
    public Tileset         Tileset { get; private init; }
    public Tileset.Tile[,] Tiles   { get; private init; }

    public Tilemap(Vector2u mapSize, Tileset tileset, Tileset.Tile[,] tiles)
    {
        this.MapSize = mapSize;
        this.Tileset = tileset;
        this.Tiles   = tiles;

        Vector2u tileSize = this.Tileset.TileSize;
        this._vertices = new VertexArray(PrimitiveType.Triangles, (uint)(this.Tiles.Length * 6));
        for (var y = 0; y < this.MapSize.Y; y++)
        for (var x = 0; x < this.MapSize.X; x++)
        {
            Tileset.Tile tile = this.Tiles[x, y];

            float tx = tile.Rect.Left;
            float ty = tile.Rect.Top;
            float tu = tile.Rect.Left + tile.Rect.Width;
            float tv = tile.Rect.Top  + tile.Rect.Height;

            this._vertices.Append(new Vertex(new Vector2f(x * tileSize.X, y * tileSize.Y),
                                             new Vector2f(tx,             ty)));
            this._vertices.Append(new Vertex(new Vector2f((x + 1) * tileSize.X, y * tileSize.Y),
                                             new Vector2f(tu,                   ty)));
            this._vertices.Append(new Vertex(new Vector2f(x * tileSize.X, (y + 1) * tileSize.Y),
                                             new Vector2f(tx,             tv)));
            this._vertices.Append(new Vertex(new Vector2f(x * tileSize.X, (y + 1) * tileSize.Y),
                                             new Vector2f(tx,             tv)));
            this._vertices.Append(new Vertex(new Vector2f((x + 1) * tileSize.X, y * tileSize.Y),
                                             new Vector2f(tu,                   ty)));
            this._vertices.Append(new Vertex(new Vector2f((x + 1) * tileSize.X, (y + 1) * tileSize.Y),
                                             new Vector2f(tu,                   tv)));
        }
    }

    public Vector2u GetTileCoordAtPosition(Vector2f position, bool ceiling = false)
    {
        Vector2f p = position - this.Position;
        float    x = p.X / this.Tileset.TileSize.X;
        float    y = p.Y / this.Tileset.TileSize.Y;
        if (ceiling)
        {
            x = MathF.Ceiling(x);
            y = MathF.Ceiling(y);
        }

        return new Vector2u((uint)MathF.Floor(x), (uint)MathF.Floor(y));
    }

    public void Draw(IRenderTarget target, RenderStates states)
    {
        states.Transform *= this.Transform;
        states.Texture   =  this.Tileset.Texture;
        target.Draw(this._vertices, states);
    }
}
