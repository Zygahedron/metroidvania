using SFML.System;
using SFML.Window;

namespace metroidvania;

public class Input
{
    public enum Buttons : byte
    {
        Left,
        Down,
        Up,
        Right,
        Confirm,
        Cancel,
        Jump,
        Attack,
        Menu,
    };

    private readonly Dictionary<Buttons, ButtonState>        _buttonStates       = new();
    private readonly Dictionary<Keyboard.Key, List<Buttons>> _keyboardBindings   = new();
    private readonly Dictionary<uint, List<Buttons>>         _controllerBindings = new();
    
    private uint _controllerID = 0;
    
    public Input()
    {
        foreach (Buttons btn in Enum.GetValues<Buttons>())
        {
            this._buttonStates.Add(btn, new ButtonState());
        }
    }

    public bool LoadBindingsFromYAML(string filepath)
    {
        if (!File.Exists(filepath))
        {

        }

        return false;
    }

    public bool IsButtonDown(Buttons button)
    {
        ButtonState.States state = this._buttonStates[button].State;
        return state is ButtonState.States.JustPressed or ButtonState.States.Held;
    }

    public ButtonState.States GetButtonState(Buttons button) => this._buttonStates[button].State;

    public void SetButtonState(Buttons button, ButtonState.States state)
    {
        ButtonState fullState = this._buttonStates[button];
        fullState.State = state;
        if (state == ButtonState.States.NotPressed)
            fullState.HeldTimer = Time.Zero;
    }

    public void HandleButtonEvent(Keyboard.Key key, bool isPressed)
    {
        if (!this._keyboardBindings.TryGetValue(key, out List<Buttons>? bindings))
            return;
        this.HandleButtonEvent(bindings, isPressed);
    }

    public void HandleButtonEvent(uint buttonID, bool isPressed)
    {
        if (!this._controllerBindings.TryGetValue(buttonID, out List<Buttons>? bindings))
            return;
        this.HandleButtonEvent(bindings, isPressed);
    }

    public void Update(Time deltaTime)
    {
        foreach ((Buttons button, ButtonState state) in this._buttonStates)
        {
            switch (state.State)
            {
            case ButtonState.States.JustPressed:
                this.SetButtonState(button, ButtonState.States.Held);
                break;
            case ButtonState.States.Held:
                state.HeldTimer += deltaTime;
                break;
            case ButtonState.States.JustReleased:
                this.SetButtonState(button, ButtonState.States.NotPressed);
                break;
            }
        }
    }

    public bool ConsumePress(Buttons button)
    {
        ButtonState state = this._buttonStates[button];
        if (state.State != ButtonState.States.JustPressed)
            return false;

        this.SetButtonState(button, ButtonState.States.Held);
        return true;
    }

    public bool ConsumeRelease(Buttons button, Time heldThreshold)
    {
        ButtonState state = this._buttonStates[button];
        if (state.State != ButtonState.States.JustReleased)
            return false;

        this.SetButtonState(button, ButtonState.States.NotPressed);
        return true;
    }

    private void HandleButtonEvent(List<Buttons> bindings, bool isPressed)
    {
        foreach (Buttons btn in bindings)
        {
            if (!this._buttonStates.ContainsKey(btn))
                this._buttonStates.Add(btn, new ButtonState());

            ButtonState state = this._buttonStates[btn];
            if (isPressed)
            {
                state.PressedCount++;
                if (state.State == ButtonState.States.NotPressed)
                    this.SetButtonState(btn, ButtonState.States.JustPressed);
            }
            else if (state.PressedCount > 0)
            {
                state.PressedCount--;
                if (state.PressedCount == 0)
                    this.SetButtonState(btn, ButtonState.States.JustReleased);
            }
        }
    }

    public class ButtonState
    {
        public enum States : byte
        {
            NotPressed,
            JustPressed,
            Held,
            JustReleased,
        };

        public States State        = States.NotPressed;
        public Time   HeldTimer    = Time.Zero;
        public byte   PressedCount = 0;
    };

}
