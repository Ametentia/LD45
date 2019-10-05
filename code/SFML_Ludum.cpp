#if LUDUM_WINDOWS
    #include <windows.h>
#endif

#include <stdio.h>
#include <SFML/System.h>
#include <SFML/Graphics.h>

#include "Ludum.h"

global sfRenderWindow *global_window;
global sfView *global_view;
global v2 global_view_size;
global bool global_running;
global u32 global_window_width;
global u32 global_window_height;

#include "Ludum.cpp"
#include "Ludum_Sockets.cpp"

internal void SFMLProcessButton(Game_Button *current, Game_Button prev, bool pressed) {
    current->pressed = pressed;
    current->transitions = (current->pressed != prev.pressed) ? 1 : 0;
}

internal void SFMLGetInput(Game_Input *current, Game_Input *prev) {
    sfEvent event;
    while (sfRenderWindow_pollEvent(global_window, &event)) {
        switch (event.type) {
            case sfEvtClosed: { global_running = false; } break;
            case sfEvtResized: {
                global_window_width  = event.size.width;
                global_window_height = event.size.height;
            }
            break;
        }
    }

    // Mouse input
    sfVector2i mouse_pos = sfMouse_getPositionRenderWindow(global_window);
    current->mouse_position = sfRenderWindow_mapPixelToCoords(global_window, mouse_pos, global_view);

    for (u32 it = 0; it < sfMouseButtonCount; ++it) {
        SFMLProcessButton(&current->mouse_buttons[it], prev->mouse_buttons[it],
                sfMouse_isButtonPressed(cast(sfMouseButton) it));
    }

    // Keyboard
    Game_Controller *current_keyboard = &current->keyboard;
    Game_Controller *prev_keyboard = &prev->keyboard;
    SFMLProcessButton(&current_keyboard->card_slot_1, prev_keyboard->card_slot_1, sfKeyboard_isKeyPressed(sfKeyNum1));
    SFMLProcessButton(&current_keyboard->card_slot_2, prev_keyboard->card_slot_2, sfKeyboard_isKeyPressed(sfKeyNum2));
    SFMLProcessButton(&current_keyboard->card_slot_3, prev_keyboard->card_slot_3, sfKeyboard_isKeyPressed(sfKeyNum3));
    SFMLProcessButton(&current_keyboard->card_slot_4, prev_keyboard->card_slot_4, sfKeyboard_isKeyPressed(sfKeyNum4));
    SFMLProcessButton(&current_keyboard->card_slot_5, prev_keyboard->card_slot_5, sfKeyboard_isKeyPressed(sfKeyNum5));

    SFMLProcessButton(&current_keyboard->menu, prev_keyboard->menu, sfKeyboard_isKeyPressed(sfKeyEscape));
}

#if LUDUM_WINDOWS
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
#else
int main(int argc, char **argv) {
#endif

    // Create window
    sfVideoMode mode = { 1280, 720, 32 };
    //sfVideoMode mode = { 1920, 1080, 32 };
    global_window = sfRenderWindow_create(mode, "Ludum Dare 45", sfClose, 0);
    if (!global_window) {
        printf("[Fatal Error] Could not create SFML window\n");
        return 1;
    }

    global_view_size = V2(1920, 1080);
    global_view = sfView_create();
    sfView_setSize(global_view, global_view_size);
    sfView_setCenter(global_view, 0.5f * global_view_size);
    sfRenderWindow_setView(global_window, global_view);

    sfRenderWindow_setVerticalSyncEnabled(global_window, true);

    Game_Input inputs[2] = {};
    Game_Input *current_input = &inputs[0];
    Game_Input *prev_input = &inputs[1];

    Game_State __state = {};
    Game_State *state = &__state;

    state->board_texture = sfTexture_createFromFile("data/board.png", 0);
    state->card_texture = sfTexture_createFromFile("data/sap_life.png", 0);
    Assert(state->card_texture && state->board_texture);
    sfTexture_setSmooth(state->card_texture, true);
    sfTexture_generateMipmap(state->card_texture);
    sfTexture_setSmooth(state->board_texture, true);
    sfTexture_generateMipmap(state->board_texture);

    global_running = true;

    // Start timer
    sfClock *timer = sfClock_create();
    while (global_running) {
        // Poll events and get input
        SFMLGetInput(current_input, prev_input);

        // Update render game
        sfColor colour = { 0, 0, 0, 255 };
        sfRenderWindow_clear(global_window, colour);
        LudumUpdateRender(state, current_input);
        sfRenderWindow_display(global_window);

        // Check if still running and swap input
        global_running = (global_running && !current_input->requested_quit);
        Swap(current_input, prev_input);

        // Calculate delta time
        sfTime elapsed_time = sfClock_getElapsedTime(timer);
        current_input->delta_time = sfTime_asSeconds(elapsed_time);
        sfClock_restart(timer);
    }

    // Shutdown
    sfClock_destroy(timer);
    sfRenderWindow_close(global_window);
    sfRenderWindow_destroy(global_window);
}
