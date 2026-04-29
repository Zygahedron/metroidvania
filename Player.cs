using SFML.Graphics;
using SFML.System;

namespace metroidvania;

public class Player : Transformable, IDrawable
{
    private readonly FloatRect      _collider = new(new Vector2f(0f, 0f), new Vector2f(8f, 8f));
    private readonly RectangleShape _rectangle;
    
    private          bool     _grounded          = false;
    private          Vector2f _velocity          = new(0f, 0f);
    private readonly Vector2f _maxSpeed          = new(128f, 64f);
    private readonly float    _gravityMultiplier = 1f;
    private readonly float    _jumpStrength      = 200f;

    private float Acceleration => this._maxSpeed.X * 4f;
    private float Friction     => this._maxSpeed.X * 4f;

    public Player()
    {
        this._rectangle = new RectangleShape(this._collider.Size);
    }
    
    public void Update(Input input, Time deltaTime, Tilemap tilemap) {
        bool left  = input.IsButtonDown(Input.Buttons.Left);
        bool right = input.IsButtonDown(Input.Buttons.Right);
        bool down  = input.IsButtonDown(Input.Buttons.Down);
        bool jump  = input.IsButtonDown(Input.Buttons.Jump);

        float accelerationD = this.Acceleration * deltaTime.AsSeconds();
        float frictionD     = this.Friction     * deltaTime.AsSeconds();

        float horizontal = this._velocity.X;
        if (left && !right)
        {
            horizontal -= accelerationD;
            if (horizontal < -this._maxSpeed.X)
                horizontal = -this._maxSpeed.X;
        }
        else if (this._velocity.X < 0f)
        {
            horizontal += frictionD;
            if (horizontal > 0f)
                horizontal = 0f;
        }

        if (right && !left)
        {
            horizontal += accelerationD;
            if (horizontal > this._maxSpeed.X)
                horizontal = this._maxSpeed.X;
        }
        else if (this._velocity.X > 0f)
        {
            horizontal -= frictionD;
            if (horizontal < 0f)
                horizontal = 0f;
        }

        float vertical = this._velocity.Y;
        float gravityD = 600f * this._gravityMultiplier * deltaTime.AsSeconds();
        if (jump && this._grounded)
        {
            if (vertical > 0f)
                vertical = 0f;
            vertical       -= this._jumpStrength;
            gravityD       =  0f;
            this._grounded =  false;
        }
        else if (jump && !this._grounded && vertical < 0f)
        {
            gravityD *= 0.5f;
        }
        else if (down && !this._grounded)
        {
            gravityD *= 3f;
        }

        vertical += gravityD;

        this._velocity = new Vector2f(horizontal, vertical);

        horizontal *= deltaTime.AsSeconds();
        vertical   *= deltaTime.AsSeconds();

        while (horizontal > 0f)
        {
            float step = MathF.Min(horizontal, 1f);
            if (this.CheckTileCollision(tilemap, new Vector2f(step, 0f)))
            {
                horizontal       = 0f;
                this._velocity.X = 0f;
                Vector2f pos = this.Position;
                pos.X         = MathF.Ceiling(pos.X);
                this.Position = pos;
                break;
            }

            this.Position += new Vector2f(step, 0f);
            horizontal    -= step;
        }

        while (horizontal < 0f)
        {
            float step = MathF.Max(horizontal, -1f);
            if (this.CheckTileCollision(tilemap, new Vector2f(step, 0f)))
            {
                // horizontal       = 0f;
                this._velocity.X = 0f;
                Vector2f pos = this.Position;
                pos.X         = MathF.Floor(pos.X);
                this.Position = pos;
                break;
            }

            this.Position += new Vector2f(step, 0f);
            horizontal    -= step;
        }

        while (vertical > 0f)
        {
            float step = MathF.Min(vertical, 1f);
            if (this.CheckTileCollision(tilemap, new Vector2f(0f, step)))
            {
                vertical         = 0f;
                this._velocity.Y = 0f;
                Vector2f pos = this.Position;
                pos.Y          = MathF.Ceiling(pos.Y);
                this.Position  = pos;
                this._grounded = true;
                break;
            }
            this.Position += new Vector2f(0f, step);
            vertical      -= step;
        }

        while (vertical < 0f)
        {
            float step = MathF.Max(vertical, -1f);
            if (this.CheckTileCollision(tilemap, new Vector2f(0f, step)))
            {
                // vertical         = 0f;
                this._velocity.Y = 0f;
                Vector2f pos = this.Position;
                pos.Y         = MathF.Floor(pos.Y);
                this.Position = pos;
                break;
            }

            this.Position += new Vector2f(0f, step);
            vertical      -= step;
        }
    }

    private bool CheckTileCollision(Tilemap tilemap, Vector2f offset = new())
    {
        IntRect tileRect = this.GetTileRect(tilemap, offset);
        for (uint y = 0; y < tileRect.Size.Y; y++)
        for (uint x = 0; x < tileRect.Size.X; x++)
        {
            Vector2u         t        = new((uint)(tileRect.Position.X + x), (uint)(tileRect.Position.Y + y));
            Tileset.Tile tileData = tilemap.Tiles[t.X, t.Y];
            if (tileData.CollisionType == Tileset.Tile.CollisionTypes.Solid)
                return true;
        }

        return false;
    }


    private IntRect GetTileRect(Tilemap tilemap, Vector2f offset)
    {
        Vector2f size = this._collider.Size;
        Vector2f tl   = this.Position + offset;
        Vector2f br   = tl            + size;

        Vector2u tlTile = tilemap.GetTileCoordAtPosition(tl);
        Vector2u brTile = tilemap.GetTileCoordAtPosition(br, true);

        Vector2u tileSize = brTile - tlTile;

        return new IntRect((Vector2i)tlTile, (Vector2i)tileSize);
    }

    public void Draw(IRenderTarget target, RenderStates states)
    {
        states.Transform *= this.Transform;
        target.Draw(this._rectangle, states);
    }
}
