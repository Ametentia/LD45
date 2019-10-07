#if !defined(LUDUM_H_)
#define LUDUM_H_

#include "Ludum_Types.h"
#include "Ludum_Platform.h"
#include "Ludum_Maths.h"
#include "Ludum_Sockets.h"

#include "Ludum_Assets.h"
struct Card_Transform {
    v2 offset;
    v2 axes;
    f32 angle;
};

// @Note: These will be function pointers to exectue card specific commands, if a card has no effect then it
// doesn't need a pointer
// @Note: These will return true if the card was successfully used, otherwise false
struct Player;
struct Board_State;
#define CARD_EFFECT(name) void name(Board_State *state, Player *player, Player *enemy)
typedef CARD_EFFECT(Card_Effect);

enum Card_Status_Flags {
    CardStatus_Hidden = 0x1, // Player can't see this card (The back will be shown instead)
    CardStatus_Frozen = 0x2, // Player can't use this card
    CardStatus_Selected = 0x4, // Selected for card transfer (i.e. in Altruism etc.)
};

enum Card_Type {
    CardType_Back = 0,
    CardType_AlternateReality,
    CardType_Altruism,
    CardType_BallAndChain,
    CardType_BestIntentions,
    CardType_BlackHole,
    CardType_BloodPact,
    CardType_BoundByTime,
    CardType_Charity,
    CardType_DarkRitual,
    CardType_DarknessOfSpace,
    CardType_DevilsWheel,
    CardType_EventHorizon,
    CardType_ForcedSalvation,
    CardType_GravitationalPull,
    CardType_LuckyFeeling,
    CardType_NostalgiaTrip,
    CardType_OwnInterests,
    CardType_Pebbles,
    CardType_PlayingDumb,
    CardType_Premonition,
    CardType_PureAnarchy,
    CardType_Recursion,
    CardType_Rewind,
    CardType_Sacrifice,
    CardType_SapLife,
    CardType_SpaceDebris,
    CardType_TimeBomb,
    CardType_TimeHealsAllWounds,
    CardType_UnendingTorment,
    CardType_WormHole,

    CardType_Count
};

struct Card {
    Card_Type type; // @Note: For integrity checking

    Image_Handle image;
    Sound_Handle sound;

    u32 cost;
    u8 hidden_turn_count; // How many turns CardStatus_Hidden lasts for
    u8 frozen_turn_count; // How many turns CardStatus_Frozen lasts for
    u16 effect_flags;

    Card_Effect *Execute;
};

// This contains a list of all of the cards
struct Card_Index {
    Card cards[CardType_Count];
};

enum Player_State_Flags {
    PlayerState_Selecting = 0x1,
    PlayerState_ViewingHand = 0x2,
    PlayerState_ViewingDraw = 0x4,
    PlayerState_RemovingHand = 0x8,
    PlayerState_ToEnemy = 0x10,
    PlayerState_FromEnemy = 0x20,
    PlayerState_ViewingCards = (PlayerState_Selecting | PlayerState_ViewingHand | PlayerState_ViewingDraw)
};

enum Player_Type {
    PlayerType_Human = 0,
    PlayerType_Computer,
};

struct Player {
    Player_Type type;

    s32 health;
    s32 last_turn_health;

    u32 mana;
    u32 max_mana;

    u32 state_flags;
    u32 select_card_count;
    u32 current_selected_count;

    // @Note: These are Card_Status_Flags that are placed on newly drawn cards. They are cleared after the
    // draw phase of the players turn
    // @Note: These are always implicitly single turn attributes.. but may change depending on card effects
    u32 new_card_flags;

    u32 next_draw_count;
    u32 card_count;
    Card cards[30]; // @Todo: This may be better as a linked list

    u32 next_card_index; // @Note: The next card to draw
    s32 cards_left;
    Card deck[24];

    u32 turn_used_count;
    u32 last_turn_used_count;
    u32 graveyard_next;
    Card graveyard[100];

    u32 active_card_count;
    Card_Type active_cards[20];
};

enum Turn_Phase {
    TurnPhase_Draw = 0,
    TurnPhase_Play,
    TurnPhase_End
};

struct Board_State {
    u32 current_player_turn;
    Turn_Phase phase;

    s32 damage_last_turn; // Copied from damage_this_turn
    s32 sacrifice_damage_last_turn;

    s32 sacrifice_damage_this_turn;
    s32 damage_this_turn;

    Card_Index *card_index;
};

struct Game_State {
    bool initialised;
    Asset_Manager assets;

    Card_Index card_index;
    Board_State board;

    Image_Handle glow_image;
    Image_Handle shadow_image;
    Image_Handle board_image;

    Font_Handle system_font;

    // @Note: There are always 2 players
    Player *players;
    s32 dragging_index;
    bool dragging;
};


#endif  // LUDUM_H_
