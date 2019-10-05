#if !defined(LUDUM_H_)
#define LUDUM_H_

#include "Ludum_Types.h"
#include "Ludum_Platform.h"
#include "Ludum_Maths.h"
#include "Ludum_Sockets.h"

#include "Ludum_Assets.h"
struct Card_Transform {
    v2 offset;
    f32 angle;
};

struct Card {
    Image_Handle image;
    Sound_Handle sound;

    u32 cost;
    u32 damage;
    u32 effect_flags;
};

struct Player {
    u32 card_count;
    Card cards[10];
};

struct Game_State {
    bool initialised;
    Asset_Manager assets;

    Image_Handle board_image;
    Image_Handle card_images[25];

    Player players[2];
    s32 dragging_index;
    bool dragging;
};

#endif  // LUDUM_H_
