#include "flow.h"
#include "../coms/comunication.h"
#include "../entities/chars.h"


void welcome_client(int client_socket){
    char* welcome_msg = "Bienvenido a Monster Hunter: Ruz";
    server_send_message(client_socket, 0,welcome_msg);
    //code: 0 -> welcome msg
}
// void pasar_turno(entity** players, entity* target, int* rondas, int* rondas_since_fb, int amt_players)
// {
//     for (int i = 0; i < amt_players; i++)
//     {
//         actualize_entity(players[i]);
//     }
//     actualize_entity(target);
//     //TODO: Se termina el turno

    
// }

// void actualize_entity(entity* player)
// {

//     dmg_player(player, player->dmg_per_turn);
//     if (player->alive)
//     {
//         if (player->jugador->intoxicated)
//         {
//             player->jugador->turns_intoxicated--;
//             if (player->jugador->turns_intoxicated == 0)
//             {
//                 player->jugador->intoxicated = false;
//                 player->dmg_per_turn -= 500; // TODO: Cambiar por variable cuando solucione el tema de importar variables globales
//             } 
//         }
//     }
// }