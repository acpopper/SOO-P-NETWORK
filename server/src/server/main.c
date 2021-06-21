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
juego* GAME;
bool game_exists=true;
entity* monstruo;

void * handle_connection(void *p_client_socket){
  int client_socket = *((int*)p_client_socket);
  welcome_client(client_socket);
  while(game_exists){
    int msg_code = server_receive_id(client_socket);
    // printf("code %i skt %i\n", msg_code, client_socket);
    if(msg_code == 0){
      add_name(entities,client_socket);
    }
    else if(msg_code == 1){
      add_type(entities, client_socket);
    }
    else if(msg_code==2){
      // instancia GAME, se crea el juego si todo ok, si no se vuelve a preguntar y no inicia
      char *msg = server_receive_payload(client_socket);
      int type = atoi(msg);
      free(msg);
      
      if(type){
        monstruo = new_monster(type);
      }
      start_game=game(entities, client_socket, actual_connections, type, monstruo);
      if(start_game){
        GAME = init_game(entities, monstruo, actual_connections);
        turno_pro(GAME, client_socket);
        // server_send_message(client_socket, 111, "");
      }
    }
    
    // else if(msg_code == 10){ 
    //   // toca el turno de client socket
    //   if(*GAME->battle_going){
    //     turno_pro(GAME, client_socket);
    //   }
    // }
    // else if(msg_code == 70) {
    //   if(*GAME->battle_going){
    //     action_selection(GAME, client_socket);
    //   }
    // }
    else if(msg_code == 77){
      bool second = action_selection(GAME, client_socket);
      if(*GAME->battle_going && *(GAME->rounds)%*(GAME->amt_of_players)==0 && !second){
        monster_attack(GAME);
      }
      if(*GAME->battle_going && !second){
        // Se pasa turno, y se manda señal al siguiente jugador en el array de que le toca
        int sig_turno = GAME->next_player;
        *GAME->battle_going = pasar_turno(GAME);
        if (!*GAME->battle_going) {
          printf("Juego terminado.\n");
          notify_all(GAME, "Juego terminado\n¡Haz ganado!\nGracias por jugar :D\n");
          start_game=false;
          *(GAME->amt_of_players)=0;
          for(int i=0; i<4; i++){
            if (GAME->players[i] != NULL) {
              close(GAME->players[i]->jugador->client_socket);
            }
          }
          free_game(GAME);
          exit(0);
        }
        turno_pro(GAME, GAME->players[sig_turno]->jugador->client_socket);
        // server_send_message(GAME->players[sig_turno]->jugador->client_socket, 111, "");
      }
      else if(!(*GAME->battle_going)){
        printf("Juego terminado.\n");
        notify_all(GAME, "Juego terminado. Gracias por jugar!\n");

        start_game=false;
        *(GAME->amt_of_players)=0;
        for(int i=0; i<4; i++){
          close(GAME->players[i]->jugador->client_socket);
        }
        free_game(GAME);
        exit(0);
      }
    }
    else if(msg_code == 81){
      set_target(GAME, client_socket);
      if(*GAME->battle_going && *(GAME->rounds)%*(GAME->amt_of_players)==0){
        monster_attack(GAME);
      }
      if(*GAME->battle_going){
        // Se pasa turno, y se manda señal al siguiente jugador en el array de que le toca
        int sig_turno = GAME->next_player;
        *GAME->battle_going = pasar_turno(GAME);
        // printf("Siguiente turno %i\n", sig_turno);
        turno_pro(GAME, GAME->players[sig_turno]->jugador->client_socket);
        // server_send_message(GAME->players[sig_turno]->jugador->client_socket, 111, "");
      }
      else if(!(*GAME->battle_going)){
        printf("Juego terminado.\n");
        notify_all(GAME, "Juego terminado. Gracias por jugar!\n");

        start_game=false;
        *(GAME->amt_of_players)=0;
        for(int i=0; i<4; i++){
          if (GAME->players[i] != NULL) {
            close(entities[i]->jugador->client_socket);
          }
        }
        free_game(GAME);
        exit(0);
      }
    }
  }
  exit(0);
}

int main(int argc, char *argv[]){
  char * IP = argv[2];
  int PORT = atoi(argv[4]);
  int server_socket = setup_server(IP, PORT);
  entities = calloc(4,sizeof(entity)); // array de jugadores
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
  // *NOTA* free_game ya hace free a entities
  // for(int i=0; i<4; i++){
  //   if(entities[i] != 0){
  //     free_entity(entities[i]);
  //   }
  // }
  // free(entities);
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

