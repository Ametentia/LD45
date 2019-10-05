#include "Ludum_Assets.cpp"

internal u32 RandomChoice(u32 choices) {
    u32 result = (rand() % choices);
    return result;
}

internal void LudumUpdateRender(Game_State *state, Game_Input *input) {
    if (!state->initialised) {
        char *card_names[] = {
            "data/black_hole.png",
            "data/bound_by_time.png",
            "data/charity.png",
            "data/devils_wheel.png",
            "data/lucky_feeling.png",
            "data/playing_dumb.png",
            "data/rewind.png",
            "data/sacrifice.png",
            "data/sap_life.png",
            "data/time_bomb.png",
            "data/worm_hole.png",
            "data/best_intentions.png"
        };

        for (u32 it = 0; it < ArrayCount(card_names); ++it) {
            state->card_images[it] = LoadImage(&state->assets, card_names[it]);
            Assert(IsValid(state->card_images[it]));
        }

        state->board_image = LoadImage(&state->assets, "data/board.png");
        Assert(IsValid(state->board_image));

        Player *player = &state->players[0];
        for (u32 it = 0; it < ArrayCount(player->cards); ++it) {
            Card *card = &player->cards[it];
            card->image = state->card_images[RandomChoice(ArrayCount(card_names))];
        }

        player->card_count = 10;

        state->initialised = true;
    }

    Game_Controller *keyboard = &input->keyboard;

    sfRectangleShape *board = sfRectangleShape_create();
    sfRectangleShape_setPosition(board, V2(0, 0));
    sfRectangleShape_setSize(board, global_view_size);
    sfRectangleShape_setTexture(board, GetImage(&state->assets, state->board_image), true);

    sfRenderWindow_drawRectangleShape(global_window, board, 0);
    sfRectangleShape_destroy(board);

    sfVector2u image_size = sfTexture_getSize(GetImage(&state->assets, state->card_images[0]));
    v2 size = V2(0.5f * image_size.x, 0.5f * image_size.y);

    sfRectangleShape *shape = sfRectangleShape_create();
    sfRectangleShape_setSize(shape, size);
    sfRectangleShape_setOrigin(shape, V2(size.x * 0.5f, size.y));

    v2 position = V2(global_view_size.x * 0.5f, global_view_size.y - (0.25f * size.y));
    sfRectangleShape_setPosition(shape, position);

    Player *player = &state->players[0];

    if (JustPressed(keyboard->card_slot_3)) {
        u32 cc = player->card_count;
        cc = (cc + 1) % 11;
        player->card_count = cc;
    }

    u32 card_count = player->card_count;
    u32 passes = 0;
    f32 starting_offset = -40;
    if (card_count & 1) {
        starting_offset = 60;
       passes = (card_count - 1) / 2;
    }
    else { passes = (card_count / 2); }

    Card_Transform transforms[card_count];

    // @Note: These are used to configure the card fan
    f32 angle_offset = 5;
    v2  position_offset = V2(80, 0); // V2(80, 20);

    // Check for mouse hover
    s32 running_card_index = 0;
    s32 highest_hovered_card = -1;
    v2 mouse_pos = input->mouse_position;

    // Left hand side of the fan
    for (s32 it = passes; it > 0; --it) {
        Card_Transform *tx = &transforms[running_card_index];

        tx->angle = Radians(-angle_offset * it);
        tx->offset = V2(-starting_offset - position_offset.x * it, -position_offset.y * ((passes - it) + 1));

        v2 axes = V2(Sin(tx->angle), -Cos(tx->angle));

        tx->offset += ((it + 1) * -20.0f) * axes;

        v2 centre = position + tx->offset + (0.5f * size.y) * axes;
        v2 rotated_mouse = V2(
                Cos(-tx->angle) * (mouse_pos.x - centre.x) - Sin(-tx->angle) * (mouse_pos.y - centre.y),
                Sin(-tx->angle) * (mouse_pos.x - centre.x) + Cos(-tx->angle) * (mouse_pos.y - centre.y));

        if (Abs(rotated_mouse.x) < (size.x * 0.5f) && Abs(rotated_mouse.y) < (size.y * 0.5f)) {
            highest_hovered_card = running_card_index;
        }

        running_card_index++;
    }

    local f32 test = 80.0f;
    if (JustPressed(keyboard->card_slot_4)) { test += 10; }
    else if (JustPressed(keyboard->card_slot_5)) { test -= 10; }

    // Centre card if there are an odd number
    if (card_count & 1) {
        Card_Transform *tx = &transforms[running_card_index];

        tx->angle = 0;
        tx->offset = V2(0, 30);

        v2 centre = position + tx->offset - V2(0, 0.5f * size.y);
        v2 rotated_mouse = mouse_pos - centre;

        if (Abs(rotated_mouse.x) < (size.x * 0.5f) && Abs(rotated_mouse.y) < (size.y * 0.5f)) {
            highest_hovered_card = running_card_index;
        }

        running_card_index++;
    }

    // Right hand side of the fan
    for (u32 it = 1; it <= passes; ++it) {
        Card_Transform *tx = &transforms[running_card_index];

        tx->angle = Radians(angle_offset * it);
        tx->offset = V2(starting_offset + position_offset.x * it, -position_offset.y * ((passes - it) + 1));

        v2 axes = V2(Sin(tx->angle), -Cos(tx->angle));

        tx->offset += ((it + 1) * -20.0f) * axes;

        v2 centre = position + tx->offset + (0.5f * size.y) * axes;
        v2 rotated_mouse = V2(
                Cos(-tx->angle) * (mouse_pos.x - centre.x) - Sin(-tx->angle) * (mouse_pos.y - centre.y),
                Sin(-tx->angle) * (mouse_pos.x - centre.x) + Cos(-tx->angle) * (mouse_pos.y - centre.y));

        if (Abs(rotated_mouse.x) < (size.x * 0.5f) && Abs(rotated_mouse.y) < (size.y * 0.5f)) {
            highest_hovered_card = running_card_index;
        }

        running_card_index++;
    }

    // Render Card Fan

    // @Hack: Just to get the dragging index instead of highest hover to prevent if from being rendererd
    if (state->dragging) { highest_hovered_card = state->dragging_index; }

    running_card_index = 0;
    // Left hand side of the fan
    for (s32 it = passes; it > 0; --it) {
        if (running_card_index != highest_hovered_card) {
            Card *card = &player->cards[running_card_index];
            Card_Transform *tx = &transforms[running_card_index];

            sfRectangleShape_setPosition(shape, position + tx->offset);
            sfRectangleShape_setRotation(shape, Degrees(tx->angle));

            sfRectangleShape_setTexture(shape, GetImage(&state->assets, card->image), true);

            sfRenderWindow_drawRectangleShape(global_window, shape, 0);

            v2 axes = (0.5f * size.y) * V2(Sin(tx->angle), -Cos(tx->angle));
            v2 centre = position + tx->offset + axes;

            sfCircleShape *c_shape = sfCircleShape_create();
            sfCircleShape_setRadius(c_shape, 10);
            sfCircleShape_setOrigin(c_shape, V2(10, 10));
            sfCircleShape_setFillColor(c_shape, sfMagenta);
            sfCircleShape_setPosition(c_shape, centre);
            sfRenderWindow_drawCircleShape(global_window, c_shape, 0);

            sfRectangleShape *bbox = sfRectangleShape_create();
            sfRectangleShape_setOrigin(bbox, 0.5f * size);
            sfRectangleShape_setSize(bbox, size);
            sfRectangleShape_setPosition(bbox, centre);
            sfRectangleShape_setFillColor(bbox, sfTransparent);
            sfRectangleShape_setOutlineColor(bbox, sfBlue);
            sfRectangleShape_setOutlineThickness(bbox, 3);
            sfRectangleShape_setRotation(bbox, Degrees(tx->angle));

            sfRenderWindow_drawRectangleShape(global_window, bbox, 0);

            sfRectangleShape_destroy(bbox);

            sfCircleShape_destroy(c_shape);
        }

        running_card_index++;
    }

    // Centre card if there is one
    if (card_count & 1) {
        if (running_card_index != highest_hovered_card) {
            Card *card = &player->cards[running_card_index];
            Card_Transform *tx = &transforms[running_card_index];

            sfRectangleShape_setRotation(shape, 0);
            sfRectangleShape_setPosition(shape, position + tx->offset);
            sfRectangleShape_setTexture(shape, GetImage(&state->assets, card->image), true);
            sfRenderWindow_drawRectangleShape(global_window, shape, 0);

            v2 centre = position + tx->offset - V2(0, 0.5f * size.y);

            sfCircleShape *c_shape = sfCircleShape_create();
            sfCircleShape_setRadius(c_shape, 10);
            sfCircleShape_setOrigin(c_shape, V2(10, 10));
            sfCircleShape_setFillColor(c_shape, sfMagenta);
            sfCircleShape_setPosition(c_shape, centre);
            sfRenderWindow_drawCircleShape(global_window, c_shape, 0);

            sfRectangleShape *bbox = sfRectangleShape_create();
            sfRectangleShape_setOrigin(bbox, 0.5f * size);
            sfRectangleShape_setSize(bbox, size);
            sfRectangleShape_setPosition(bbox, centre);
            sfRectangleShape_setFillColor(bbox, sfTransparent);
            sfRectangleShape_setOutlineColor(bbox, sfBlue);
            sfRectangleShape_setOutlineThickness(bbox, 3);
            sfRectangleShape_setRotation(bbox, Degrees(tx->angle));

            sfRenderWindow_drawRectangleShape(global_window, bbox, 0);

            sfRectangleShape_destroy(bbox);
            sfCircleShape_destroy(c_shape);
        }

        running_card_index++;
    }

    // Right hand side of the fan
    for (s32 it = 1; it <= passes; ++it) {
        if (running_card_index != highest_hovered_card) {
            Card *card = &player->cards[running_card_index];
            Card_Transform *tx = &transforms[running_card_index];

            v2 axes = (0.5f * size.y) * V2(Sin(tx->angle), -Cos(tx->angle));
            v2 centre = position + tx->offset + axes;

            sfRectangleShape_setPosition(shape, position + tx->offset);
            sfRectangleShape_setRotation(shape, Degrees(tx->angle));

            sfRectangleShape_setTexture(shape, GetImage(&state->assets, card->image), true);

            sfRenderWindow_drawRectangleShape(global_window, shape, 0);

            sfCircleShape *c_shape = sfCircleShape_create();
            sfCircleShape_setRadius(c_shape, 10);
            sfCircleShape_setOrigin(c_shape, V2(10, 10));
            sfCircleShape_setFillColor(c_shape, sfMagenta);
            sfCircleShape_setPosition(c_shape, centre);
            sfRenderWindow_drawCircleShape(global_window, c_shape, 0);

            sfRectangleShape *bbox = sfRectangleShape_create();
            sfRectangleShape_setOrigin(bbox, 0.5f * size);
            sfRectangleShape_setSize(bbox, size);
            sfRectangleShape_setPosition(bbox, centre);
            sfRectangleShape_setFillColor(bbox, sfTransparent);
            sfRectangleShape_setOutlineColor(bbox, sfBlue);
            sfRectangleShape_setOutlineThickness(bbox, 3);
            sfRectangleShape_setRotation(bbox, Degrees(tx->angle));

            sfRenderWindow_drawRectangleShape(global_window, bbox, 0);

            sfRectangleShape_destroy(bbox);

            sfCircleShape_destroy(c_shape);
        }

        running_card_index++;
    }


    Assert(running_card_index <= 10);

    // Show the card that is being hovered
    if (!state->dragging && highest_hovered_card != -1) {
        Card *card = &player->cards[highest_hovered_card];
        Card_Transform *tx = &transforms[highest_hovered_card];

        if (JustPressed(input->mouse_buttons[0])) {
            state->dragging = true;
            state->dragging_index = highest_hovered_card;
        }
        else {
            sfRectangleShape_setOrigin(shape, 0.5f * size);
            sfRectangleShape_setSize(shape, 2 * size);

            sfRectangleShape_setOrigin(shape, V2(0.5f * size.x, 2 * size.y));
            sfRectangleShape_setPosition(shape, position + tx->offset - V2(100, 0));
            sfRectangleShape_setRotation(shape, 0);
            sfRectangleShape_setTexture(shape, GetImage(&state->assets, card->image), true);
            sfRenderWindow_drawRectangleShape(global_window, shape, 0);
        }
    }

    if (state->dragging) {
        Card *card = &player->cards[highest_hovered_card];

        sfRectangleShape_setOrigin(shape, size);
        sfRectangleShape_setRotation(shape, 0);
        sfRectangleShape_setSize(shape, 2 * size);
        sfRectangleShape_setPosition(shape, input->mouse_position);
        sfRectangleShape_setTexture(shape, GetImage(&state->assets, card->image), true);
        sfRenderWindow_drawRectangleShape(global_window, shape, 0);

        if (!IsPressed(input->mouse_buttons[0])) {
            state->dragging = false;
            state->dragging_index = -1;

            if (input->mouse_position.y < (0.5f * global_view_size.y)) {
                printf("[Info] Player used card!\n");

                for (u32 it = highest_hovered_card; it < player->card_count - 1; ++it) {
                    Card c = player->cards[it + 1];
                    player->cards[it] = c;
                }

                player->card_count--;
            }
        }
    }

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
