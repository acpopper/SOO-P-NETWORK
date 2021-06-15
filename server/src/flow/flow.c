#include "flow.h"

// Función para pasar de turno
// @return [bool] retorna true si el combate sigue y false si el combate se acaba
bool pasar_turno(entity** players, entity* target, int* rondas, int* rondas_since_rm, int amt_players)
{
    *rondas += 1;
    *rondas_since_rm += 1;
    bool someone_alive = false;
    for (int i = 0; i < amt_players; i++)
    {
        actualize_entity(players[i]);
        if (!someone_alive)
        {
            someone_alive = players[i]->alive;
        }
    }
    actualize_entity(target);
    if (!target->alive || !someone_alive)
    {
        return false;
    }
    return true;
}

void actualize_entity(entity* entidad)
{
    entity* nature = malloc(sizeof(entity));
    nature->type = "Tiempo";
    nature->jugador = malloc(sizeof(player));
    nature->jugador->nombre = "Tiempo";
    nature->dmg_modifier = 1;
    dmg_entity(nature, entidad, entidad->dmg_per_turn);
    if (entidad->alive)
    {
        if (entidad->is_player && entidad->jugador->intoxicated)
        {
            printf("Daño por intoxicación");
            entidad->jugador->turns_intoxicated--;
            if (entidad->jugador->turns_intoxicated == 0)
            {
                entidad->jugador->intoxicated = false;
                entidad->dmg_per_turn -= 500; // TODO: Cambiar por variable cuando solucione el tema de importar variables globales
            } 
        }
    }
    free(nature->jugador);
    free(nature);
}