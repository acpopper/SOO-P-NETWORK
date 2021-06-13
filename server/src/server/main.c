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

void * handle_connection(void *p_client_socket){
  int client_socket = *((int*)p_client_socket);
  free(p_client_socket);
  welcome_client(client_socket);
  while(1){
    int msg_code = server_receive_id(client_socket);
    if(msg_code == 0){
      add_name(entities,client_socket);
    }
    else if(msg_code == 1){
      add_type_and_notify_leader(entities, client_socket);
    }
    else if(msg_code ==-1){
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
    if (actual_connections<max_connections){
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
    }
  }
  return 0;
}

