#pragma once
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    int vida_max;
    int vida;
    char *type;
    bool alive;
    int times_sangrado;
    int side_effect_dmg;
    bool used_salto;
} monster;

monster* new_monster(char* type);
