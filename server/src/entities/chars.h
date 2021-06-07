#pragma once
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "monsters.h"
#include "values.h"

typedef struct Player
{
    bool party_leader;
    bool alive;
    bool intoxicated;
    char* nombre;
    int vida_max;
    int vida;
    float dmg_modifier;
    float dmg_recieved_modifier;
    int dmg_per_turn;
    int turns_intoxicated;
    char* class;
    char* ability1_name;
    char* ability2_name;
    char* ability3_name;
    int times_fb;
    bool last_used_distraer;
} player;


player* new_player(char* nombre, bool party_leader, char* class);
bool use_ability(player* user, monster* target, player* target_p, char* ability, player ** players, int amt_of_players);
bool use_ability_cazador(player* user, monster* target, char* ability, player** players, int amt_of_players);
bool use_ability_medico(player* user, monster* target, player* target_p, char* ability, player** players, int amt_of_players);
bool use_ability_hacker(player* user, monster* target, player* target_p, char* ability);
bool monster_use_ability(monster *user, player** players, int amt_of_players, int* rounds);
void dmg_player(player* target, int dmg);
void dmg_monster(player* attacker, monster* target, int dmg);
void heal_player(player* user, player* target, int amt);
bool use_estocada(monster* target);
bool use_corte_cruzado(player* user, monster* target);
bool use_distraer(player **players, player* user, int amt_of_players);
