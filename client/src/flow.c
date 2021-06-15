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
    printf("Ingresar nombre de usuario:\n");
    char * name= get_input();
    client_send_message(server_socket, 0, name);
    printf("Elige una clase con la que deseas jugar\n");
    printf("(1) Cazador\n (2)Médico\n (3)Hacker\n");
    char* response= get_input();
    client_send_message(server_socket,1, response);
}

void notification_leader(int server_socket){
  char * message = client_receive_payload(server_socket);
  printf("Ha ingresado un nuevo jugador: %s\n", message);
  printf("¿Deseas comenzar la partida?\n (0) No\n (1) Sí\n");
  char* ipt = get_input();
  int input = atoi(ipt);
  if(input==1){
    printf("¿Con qué monstruo deseas combatir?\n (1) Great JagRuz\n (2) Ruzalos\n (3) Ruiz, el Gemelo Malvado del Profesor Ruz\n");
    char* response = get_input();
    client_send_message(server_socket,2 ,response);  
  }else if(input==0){
    printf("Esperando más jugadores...\n");
  }
}
void catch_error_start_game(int server_socket){
  printf("Hay jugadores que aun no ingresan su nombre y/o clase...\n");

}
void start_game(int server_socket){
  char * message = client_receive_payload(server_socket);
  printf("¡Que el juego comience! Su enemigo es: %s\n", message);
  //crear una funcion que se llame cada vez que sea el turno del jugador
}

void print_situacion(int server_socket){
  char * message = client_receive_payload(server_socket);
  printf("================== SITUACIÓN ACTUAL ==================\n");
  printf(" %s", message);
  printf("======================================================\n");
  free(message);
  //crear una funcion que se llame cada vez que sea el turno del jugador
}

void select_action_cazador(int server_socket){
  printf("Es tu turno, ¿Qué deseas hacer?\n");
  printf(" (0) Rendirse\n (1) Estocada\n (2) Corte Cruzado\n (3) Distraer\n");
  char* response = get_input();
  client_send_message(server_socket, 3, response);
  free(response);
}

void select_action_medico(int server_socket){
  printf("Es tu turno, ¿Qué deseas hacer?\n");
  printf(" (0) Rendirse\n (1) Curar\n (2) Destello Regenerador\n (3) Descarga Vital\n");
  char* response = get_input();
  client_send_message(server_socket, 3, response);
  free(response);
}

void select_action_hacker(int server_socket){
  printf("Es tu turno, ¿Qué deseas hacer?\n");
  printf(" (0) Rendirse\n (1) Inyección SQL\n (2) Ataque DDOS\n (3) Fuerza Bruta\n");
  char* response = get_input();
  client_send_message(server_socket, 3, response);
  free(response);
}



