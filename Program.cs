using SFML.Graphics;
using SFML.System;
using SFML.Window;

namespace metroidvania;

internal static class Program
{
    private static readonly Vector2u Resolution      = new(640, 360);
    private const           uint     ResolutionScale = 2;

    private static readonly RenderWindow Window = new(
        new VideoMode(Program.Resolution * Program.ResolutionScale),
        "metroidvania",
        Styles.Titlebar | Styles.Close,
        State.Windowed
    );

    private static readonly View View = new();

    private static void Main()
    {
        Program.Window.SetKeyRepeatEnabled(false);
        Program.Window.SetView(Program.View);
        Program.Window.SetFramerateLimit(60);
        Console.WriteLine("Hello, World!");
    }
}
