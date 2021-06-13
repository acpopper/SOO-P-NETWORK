#include <unistd.h>
#include <stdio.h>
#include "conection.h"
#include "comunication.h"
#include "pthread.h"
#include "flow.h"


char * get_input(){
  char * response = malloc(20);
  int pos=0;
  while (1){
    char c = getchar();
    if (c == '\n') break;
    response[pos] = c;
    pos++;
  }
  response[pos] = '\0';
  return response;
}




int main (int argc, char *argv[]){
  //Se obtiene la ip y el puerto donde está escuchando el servidor (la ip y puerto de este cliente da igual)
  char * IP = argv[2];
  int PORT = atoi(argv[4]);
  int connected = 0;
  // Se prepara el socket
  int server_socket = prepare_socket(IP, PORT); 
  if(server_socket!=-1){ //si hay mas de 4 no se debe aceptar la conexion
    connected=1;
    
    while(connected){
      int msg_code = client_receive_id(server_socket);
      if(msg_code==0){
        lobby(server_socket);
      }
    }
    
  }
  
  close(server_socket);

  return 0;
}

