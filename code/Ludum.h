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
    CardType_Barrier,
    CardType_BestIntentions,
    CardType_BlackHole,
    CardType_BloodPact,
    CardType_BoundByTime,
    CardType_Charity,
    CardType_CompleteMeltdown,
    CardType_DarkRitual,
    CardType_DarknessOfSpace,
    CardType_DazzlingLight,
    CardType_DevilsWheel,
    CardType_Emc2,
    CardType_EmptyManaCrystal,
    CardType_EnergyInefficient,
    CardType_EventHorizon,
    CardType_FootingTheBill,
    CardType_ForcedSalvation,
    CardType_GravitationalPull,
    CardType_LuckyFeeling,
    CardType_Malfunction,
    CardType_ManaErosion,
    CardType_ManaStarved,
    CardType_MiracleMachine,
    CardType_NostalgiaTrip,
    CardType_NotPlayingAlong,
    CardType_Overdrive,
    CardType_OverflowingEnergy,
    CardType_OwnInterests,
    CardType_Pebbles,
    CardType_PlayingDumb,
    CardType_PowerSurge,
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

    f32 card_use_time;

    s32 temp_mana_change;
    s32 mana;
    s32 max_mana;

    bool set_attack_mod; // @Hack: For resetting back to 1
    f32 attack_modifier; // Usually 1, 2 for complete meltdown
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
    Card deck[40];

    u32 turn_used_count;
    u32 last_turn_used_count;
    u32 graveyard_next;
    Card graveyard[100];

    u32 active_card_count;
    Card_Type active_cards[20];

    bool has_card;
    f32 used_card_display_time;
    f32 used_card_display_y_offset;
    Image_Handle used_card_image;

    f32 clear_out_time;
    bool cleared_out;
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

    bool in_menu;

    Card_Index card_index;
    Board_State board;

    Image_Handle inverted_start_image;
    Image_Handle start_image;
    Image_Handle menu_image;
    Image_Handle confirm_image;
    Image_Handle mana_image;
    Image_Handle end_turn_image;
    Image_Handle glow_image;
    Image_Handle shadow_image;
    Image_Handle board_image;

    Font_Handle system_font;

    v2 confim_button_size;

    // @Note: There are always 2 players
    Player *players;
    s32 dragging_index;
    bool dragging;

    v2 end_turn_pos;
    v2 end_turn_size;
};

// @Todo: List the decks.. Then we can copy them into players decks and shuffle
const static Card_Type energy_deck[] = {
    CardType_Barrier,
    CardType_Barrier,
    CardType_CompleteMeltdown,
    CardType_CompleteMeltdown,
    CardType_DazzlingLight,
    CardType_DazzlingLight,
    CardType_Emc2,
    CardType_Emc2,
    CardType_EnergyInefficient,
    CardType_EnergyInefficient,
    CardType_FootingTheBill,
    CardType_FootingTheBill,
    CardType_Malfunction,
    CardType_Malfunction,
    CardType_ManaErosion,
    CardType_ManaErosion,
    CardType_ManaStarved,
    CardType_ManaStarved,
    CardType_MiracleMachine,
    CardType_MiracleMachine,
    CardType_NotPlayingAlong,
    CardType_NotPlayingAlong,
    CardType_Overdrive,
    CardType_Overdrive,
    CardType_OverflowingEnergy,
    CardType_PowerSurge,
    CardType_PowerSurge,
    CardType_Sacrifice,
    CardType_Sacrifice,
    CardType_Sacrifice,
    CardType_Sacrifice,
    CardType_Sacrifice,
    CardType_Sacrifice,
    CardType_Sacrifice,
    CardType_Sacrifice
};

const static Card_Type time_and_space_deck[] = {
    CardType_AlternateReality,
    CardType_AlternateReality,
    CardType_BlackHole,
    CardType_BlackHole,
    CardType_BoundByTime,
    CardType_BoundByTime,
    CardType_DarknessOfSpace,
    CardType_DarknessOfSpace,
    CardType_EventHorizon,
    CardType_EventHorizon,
    CardType_GravitationalPull,
    CardType_GravitationalPull,
    CardType_NostalgiaTrip,
    CardType_NostalgiaTrip,
    CardType_Premonition,
    CardType_Premonition,
    CardType_Recursion,
    CardType_Recursion,
    CardType_Rewind,
    CardType_Rewind,
    CardType_SpaceDebris,
    CardType_SpaceDebris,
    CardType_TimeBomb,
    CardType_TimeBomb,
    CardType_TimeHealsAllWounds,
    CardType_TimeHealsAllWounds,
    CardType_WormHole,
    CardType_WormHole,
    CardType_Sacrifice,
    CardType_Sacrifice,
    CardType_Sacrifice,
    CardType_Sacrifice,
    CardType_Sacrifice,
    CardType_Sacrifice,
    CardType_Sacrifice
};

const static Card_Type chaos_deck[] = {
    CardType_Altruism,
    CardType_Altruism,
    CardType_BallAndChain,
    CardType_BallAndChain,
    CardType_BestIntentions,
    CardType_BestIntentions,
    CardType_BloodPact,
    CardType_BloodPact,
    CardType_Charity,
    CardType_Charity,
    CardType_DarkRitual,
    CardType_DarkRitual,
    CardType_DevilsWheel,
    CardType_DevilsWheel,
    CardType_ForcedSalvation,
    CardType_ForcedSalvation,
    CardType_LuckyFeeling,
    CardType_LuckyFeeling,
    CardType_OwnInterests,
    CardType_OwnInterests,
    CardType_PlayingDumb,
    CardType_PlayingDumb,
    CardType_PureAnarchy,
    CardType_PureAnarchy,
    CardType_SapLife,
    CardType_SapLife,
    CardType_UnendingTorment,
    CardType_UnendingTorment,
    CardType_Sacrifice,
    CardType_Sacrifice,
    CardType_Sacrifice,
    CardType_Sacrifice,
    CardType_Sacrifice,
    CardType_Sacrifice,
};

#endif  // LUDUM_H_
