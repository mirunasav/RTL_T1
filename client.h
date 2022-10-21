//
// Created by mirunasav on 10/20/22.
//

#ifndef TEMA1_CLIENT_H
#define TEMA1_CLIENT_H

#include <cstdio>
#include "functions.h"
void handleResponse ( char * response)
{
    if(strcmp (response, "quit") == 0)
        exit(0);
}
void client_loop(int server_socket)
{
    char inBuffer[BUFFER_LENGTH], outBuffer[BUFFER_LENGTH],
            consoleBuffer[BUFFER_LENGTH];
    while(1)
    {
        if(!readCommand(&consoleBuffer))
            printf("eroare la citirea comenzii in client\n");
        strcpy(outBuffer, consoleBuffer);
        writeBuffer(server_socket, outBuffer);
        readBuffer(server_socket, &inBuffer);
        handleResponse(inBuffer);
        printf("%s", inBuffer);
        fflush(stdout);

    }
}

///CUM FAC SA PORNESC IAR CLIENTUL DUPA CE AM DAT QUIT?
#endif //TEMA1_CLIENT_H
