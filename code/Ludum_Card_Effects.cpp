internal CARD_EFFECT(GravitationalPull) {
    // Send your opponents last drawn card back to their deck
    // Mana cost: 3

    if (enemy->card_count != 0) {
        enemy->current_card_index--;
        enemy->card_count--;
    }
}

internal CARD_EFFECT(SapLife) {
    // Lose 3 health
    // Mana cost: 0

    player->health -= 3;
}

internal CARD_EFFECT(Sacrifice) {
    // Lose 3 health
    // Mana cost: 0

    player->health -= 3;
}

internal CARD_EFFECT(DevilsWheel) {
    // Swap a random card with the enemy
    // Mana cost: 3

    if (enemy->card_count != 0) {
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

    // @Todo: Implement
    // ... How to select the card.. It seems like this is a multi-step process
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

    if (RandomChoice(2) == 0) { player->health -= 3; }
    else { enemy->health -= 3; }
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

    u32 damage = (player->card_count - 1) * 2;
    enemy->health -= damage;
}

internal CARD_EFFECT(Rewind) {
    // Recover all cards you played last turn
    // Mana cost: 7

    for (u32 it = 0; it < player->last_turn_used_count; ++it) {
        Card *card = &player->graveyard[--player->graveyard_next];
        player->cards[player->card_count++] = *card;

        // @Hack: To get around the 10 card limit in hand
        if (player->card_count == 10) { break; }
    }
}

internal CARD_EFFECT(SpaceDebris) {
    // Add 3, 1-mana, 'debris' cards to your opponents hand
    // Mana cost: 2

    Card debris = GetCard(state->card_index, CardType_SpaceDebris);
    for (u32 it = 0; it < 3; ++it) { enemy->cards[enemy->card_count++] = debris; }
}

internal CARD_EFFECT(TimeBomb) {
    // Deals damage equal to all damage done last turn
    // Mana cost: 5

    // @Todo: Implement
    // ... Where to get this information from
}

internal CARD_EFFECT(WormHole) {
    // Your opponent can't use their furthest left card for 2 turns
    // Mana cost: 6

    enemy->cards[0].effect_flags |= CardStatus_Frozen;
    enemy->cards[0].frozen_turn_count = 2;

    enemy->cards[1].effect_flags |= CardStatus_Frozen;
    enemy->cards[1].frozen_turn_count = 2;
}

internal CARD_EFFECT(PureAnarchy) {
    // Shuffle the player's hands
    // Mana cost: 4

    u32 card_total = player->card_count + enemy->card_count;
    Card card_pool[card_total];
    for (u32 it = 0; it < player->card_count; ++it) {
        card_pool[it] = player->cards[it];
    }

    for (u32 it = 0; it < enemy->card_count; ++it) {
        card_pool[player->card_count + it] = enemy->cards[it];
    }

    // Distribute evenly
    u32 player_count = card_total / 2;
    u32 enemy_count = card_total - player_count;

    // @Todo: shuffle the cards
}

internal CARD_EFFECT(DarkRitual) {
    // Draw 2 sacrifice cards and add 1 to max mana
    // Mana cost: 5

    if (player->max_mana < 10) { player->max_mana++; }

    Card sacrifice = GetCard(state->card_index, CardType_Sacrifice);
    player->cards[player->card_count++] = sacrifice;
}

internal CARD_EFFECT(UnendingTorment) {
    // If your opponent made a sacrifice last turn, add a sacrifice to their hand

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

    // @Todo: Implement
}

internal CARD_EFFECT(BlackHole) {
    // Dispose of either your or your opponent's entire hand
    // Mana cost: 8

    // @Todo: Like Charity this is a 2 step process
}

internal CARD_EFFECT(BloodPact) {
    // Any sacfifice damage done this turn is also done to the enemy
    // Mana cost: 2

    // @Todo: This requires changes to the sacrifice and sap life cards
}
