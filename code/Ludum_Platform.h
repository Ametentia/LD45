#if !defined(LUDUM_PLATFORM_H_)
#define LUDUM_PLATFORM_H_

struct Game_Button {
    bool pressed;
    u32 transitions;
};

union Game_Controller {
    struct {
        Game_Button card_slot_1;
        Game_Button card_slot_2;
        Game_Button card_slot_3;
        Game_Button card_slot_4;
        Game_Button card_slot_5;

        Game_Button menu;
    };

    Game_Button buttons[6];
};

struct Game_Input {
    f32 delta_time;
    bool requested_quit;

    Game_Controller keyboard;

    v2 mouse_position;
    Game_Button mouse_buttons[5];
};

inline bool IsPressed(Game_Button button) {
    bool result = (button.pressed);
    return result;
}

inline bool JustPressed(Game_Button button) {
    bool result = button.pressed && (button.transitions != 0);
    return result;
}

inline bool WasPressed(Game_Button button) {
    bool result = !button.pressed && (button.transitions != 0);
    return result;
}

#endif  // LUDUM_PLATFORM_H_
