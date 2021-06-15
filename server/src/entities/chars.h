#pragma once
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Player
{
    bool party_leader;
    bool intoxicated;
    char* nombre;
    int turns_intoxicated;
    char* ability1_name;
    char* ability2_name;
    char* ability3_name;
    bool last_used_distraer;
} player;

typedef struct Monster
{
    bool used_salto;
} monster;

typedef struct Entity
{
    int vida_max;
    int vida;
    char *type; // clase para players, tipo para monstruos
    bool alive;
    bool is_player;
    float dmg_modifier;
    float dmg_recieved_modifier;
    int times_sangrado;
    int dmg_per_turn;
    int times_fb;
    player* jugador;
    monster* monstruo;
} entity;

entity* new_monster(char* type);
entity* new_entity(char* nombre, bool party_leader, char* type, bool is_player);
bool use_ability(entity* user, entity* target, char* ability, entity ** players, int amt_of_players);
bool use_ability_cazador(entity* user, entity* target, char* ability, entity** players, int amt_of_players);
bool use_ability_medico(entity* user, entity* target, char* ability, entity** players, int amt_of_players);
bool use_ability_hacker(entity* user, entity* target, char* ability);
bool entity_use_ability(entity *user, entity** players, int amt_of_players, int* rounds);
void dmg_entity(entity* attacker, entity* target, int dmg);
void heal_entity(entity* user, entity* target, int amt);
bool use_estocada(entity* user, entity* target);
bool use_corte_cruzado(entity* user, entity* target);
bool use_distraer(entity **players, entity* user, int amt_of_players);
bool use_sql(entity* target);
bool use_ddos(entity* user, entity* target);
bool use_fb(entity* user, entity* target);
bool use_curar(entity* user, entity* target);
bool use_destello(entity* user, entity* target, entity** entitys, int amt_of_entitys);
bool use_vital(entity* user, entity* target);
void free_entity(entity* entidad);