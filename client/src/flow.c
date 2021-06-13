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
void lobby(int server_socket){
    char * message = client_receive_payload(server_socket);
    printf("El servidor dice: %s\n", message);
    free(message);
    printf("Ingresar nombre de usuario:\n");
    char * name= get_input();
    client_send_message(server_socket, 0, name);
    printf("Elige una clase con la que deseas jugar\n");
    printf("(1) Cazador\n (2)Médico\n (3)Hacker\n");
    char* response= get_input();
    client_send_message(server_socket,1, response);
    free(response);
}

void notification_leader(int server_socket){
  char * message = client_receive_payload(server_socket);
  printf("Ha ingresado un nuevo jugador:%s\n", message);
  free(message);
  
  //preguntar si quiere iniciar el juego 
  //llegada de mensaje de error desde el servidor si es que falta alguien que guarde su nombre
  //hacer que elija el enemigo
  //enviar al servidor

}


