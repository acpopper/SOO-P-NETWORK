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
  printf("Ha ingresado un nuevo jugador: %s\n", message);
  free(message);
  printf("¿Deseas comenzar la partida?\n (0) No\n (1) Sí\n");
  char* ipt = get_input();
  int input = atoi(ipt);
  free(ipt);
  if(input==1){
    choose_monster(server_socket);  
  }else if(input==0){
    //debo enviar mensaje al servidor porque qué pasa si el lider no desea comenzar pero ya está el máx de jugadores
    printf("Esperando más jugadores...\n");
    client_send_message(server_socket,2,"0");
  }
}
void catch_error_start_game(int server_socket){
  char* error =  client_receive_payload(server_socket);
  int er = atoi(error);
  free(error);
  if (error){
    printf("Esta el máximo de jugadores conectados ¡Es hora de comenzar a jugar!\n");
    choose_monster(server_socket);
  } else{
    printf("Hay jugadores que aun no ingresan su nombre y/o clase...\n");
  }
}

void choose_monster(int server_socket){
  printf("¿Con qué monstruo deseas combatir?\n (1) Great JagRuz\n (2) Ruzalos\n (3) Ruiz, el Gemelo Malvado del Profesor Ruz\n");
  char* response = get_input();
  client_send_message(server_socket,2 ,response);
  free(response); 
}

void start_game(int server_socket){
  char * message = client_receive_payload(server_socket);
  printf("¡Que el juego comience! Su enemigo es: %s\n", message);
  free(message);
  //crear una funcion que se llame cada vez que sea el turno del jugador
}





