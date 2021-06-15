#pragma once
#include "../entities/chars.h"

void pasar_turno(entity** players, entity* target, int* rondas, int* rondas_since_fb, int amt_players);
void actualize_entity(entity* player);