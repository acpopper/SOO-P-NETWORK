#include <unistd.h>
#include <stdio.h>
#include "conection.h"
#include "comunication.h"
#include "pthread.h"
#include "flow.h"




int main (int argc, char *argv[]){
  //Se obtiene la ip y el puerto donde está escuchando el servidor (la ip y puerto de este cliente da igual)
  char * IP = argv[2];
  int PORT = atoi(argv[4]);
  int connected = 0;
  // Se prepara el socket
  int server_socket = prepare_socket(IP, PORT); 
  connected = 1;
  while(connected){
      int msg_code = client_receive_id(server_socket);
      if(msg_code==2){
        lobby(server_socket);
      }
      else if(msg_code==3){
        notification_leader(server_socket);
      }
      else if(msg_code==4){
        catch_error_start_game(server_socket);
        sleep(1);
        //error: aun no se puede comenzar partida-> solo lo puede recibir el lider
      }
      else if(msg_code==5){
        //comienza el juego-> notificacion para todos los jugadores- 
        start_game(server_socket);
      }
      else if(msg_code == 100){
        printf("El número máximo de jugadores ya esta completo\n");
        connected =0;
      }
      
  }

  close(server_socket);

  return 0; 
    
  }
  
  


