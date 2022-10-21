#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/wait.h>
#include "functions.h"
#include "definitions.h"
#include "server.h"

int main()
{

    int s = init_server();

    listen(s, 5); //nr din dreapta nu e asa relevant pt ca in acest caz lucram cu un singur client

    struct sockaddr_in client_address;
    socklen_t length = sizeof(struct sockaddr_in);

    int client_socket = accept(s, (sockaddr * ) &client_address, &length);

    if(client_socket < 0)
        printf("nu  s a realizat ok conexiunea");

    server_loop(client_socket);
//    while(1)
//    {
//        readBuffer(client_socket, &inBuffer);
//
//        if (strstr(inBuffer, "login : ")) {
//            //cu pipe
//            {
//                int parentToChild[2];
//                int childToParent[2];
//                char messageFromChild[BUFFER_LENGTH];
//
//                if(pipe(parentToChild) < 0 )
//                    printError("Eroare la creare pipe\n");
//                if (pipe(childToParent ) < 0)
//                    printError("Eroare la creare pipe\n");
//
//                pid_t childPid = fork();
//
//                if (childPid < 0 )
//                    printError("eroare la fork!\n");
//                if(childPid != 0 ) //in parinte
//                {
//                    close(childToParent[WRITE]);
//
//                    int status;
//                    waitpid(childPid, &status, 0);
//                    if(!WIFEXITED(status))
//                        printf("eroare la copilul care a facut login\n");
//                    readBuffer(childToParent[READ], &messageFromChild);
//                    if(strcmp(messageFromChild , "yes" ) == 0 ) {
//                        userInfo.isLoggedIn = true;
//                        writeBuffer(client_socket, "You are logged in!\n");
//                    }
//
//                    if(strcmp(messageFromChild, "no") == 0)
//                        writeBuffer(client_socket,"Login failed!\n");
//                    if(strcmp(messageFromChild, "already logged in") == 0)
//                        writeBuffer(client_socket,"You are already logged in!\n");
//                    close(childToParent[READ]);
//
//
//                }
//                else //in copil
//                {
//                    close(childToParent[READ]);
//
//                    switch (loginFunction(inBuffer, userInfo)) {
//                        case true:
//                            if(userInfo.isLoggedIn == true )
//                            {
//                                writeBuffer(childToParent[WRITE], "already logged in");
//                                close(childToParent[WRITE]);
//                                exit(0);
//                            }
//                            else //daca nu era logat deja
//                            {
//                                printf("logarea s-a realizat cu succes!\n");
//                                if (writeBuffer(childToParent[WRITE], "yes"))
//                                    exit(1);
//                                close(childToParent[WRITE]);
//
//                                exit(0);
//                            }
//                            break;
//                        case false:
//                            printf("logarea a esuat\n");
//                            if(writeBuffer(childToParent[WRITE], "no") < 0 )
//                                exit(1);
//                            close(childToParent[WRITE]);
//                            exit(0);
//
//                }
//
//                }
//
//            }
//        }
//        continue;
        //exit(0);
  //  }

//    if(! writeBuffer(client_socket, server_message))
//        printf("eroare la scriere in server\n");



    //return 0;
}