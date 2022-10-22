//
// Created by mirunasav on 10/20/22.
//

#ifndef TEMA1_CLIENT_H
#define TEMA1_CLIENT_H

#include <cstdio>
#include "functions.h"
void handleResponse ( char * response, int serverSocket)
{
    char inBuffer[BUFFER_LENGTH];
    if(strcmp (response, "quit") == 0)
    exit(0);

    if(strstr(response, "PID_OK") == response)
    {
        for(int i = 1; i<= 5; i++)
        {
            readBuffer(serverSocket, &inBuffer);
            printf("%s\n", inBuffer);
            fflush(stdout);
        }
        return;
    }
    if(strstr(response, "PID_NOT_OK") == response)
    {
        printf("PID NOT OK!\n");
        return;
    }
    if(strstr(response, "USER OK") == response)
    {
        for(int i = 1; i <= 3; i++)
        {
            readBuffer(serverSocket, &inBuffer);
            printf("%s\n", inBuffer);
            fflush(stdout);
        }
        return;
    }
    printf("%s", response);
    fflush(stdout);


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
        handleResponse(inBuffer, server_socket);

        fflush(stdout);

    }
}

#endif //TEMA1_CLIENT_H
