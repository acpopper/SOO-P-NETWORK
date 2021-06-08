#include "monsters.h"

// Crea un monstruo en el heap
// @param [char*] type: tipos permitidos: "Great JagRuz" "Ruzalos" "Ruiz, el Gemelo Malvado del Profesor Ruz"
// @return [monster*] retorna el puntero al monstruo nuevo
monster* new_monster(char* type)
{
    monster* new_mons = malloc(sizeof(monster));
    if (!strcmp(type, "Great JagRuz")){
        new_mons->vida_max = 10000;
        new_mons->type = type;} 
    else if (!strcmp(type, "Ruzalos")){
        new_mons->vida_max = 20000;
        new_mons->type = type;} 
    else if (!strcmp(type, "Ruiz, el Gemelo Malvado del Profesor Ruz")){
        new_mons->vida_max = 25000;
        new_mons->type = type;}
    new_mons->alive = true;
    new_mons->used_salto = false;
    new_mons->vida = new_mons->vida_max;
    new_mons->times_sangrado = 0;
    return new_mons;
}
