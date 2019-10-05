internal void LudumUpdateRender(Game_State *state, Game_Input *input) {
    sfRectangleShape *board = sfRectangleShape_create();
    sfRectangleShape_setPosition(board, V2(0, 0));
    sfRectangleShape_setSize(board, global_view_size);
    sfRectangleShape_setTexture(board, state->board_texture, true);

    sfRenderWindow_drawRectangleShape(global_window, board, 0);
    sfRectangleShape_destroy(board);

    sfRectangleShape *shape = sfRectangleShape_create();
    v2 size = V2(200, 300); // @Todo: Get actual texture size
    sfRectangleShape_setSize(shape, size);
    sfRectangleShape_setOrigin(shape, V2(size.x * 0.5f, size.y));
    sfRectangleShape_setTexture(shape, state->card_texture, true);

    v2 position = V2(global_view_size.x * 0.5f, global_view_size.y - (0.25f * size.y));
    sfRectangleShape_setPosition(shape, position);
    for (s32 it = 0; it < 5; ++it) {
        f32 angle = -30 + (15 * it);
        v2 offset = V2(-60 + (it * 30), 0);
        if (it == 4) {
            sfRectangleShape_setPosition(shape, input->mouse_position);
        }
        else {
            sfRectangleShape_setPosition(shape, position + offset);
        }
        sfRectangleShape_setRotation(shape, angle);

        v2 pos = sfRectangleShape_getPosition(shape);
        v2 origin = sfRectangleShape_getOrigin(shape);
        sfCircleShape *c_shape = sfCircleShape_create();
        sfCircleShape_setRadius(c_shape, 10);
        sfCircleShape_setOrigin(c_shape, V2(10, 10));
        sfCircleShape_setFillColor(c_shape, { 255, 0, 0, 255 });

        f32 rad_angle = Radians(angle);
        v2 axes = 150 * V2(Sin(rad_angle), -Cos(rad_angle));

        sfCircleShape_setPosition(c_shape, pos + axes);

        sfRenderWindow_drawRectangleShape(global_window, shape, 0);
        sfRenderWindow_drawCircleShape(global_window, c_shape, 0);

        sfCircleShape_destroy(c_shape);
    }



    Game_Controller *keyboard = &input->keyboard;
    if (IsPressed(keyboard->menu)) { input->requested_quit = true; }

    if (JustPressed(keyboard->card_slot_1)) {
        sfVector2u size = { 1600, 900 };
        sfRenderWindow_setSize(global_window, size);
    }

    if (JustPressed(keyboard->card_slot_2)) {
        sfVector2u size = { 1280, 720 };
        sfRenderWindow_setSize(global_window, size);
    }

    sfRectangleShape_destroy(shape);
}
