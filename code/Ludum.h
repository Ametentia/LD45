#if !defined(LUDUM_H_)
#define LUDUM_H_

#include "Ludum_Types.h"
#include "Ludum_Platform.h"
#include "Ludum_Maths.h"
#include "Ludum_Sockets.h"

struct Game_State {
    sfTexture *board_texture;
    sfTexture *card_texture;
};

#endif  // LUDUM_H_
