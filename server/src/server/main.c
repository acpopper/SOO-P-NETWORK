#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "../coms/comunication.h"
#include "../coms/conection.h"
#include "../flow/flow.h"
#include "../entities/chars.h"

int max_connections = 4;
int actual_connections = 0; //size de entities
entity** entities;
bool start_game = false;

void * handle_connection(void *p_client_socket){
  int client_socket = *((int*)p_client_socket);
  welcome_client(client_socket);
  while(1){
    int msg_code = server_receive_id(client_socket);
    if(msg_code == 0){
      add_name(entities,client_socket);
    }
    else if(msg_code == 1){
      add_type(entities, client_socket);
    }
    else if(msg_code==2){
      // start_game=game(entities, client_socket, actual_connections);
      if (start_game){
        turno(entities, client_socket);
      }
      //comenzar juego, solo lo puede enviar si es el lider
    }
    else if(msg_code == 3){ 
      char *msg = server_receive_payload(client_socket);
      int action = atoi(msg);
      if (action == 0){
        // Rendirse
      }
      else{
        // Encuentra las entities y usa la funcion use_ability que modifica los valores de las entidades
        entity* user;
        entity* target;
        user = select_user(client_socket, entities);
        target = select_target(client_socket, entities, action, user->type);
        printf("User: %s Target: %s\n", user->jugador->nombre, target->type);
        // use_ability(user, target, user->jugador->ability1_name, entities, actual_connections);
      }
    }
    else if(msg_code ==-1){
      //cliente pide desconectarse
      //eliminar cliente del array
      //actualizar actual_connection ojo: que es un thread-> hacer sincronizacion
      //hacer free del cliente
      //cerrar socket
      //break;
    }  
  }
  exit(0);
}

int main(int argc, char *argv[]){
  char * IP = argv[2];
  int PORT = atoi(argv[4]);
  int server_socket = setup_server(IP, PORT);
  entities = calloc(5,sizeof(entity)); //son 5 porque el ultimo es el monster
  while (1){
    if (actual_connections<max_connections && !start_game){
      printf("Esperando conexiones...\n");
      int client_socket = accept_new_connection(server_socket);
      for(int i=0; i<4; i++){
        if(entities[i]==0){
          if(actual_connections==0){
            entities[i] = new_entity(true, client_socket,true,"","");
          } else{
            entities[i] = new_entity(false, client_socket,true,"","");
            
          }
          break;
        }
      }
      actual_connections++;
      printf("Nuevo cliente conectado\n");
      pthread_t client;
      int *pclient = malloc(sizeof(int));
      *pclient = client_socket;
      pthread_create(&client, NULL, handle_connection,pclient);
    } else{
      int client_socket = accept_new_connection(server_socket);
      server_send_message(client_socket, 100,"");
      close(client_socket);
    }
  }
  for(int i=0; i<5; i++){
    if(entities[i] != 0){
      free_entity(entities[i]);
    }
  }
  free(entities);
  return 0;

  // entity** players = calloc(4, sizeof(player*));
  // int player_amt = 3;
  // entity* p1 = new_entity(false, 1, true, "juan", "");
  // entity* p2 = new_entity(false, 2, true, "fco", "");
  // entity* p3 = new_entity(false, 3, true, "pedro", "");
  // type_entity_player(p1, 1);
  // type_entity_player(p2, 2);
  // type_entity_player(p3, 3);
  // // entity* p4 = new_entity(true, 4, true, "juan", "Cazador");
  // players[0] = p1;
  // // add_name(players, 1);
  // players[1] = p2;
  // players[2] = p3;
  // // players[3] = p4;
  // entity* m1 = new_monster(3);
  // juego* juego = init_game(players, m1, player_amt);

  // start_battle(juego);
  // free_game(juego);
}

