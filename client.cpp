#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include "functions.h"
#include "definitions.h"
#include "client.h"

int main() {
    char inBuffer[BUFFER_LENGTH], outBuffer[BUFFER_LENGTH],
            consoleBuffer[BUFFER_LENGTH];
    //cream socketul
    int s = socket(AF_INET, SOCK_STREAM, 0);

    //specificam adresa socketului
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(34000);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");


    int connection_status = connect(s, (struct sockaddr *) &server_address, sizeof(server_address));
    if (connection_status == -1) {
        printf("Eroare la conexiunea cu serverul\n");
        exit(-1);
    }

    printf("Welcome! Please login: \n");

    client_loop(s);


    return 0;
}
