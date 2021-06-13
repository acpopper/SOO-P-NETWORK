#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "../coms/comunication.h"
#include "../coms/conection.h"
#include "../entities/chars.h"
#include "../flow/flow.h"

char * revert(char * message){
  //Se invierte el mensaje

  int len = strlen(message) + 1;
  char * response = malloc(len);

  for (int i = 0; i < len-1; i++)
  {
    response[i] = message[len-2-i];
  }
  response[len-1] = '\0';
  return response;
}

int main(int argc, char *argv[]){
  entity** players = malloc(sizeof(player*) * 4);
  char** actions = malloc(sizeof(char*) * 5);
  char* action_p1 = malloc(sizeof(char*));
  char* action_p2 = malloc(sizeof(char*));
  char* action_p3 = malloc(sizeof(char*));
  char* action_p4 = malloc(sizeof(char*));
  actions[0] = action_p1;
  actions[1] = action_p2;
  actions[2] = action_p3;
  actions[3] = action_p4;
  entity* p1 = new_entity("juan", false, "Cazador", true);
  entity* p2 = new_entity("fede", false, "Médico", true);
  entity* p3 = new_entity("fco", false, "Hacker", true);
  entity* p4 = new_entity("pedro", true, "Cazador", true);
  int* rondas = malloc(sizeof(int));
  int* rondas_rm = malloc(sizeof(int));
  *rondas_rm = 0;
  *rondas = 0;
  players[0] = p1;
  players[1] = p2;
  players[2] = p3;
  players[3] = p4;
  entity* m1 = new_entity("", false, "Ruiz, el Gemelo Malvado del Profesor Ruz", false);
  bool battle_going = true;
  int* c = malloc(4);
  while (battle_going)
  {
    printf("RONDA: %d\n", *rondas);
    printf( "0 para estocada 1 para corte cruzado:\n");
    scanf("%d", c);
    if (*c == 0)
    {
      use_ability(p1, m1, "Estocada", players, 4);
    }
    else
    {
      use_ability(p1, m1, "Corte Cruzado", players, 4);
    }
    entity_use_ability(m1, players, 4, rondas_rm);
    for (int i = 0; i < 4; i++)
    {
      printf("Vida jugador %s: %d/%d\n", players[i]->jugador->nombre, players[i]->vida, players[i]->vida_max);
    }
    printf("Vida monstruo %s: %d/%d\n", m1->type, m1->vida, m1->vida_max);
    battle_going = pasar_turno(players, m1, rondas, rondas_rm, 4);
  }
  
  // printf("%d\n", m1->vida);
  // player* p0 = new_player("prer", true, "Médico");
  
  // if (entity_use_ability(m1, players, 4, rondas))
  // {
  //   for (int i = 0; i < 4; i++)
  //   {
  //     printf("Vida jugador %d: %d/%d\n", i, players[i]->vida, players[i]->vida_max);
  //     // printf("%d\n", players[i]->jugador->last_used_distraer);
  //   }

  // }
  // printf("%d\n", *rondas);
  
  free(rondas);
  free(rondas_rm);
  free_entity(p1);
  free_entity(p2);
  free_entity(p3);
  free_entity(p4);
  free_entity(m1);
  free(action_p1);
  free(action_p2);
  free(action_p3);
  free(action_p4);
  free(players);
  free(actions);
  free(c);

  // int i;
  // for (i = 0; i < argc; i++)
  // printf("argv[%d] = %s\n", i, argv[i]);
  // while (1)
  // {

  // }

  // Se define una IP y un puerto
  // char * IP = argv[2];
  // int PORT = atoi(argv[4]);

  // // Se crea el servidor y se obtienen los sockets de ambos clientes.
  // PlayersInfo * players_info = prepare_sockets_and_get_clients(IP, PORT);

  // // Le enviamos al primer cliente un mensaje de bienvenida
  // char * welcome = "Bienvenido Cliente 1!!";
  // server_send_message(players_info->socket_c1, 1, welcome);

  // // Guardaremos los sockets en un arreglo e iremos alternando a quién escuchar.
  // int sockets_array[2] = {players_info->socket_c1, players_info->socket_c2};
  // int my_attention = 0;
  // while (1)
  // {
  //   // Se obtiene el paquete del cliente 1
  //   int msg_code = server_receive_id(sockets_array[my_attention]);

  //   if (msg_code == 1) //El cliente me envió un mensaje a mi (servidor)
  //   {
  //     char * client_message = server_receive_payload(sockets_array[my_attention]);
  //     printf("El cliente %d dice: %s\n", my_attention+1, client_message);

  //     // Le enviaremos el mismo mensaje invertido jeje
  //     char * response = revert(client_message);

  //     // Le enviamos la respuesta
  //     server_send_message(sockets_array[my_attention], 1, response);
  //   }
  //   else if (msg_code == 2){ //El cliente le envía un mensaje al otro cliente
  //     char * client_message = server_receive_payload(sockets_array[my_attention]);
  //     printf("Servidor traspasando desde %d a %d el mensaje: %s\n", my_attention+1, ((my_attention+1)%2)+1, client_message);

  //     // Mi atención cambia al otro socket
  //     my_attention = (my_attention + 1) % 2;

  //     server_send_message(sockets_array[my_attention], 2, client_message);
  //   }
  //   printf("------------------\n");
  // }

  return 0;
}
