#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "../coms/comunication.h"
#include "../coms/conection.h"
#include "../flow/flow.h"

int max_connections = 4;
void * handle_connection(void *p_client_socket){
  int client_socket = *((int*)p_client_socket);
  //hay que agregarlo a la estructura de clientes y crear entity 
  free(p_client_socket);
  welcome_client(client_socket);
  while(1){
    int msg_code = server_receive_id(client_socket);
    if(msg_code == 0){
      
    }
    else if(msg_code ==-1){
      //eliminar cliente
      //matar thread
    }
    
  }

}

int main(int argc, char *argv[]){
  int actual_connections = 0;
  char * IP = argv[2];
  int PORT = atoi(argv[4]);
  int server_socket = setup_server(IP, PORT);
  while (1){
    if (actual_connections<max_connections){
      printf("Esperando conexiones...\n");
      int client_socket = accept_new_connection(server_socket);
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

