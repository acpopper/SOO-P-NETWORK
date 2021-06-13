#include "flow.h"
#include "../coms/comunication.h"
#include "../entities/chars.h"


void welcome_client(int client_socket){
    char* welcome_msg = "Bienvenido a Monster Hunter: Ruz";
    server_send_message(client_socket, 0,welcome_msg);
    //code: 0 -> welcome msg
}

void add_name(entity** players, int client_socket){
    char* name = server_receive_payload(client_socket);
    for(int i =0; i<4; i++){
        if(players[i]!=0){
            if(players[i]->jugador->client_socket==client_socket){
                name_entity_player(players[i], name);
        }
        }   
    }
    free(name);
}

void add_type(entity** players, int client_socket){
    char *msg = server_receive_payload(client_socket);
    int type = atoi(msg);
    free(msg);
    for(int i =0; i<4; i++){
        if(players[i]!=0){
            if(players[i]->jugador->client_socket==client_socket){
                type_entity_player(players[i], type);
            }
            if(players[i]->jugador->party_leader){
                notify_leader(players[i]->jugador->client_socket,players);
            }
        }   
    }

}
void notify_leader(int leader, entity** players){
    
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