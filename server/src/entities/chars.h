#pragma once
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Player
{
    int client_socket;
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

typedef struct Game {
    int amt_of_players;
    entity** players;
    entity* monster;
    int* rounds;
    int* rounds_rm;
    bool battle_going;
    bool game_going;
    char* game_state;
} juego;

juego* init_game(entity** players, entity* monster, int amt_of_players);
void acualize_game_state(juego* juego);
bool pasar_turno(entity** players, entity* target, int* rondas, int* rondas_since_fb, int amt_players, juego* game);
void actualize_entity(entity* player);
void free_game(juego* game);


entity* new_monster(int type);
entity* new_entity(bool party_leader, int client_socket, bool is_player, char *name, char* type); //crear entity
void name_entity_player(entity* player, char* name);
void type_entity_player(entity* player, int type);

bool use_ability(entity* user, entity* target, char* ability, entity ** players, int amt_of_players, juego* game);
bool use_ability_cazador(entity* user, entity* target, char* ability, entity** players, int amt_of_players, juego* game);
bool use_ability_medico(entity* user, entity* target, char* ability, entity** players, int amt_of_players, juego* game);
bool use_ability_hacker(entity* user, entity* target, char* ability, juego* game);
bool entity_use_ability(entity *user, entity** players, int amt_of_players, int* rounds, juego* game);
void dmg_entity(entity* attacker, entity* target, int dmg);
void heal_entity(entity* user, entity* target, int amt);
bool use_estocada(entity* user, entity* target, juego* game);
bool use_corte_cruzado(entity* user, entity* target, juego* game);
bool use_distraer(entity **players, entity* user, int amt_of_players, juego* game);
bool use_sql(entity* target, juego* game);
bool use_ddos(entity* user, entity* target, juego* game);
bool use_fb(entity* user, entity* target, juego* game);
bool use_curar(entity* user, entity* target, juego* game);
bool use_destello(entity* user, entity* target, entity** entitys, int amt_of_entitys, juego* game);
bool use_vital(entity* user, entity* target, juego* game);
void free_entity(entity* entidad);