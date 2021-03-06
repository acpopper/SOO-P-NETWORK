#pragma once
#include "../entities/chars.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void pasar_turno(entity** players, entity* target, int* rondas, int* rondas_since_fb, int amt_players);
void actualize_entity(entity* player);
void welcome_client(int client_socket);
void add_name(entity** players, int client_socket);
void add_type(entity** players, int client_socket);
void notify_leader(int leader, entity** players, int new_player);
bool game(entity** players, int leader,int connections);
void turno(entity** players, int client_socket);
void print_situacion(int client_socket, entity** players);
entity* select_user(int client_socket, entity** entities);
entity* select_target(int client_socket, entity** entities, int skill, char* type_user);
void display_players_targets(int client_socket, entity** entities);