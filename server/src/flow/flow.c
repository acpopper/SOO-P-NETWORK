#include "flow.h"
#include "../coms/comunication.h"
#include "../entities/chars.h"

const int MAX_PLAYERS = 4;

void welcome_client(int client_socket)
{
    char *welcome_msg = "Bienvenido a Monster Hunter: Ruz";
    server_send_message(client_socket, 2, welcome_msg);
    //code: 0 -> welcome msg
}

void add_name(entity **players, int client_socket)
{
    char *name = server_receive_payload(client_socket);
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (players[i] != 0)
        {
            if (players[i]->jugador->client_socket == client_socket)
            {
                name_entity_player(players[i], name);
            }
        }
    }
}

void add_type(entity **players, int client_socket)
{
    char *msg = server_receive_payload(client_socket);
    int type = atoi(msg);
    free(msg);
    int leader = 0;
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (players[i] != 0)
        {
            if (players[i]->jugador->client_socket == client_socket)
            {
                type_entity_player(players[i], type);
            }
            if (players[i]->jugador->party_leader)
            {
                leader = players[i]->jugador->client_socket;
            }
        }
    }
    notify_leader(leader, players, client_socket);
}

void notify_leader(int leader, entity **players, int new_player)
{
    char msg[20];
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (players[i]->jugador->client_socket == new_player)
        {
            sprintf(msg, "%s - %s \n", players[i]->jugador->nombre, players[i]->type);
            break;
        }
    }
    server_send_message(leader, 3, msg);
}

bool game(entity **players, int leader, int connections)
{
    int count_players_with_name = 0;
    char *msg = server_receive_payload(leader);
    int type = atoi(msg);
    free(msg);
    bool start = false;
    if (!type)
    {
        if (connections == MAX_PLAYERS)
        {
            //lider esta obligado a comenzar -> debe elegir monstruo
            printf("hola\n");
            server_send_message(leader, 4, "1");
        }
    }
    else
    {
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            if (players[i] != 0)
            {
                if (strcmp(players[i]->jugador->nombre, "") != 0 && strcmp(players[i]->type, "") != 0)
                {
                    count_players_with_name++;
                }
            }
        }
        if (connections == count_players_with_name)
        {
            start = true;
            players[MAX_PLAYERS] = new_monster(type);

            for (int i = 0; i < MAX_PLAYERS; i++)
            {
                if (players[i] != 0)
                {
                    server_send_message(players[i]->jugador->client_socket, 5, players[MAX_PLAYERS]->jugador->nombre);
                }
            }
        }
        else
        {
            //error: no todos los jugadores han ingresado su nombre
            server_send_message(leader, 4, "0");
        }
    }

    return start;
}

void turno(entity **players, int client_socket)
{
    int turno_actual = client_socket;
    print_situacion(turno_actual, players);
    for (int i = 0; i < 4; i++)
    {
        if (players[i]->jugador->client_socket == client_socket)
        {
            if (strcmp(players[i]->type, "Cazador") == 0)
            {
                server_send_message(turno_actual, 7, "");
            }
            else if (strcmp(players[i]->type, "Médico") == 0)
            {
                server_send_message(turno_actual, 8, "");
            }
            else if (strcmp(players[i]->type, "Hacker") == 0)
            {
                server_send_message(turno_actual, 9, "");
            }
            break;
        }
    }
}

void print_situacion(int client_socket, entity **players)
{
    char msg[200];
    char info[100];
    for (int i = 0; i < 5; i++)
    {
        if (players[i] != 0)
        {
            if (players[i]->is_player)
            {
                sprintf(info, "[%s] %s -> VIDA: %d / %d\n", players[i]->jugador->nombre, players[i]->type, players[i]->vida, players[i]->vida_max);
                strcat(msg, info);
            }
            else
            {
                sprintf(info, "[MONSTER] %s -> VIDA: %d / %d\n", players[i]->type, players[i]->vida, players[i]->vida_max);
                strcat(msg, info);
            }
        }
    }
    server_send_message(client_socket, 6, msg);
}

entity *select_user(int client_socket, entity **entities)
{
    entity *user;
    for (int i = 0; i < 5; i++)
    {
        if (entities[i] && entities[i]->is_player && entities[i]->jugador->client_socket == client_socket)
        {
            user = entities[i];
        }
    }
    return user;
}

entity *select_target(int client_socket, entity **entities, int skill, char *type_user)
{
    entity *target;
    // Si no hay opcion de elegir (skills 2 o 3 para cualquiera) o si es cazador, target=monstruo
    if (skill > 1 || !strcmp(type_user, "Cazador"))
    {
        for (int i = 0; i < 5; i++)
        {
            if (entities[i] && !(entities[i]->is_player))
            {
                target = entities[i];
                printf("Se usó abilidad %i contra %s\n", skill, target->type);
                return target;
            }
        }
    }
    // Si hay elección le pregunto
    else
    {
        printf("Eligiendo target\n");
        display_players_targets(client_socket, entities);
        char *msg = server_receive_payload(client_socket);
        int player = atoi(msg);
        free(msg);
        int c = 0;
        for (int i = 0; i < 5; i++)
        {
            if (entities[i] && entities[i]->is_player && player == c)
            {
                target = entities[i];
                printf("Se usó abilidad %i contra %s\n", skill, target->type);
                return target;
            }
            c += 1;
        }
    }
    return NULL;
}
// Envía los jugadores en la party enumerados
void display_players_targets(int client_socket, entity **entities)
{
    char msg[100];
    char info[50];
    int contador = 0;
    for (int i = 0; i < 5; i++)
    {
        if (entities[i] && entities[i]->is_player)
        {
            sprintf(info, "%i) %s\n", contador, entities[i]->jugador->nombre);
            strcat(msg, info);
            contador += 1;
        }
    }
    server_send_message(client_socket, 78, msg);
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