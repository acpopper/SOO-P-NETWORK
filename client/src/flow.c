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
    choose_monster(server_socket);  
  }else if(input==0){
    //debo enviar mensaje al servidor porque qué pasa si el lider no desea comenzar pero ya está el máx de jugadores
    printf("Esperando más jugadores...\n");
    client_send_message(server_socket,3,"0");
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
  client_send_message(server_socket, 2, response);
  free(response); 
}

void start_game(int server_socket){
  char * message = client_receive_payload(server_socket);
  printf("¡Que el juego comience! Su enemigo es: %s\n", message);
  free(message);
  //crear una funcion que se llame cada vez que sea el turno del jugador
}

void game_end(int server_socket){
  char * message = client_receive_payload(server_socket);
  printf("%s", message);
  free(message);
}

void print_situacion(int server_socket){
  char * message = client_receive_payload(server_socket);
  printf("================== SITUACIÓN ACTUAL ==================\n");
  printf(" %s", message);
  printf("======================================================\n");
  free(message);
  //crear una funcion que se llame cada vez que sea el turno del jugador
}

void select_action(int server_socket){
  char *msg = client_receive_payload(server_socket);
  printf("%s", msg);
  free(msg);
  char* response = get_input();
  client_send_message(server_socket, 77, response);
  free(response); 
}

void select_action_cazador(int server_socket){
  printf("Es tu turno, ¿Qué deseas hacer?\n");
  printf(" (0) Rendirse\n (1) Estocada\n (2) Corte Cruzado\n (3) Distraer\n");
  char* response = get_input();
  client_send_message(server_socket, 10, response);
  free(response);
}

void select_action_medico(int server_socket){
  printf("Es tu turno, ¿Qué deseas hacer?\n");
  printf(" (0) Rendirse\n (1) Curar\n (2) Destello Regenerador\n (3) Descarga Vital\n");
  char* response = get_input();
  client_send_message(server_socket, 10, response);
  free(response);
}

void select_action_hacker(int server_socket){
  printf("Es tu turno, ¿Qué deseas hacer?\n");
  printf(" (0) Rendirse\n (1) Inyección SQL\n (2) Ataque DDOS\n (3) Fuerza Bruta\n");
  char* response = get_input();
  client_send_message(server_socket, 10, response);
  free(response);
}

void select_target(int server_socket){
  char *msg = client_receive_payload(server_socket);
  printf("%s", msg);
  free(msg);
  char* response = get_input();
  client_send_message(server_socket, 81, response);
  free(response);
}

void display_players(int server_socket){
  printf("Selecciona sobre quien usar la habilidad:\n");
  char * message = client_receive_payload(server_socket);
  printf("================= LISTA DE JUGADORES =================\n");
  printf("%s", message);
  printf("======================================================\n");
  free(message);
  char* response = get_input();
  // al parecer da lo mismo el code del send que sigue...
  // se recibe solo payload y parece funcionar
  client_send_message(server_socket, 70, response);
  free(response);
}



