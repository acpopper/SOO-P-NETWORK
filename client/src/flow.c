#include "flow.h"

void lobby(int server_socket){
    char * message = client_receive_payload(server_socket);
    printf("El servidor dice: %s\n", message);
    free(message);
    //pedir nombre y enviarlo al servidor
}