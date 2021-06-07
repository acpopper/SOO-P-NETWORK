#include "flow.h"

void pasar_turno(player** players, monster* target, int* rondas, int* rondas_since_fb, int amt_players)
{
    for (int i = 0; i < amt_players; i++)
    {
        actualize_player(players[i]);
    }
    actualize_monster(target);
    //TODO: Se termina el turno

    
}

void actualize_player(player* player)
{
    dmg_player(player, player->dmg_per_turn);
    if (player->alive)
    {
        if (player->intoxicated)
        {
            player->turns_intoxicated--;
            if (player->turns_intoxicated == 0)
            {
                player->intoxicated = false;
                player->dmg_per_turn -= 500; // TODO: Cambiar por variable cuando solucione el tema de importar variables globales
            } 
        }
    }
}

void actualize_monster(monster* monster)
{
    player* dummy_player = malloc(sizeof(player));
    dummy_player->dmg_modifier = 1;
    dmg_monster(dummy_player, monster, monster->side_effect_dmg);
    if (monster->alive)
    {
        if (monster->times_sangrado > 0)
        {
            monster->times_sangrado--;
            monster->side_effect_dmg -= 500; // TODO: Cambiar por variable cuando solucione el tema de importar variables globales
        }
        
    }   
    free(dummy_player);
}