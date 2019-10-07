internal void ShuffleCards(Card *cards, u32 count) {
    Card *temp = cast(Card *) malloc(sizeof(Card) * count);

    // @Hack: This is super inefficient lmao

    for (u32 it = 0; it < count; ++it) {
        u32 j = RandomChoice(it + 1);

        Assert(j < count);

        temp[it] = temp[j];
        temp[j] = cards[it];
    }

    for (u32 it = 0; it < count; ++it) { cards[it] = temp[it]; }

    free(temp);
}

internal Card GetCard(Card_Index *index, Card_Type type) {
    Card result = index->cards[type];
    return result;
}

internal bool HasActiveCard(Player *player, Card_Type type) {
    bool result = false;
    for (u32 it = 0; it < player->active_card_count; ++it) {
        if (player->active_cards[it] == type) { result = true; break; }
    }

    return result;
}

internal void RemoveCard(Player *player, u32 index) {
    if (player->card_count == 0) { return; }

    Card removed = player->cards[index];
    for (u32 it = index; it < player->card_count - 1; ++it) {
        player->cards[it] = player->cards[it + 1];
    }

    player->graveyard[player->graveyard_next++] = removed;
    player->card_count--;
}

internal void RemoveActiveCard(Player *player, Card_Type type) {
    for (u32 it = 0; it < player->active_card_count; ++it) {
        if (player->active_cards[it] == type) {

            for (u32 move = it; move < player->active_card_count - 1; ++move) {
                player->active_cards[move] = player->active_cards[move + 1];
            }

            player->active_card_count--;
        }
    }
}

internal CARD_EFFECT(GravitationalPull) {
    // Send your opponents last drawn card back to their deck
    // Mana cost: 3

    if (enemy->card_count != 0) {
        enemy->next_card_index--;
        enemy->card_count--;
    }
}

internal CARD_EFFECT(SapLife) {
    // Lose 3 health
    // Mana cost: 0

    player->health -= 3;
    state->sacrifice_damage_this_turn += 3;

    if (HasActiveCard(enemy, CardType_ForcedSalvation)) { enemy->health += 3; }
    if (HasActiveCard(player, CardType_BloodPact)) { enemy->health -= 3; }
}

internal CARD_EFFECT(Sacrifice) {
    // Lose 3 health
    // Mana cost: 0

    player->health -= 3;
    state->sacrifice_damage_this_turn += 3;

    if (HasActiveCard(enemy, CardType_ForcedSalvation)) { enemy->health += 3; }
    if (HasActiveCard(player, CardType_BloodPact)) { enemy->health -= 3; }
}

internal CARD_EFFECT(DevilsWheel) {
    // Swap a random card with the enemy
    // Mana cost: 3

    if (enemy->card_count != 0 && player->card_count != 0) {
        u32 player_index = RandomChoice(player->card_count);
        u32 enemy_index = RandomChoice(enemy->card_count);

        Swap(player->cards[player_index], enemy->cards[enemy_index]);
    }
}

internal CARD_EFFECT(BoundByTime) {
    // The opponent cannot use newly drawn cards next turn
    // Mana cost: 4

    enemy->new_card_flags |= CardStatus_Frozen;
}


internal CARD_EFFECT(Charity) {
    // Give your opponent a card from your hand
    // Mana cost: 6

    player->state_flags = PlayerState_Selecting | PlayerState_ToEnemy;
    player->select_card_count = 1;
}

internal CARD_EFFECT(DarknessOfSpace) {
    // Your opponent cannot see their cards next turn
    // Mana cost: 2

    for (u32 it = 0; it < enemy->card_count; ++it) {
        enemy->cards[it].effect_flags |= CardStatus_Hidden;
        enemy->cards[it].hidden_turn_count = 1;
    }

    enemy->new_card_flags |= CardStatus_Hidden;
}

internal CARD_EFFECT(LuckyFeeling) {
    // Deal 3 damage to your opponent... Or yourself...
    // Mana cost: 2

    if (RandomChoice(2) == 0) {
        player->health -= 3;

        state->sacrifice_damage_this_turn += 3;

        if (HasActiveCard(enemy, CardType_ForcedSalvation)) { enemy->health += 3; }
        if (HasActiveCard(player, CardType_BloodPact)) { enemy->health -= 3; }
    }
    else {
        enemy->health -= 3;
        state->damage_this_turn += 3;
    }
}

internal CARD_EFFECT(NostalgiaTrip) {
    // The last card your opponent played is played again
    // Mana cost: 2
    if (enemy->graveyard_next != 0) {
        Card *last_card = &enemy->graveyard[enemy->graveyard_next - 1];
        last_card->Execute(state, enemy, player);
    }
}

internal CARD_EFFECT(OwnInterests) {
    // Duplicate all cards in your hand
    // Mana cost: 8

    u32 card_count = player->card_count;
    for (u32 it = 0; it < card_count; ++it) {
        player->cards[player->card_count++] = player->cards[it];
    }
}

internal CARD_EFFECT(PlayingDumb) {
    // Deal damage equal to the number of cards in your hand multipled by 2
    // Mana cost: 2

    u32 damage = player->card_count * 2;
    enemy->health -= damage;
    state->damage_this_turn += damage;
}

internal CARD_EFFECT(Rewind) {
    // Recover all cards you played last turn
    // Mana cost: 7

    // @Note: This is the rewind card itself as it is added to the graveyard before this function is executed
    Card just_used = player->graveyard[--player->graveyard_next];

    for (u32 it = 0; it < player->last_turn_used_count; ++it) {
        Card card = player->graveyard[--player->graveyard_next];
        player->cards[player->card_count++] = card;
    }

    player->graveyard[player->graveyard_next++] = just_used;
}

internal CARD_EFFECT(SpaceDebris) {
    // Add 3, 1-mana, 'debris' cards to your opponents hand
    // Mana cost: 2

    Card debris = GetCard(state->card_index, CardType_Pebbles);
    for (u32 it = 0; it < 3; ++it) {
        enemy->cards[enemy->card_count++] = debris;
    }
}

internal CARD_EFFECT(TimeBomb) {
    // Deals damage equal to all damage done last turn
    // Mana cost: 5

    u32 total_damage = (state->damage_last_turn + state->sacrifice_damage_last_turn);
    enemy->health -= total_damage;

    state->damage_this_turn += total_damage;
}

internal CARD_EFFECT(WormHole) {
    // View your opponents hand
    // Mana cost: 5

    player->state_flags = PlayerState_ViewingHand;
}

internal CARD_EFFECT(PureAnarchy) {
    // Shuffle the player's hands
    // Mana cost: 4

    u32 card_total = player->card_count + enemy->card_count;
    Card *card_pool = cast(Card *) malloc(sizeof(Card) * card_total);
    for (u32 it = 0; it < player->card_count; ++it) {
        card_pool[it] = player->cards[it];
    }

    for (u32 it = 0; it < enemy->card_count; ++it) {
        card_pool[player->card_count + it] = enemy->cards[it];
    }

    // Distribute evenly
    u32 player_count = card_total / 2;
    u32 enemy_count = card_total - player_count;

    ShuffleCards(card_pool, card_total);

    player->card_count = player_count;
    for (u32 it = 0; it < player_count; ++it) {
        player->cards[it] = card_pool[it];
    }

    enemy->card_count = enemy_count;
    for (u32 it = 0; it < enemy_count; ++it) {
        enemy->cards[it] = card_pool[player_count + it];
    }

    // This is why a temporary allocator is really useful
    free(card_pool);
}

internal CARD_EFFECT(DarkRitual) {
    // Draw 2 sacrifice cards and add 1 to max mana
    // Mana cost: 5

    if (player->max_mana < 10) { player->max_mana++; }
    else { player->mana++; } // @Note: This is temporary (i.e. only lasts for the turn)

    Card sacrifice = GetCard(state->card_index, CardType_Sacrifice);
    for (u32 it = 0; it < 2; ++it) { player->cards[player->card_count++] = sacrifice; }
}

internal CARD_EFFECT(UnendingTorment) {
    // If your opponent made a sacrifice last turn, add a sacrifice to their hand
    // Mana cost: 3

    u32 base_index = enemy->graveyard_next - enemy->last_turn_used_count;
    for (u32 it = 0; it < enemy->last_turn_used_count; ++it) {
        if (enemy->graveyard[base_index + it].type == CardType_Sacrifice) {
            enemy->cards[enemy->card_count++] = GetCard(state->card_index, CardType_Sacrifice);
            break;
        }
    }
}

internal CARD_EFFECT(BestIntentions) {
    // Duplicate all cards in your opponents hand
    // Mana cost: 3

    u32 card_count = enemy->card_count;
    for (u32 it = 0; it < card_count; ++it) {
        enemy->cards[enemy->card_count++] = enemy->cards[it];
    }
}

internal CARD_EFFECT(BlackHole) {
    // Dispose of either your or your opponent's entire hand
    // Mana cost: 8

    player->state_flags |= PlayerState_RemovingHand;
}

internal CARD_EFFECT(BloodPact) {
    // Any sacfifice damage done this turn is also done to the enemy
    // Mana cost: 2

    player->active_cards[player->active_card_count++] = CardType_BloodPact;
}

internal CARD_EFFECT(Altruism) {
    // Select up to 3 cards to give to your opponent
    // Mana cost: 6

    player->state_flags = PlayerState_Selecting | PlayerState_ToEnemy;
    player->select_card_count = 3;
}

internal CARD_EFFECT(EventHorizon) {
    // Your opponent can't use their furthest left card for 2 turns
    // Mana cost: 6

    enemy->cards[0].effect_flags |= CardStatus_Frozen;
    enemy->cards[0].frozen_turn_count = 2;

    enemy->cards[1].effect_flags |= CardStatus_Frozen;
    enemy->cards[1].frozen_turn_count = 2;
}

internal CARD_EFFECT(ForcedSalvation) {
    // Self-inflicted damage your opponent deals on their turn, heals you
    // Mana cost: 2

    player->active_cards[player->active_card_count++] = CardType_ForcedSalvation;
}

internal CARD_EFFECT(Premonition) {
    // See the card you opponent will next draw
    // Mana cost: 2

    player->state_flags = PlayerState_ViewingDraw;
    player->select_card_count = 1;
}

internal CARD_EFFECT(Recursion) {
    // Gain 2 mana this turn. This card is played every turn until you end your turn with < 2 mana.
    // Mana cost: 1

    player->mana += 2;
    player->active_cards[player->active_card_count++] = CardType_Recursion;
}

internal CARD_EFFECT(TimeHealsAllWounds) {
    // Your health is reset to what it was last turn
    // Mana cost: 5

    player->health = player->last_turn_health;
}

internal CARD_EFFECT(AlternateReality) {
    // Swap your middle card for their card. If they have an even number take 2 (give 1)
    // Mana cost: 3

    // @Note: I think this is working correctly, it appends the new cards to the end of the hand rather than
    // inserting them into the middle because it is easier

    u32 player_card_count = 2;
    u32 player_middle = (player->card_count / 2) - 1; // Can technically overflow, but we don't use results
    if (player->card_count & 1) {
        player_card_count = 1;
        player_middle = (player->card_count / 2);
    }
    else if (player->card_count == 0) {
        player_card_count = 0;
        player_middle = 0;
    }

    u32 enemy_card_count = 2;
    u32 enemy_middle = (enemy->card_count / 2) - 1; // Can technically overflow, but we don't use results
    if (enemy->card_count & 1) {
        enemy_card_count = 1;
        enemy_middle = (enemy->card_count / 2);
    }
    else if (enemy->card_count == 0) {
        enemy_card_count = 0;
        enemy_middle = 0;
    }

    // 10 / 2 = 5 ... 2 card index 4 & 5
    // [0] [] [] [] | [4] [5] | [] [] [] [9]

    // 9 / 2 = 4 ... 1 card index 4
    // [0] [] [] [] | [4] | [] [] [] [8]

    // 3 / 2 = 1 ... 1 card index 1
    // [] | [1] | []

    for (u32 it = 0; it < player_card_count; ++it) {
        enemy->cards[enemy->card_count++] = player->cards[player_middle + it];
    }

    for (u32 it = 0; it < enemy_card_count; ++it) {
        player->cards[player->card_count++] = enemy->cards[enemy_middle + it];
    }

    for (u32 it = 0; it < enemy_card_count; ++it) { RemoveCard(enemy, enemy_middle); }
    for (u32 it = 0; it < player_card_count; ++it) { RemoveCard(player, player_middle); }
}
