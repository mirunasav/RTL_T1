//
// Created by mirunasav on 10/20/22.
//

#ifndef TEMA1_SERVER_H
#define TEMA1_SERVER_H

#include <sys/stat.h>
#include "functions.h"

int init_server()
{
   // char server_message [BUFFER_LENGTH] = "You have reached the server!\n";

    int s = socket(AF_INET, SOCK_STREAM, 0);

    int toggle = 1;
    setsockopt (
            s,
            SOL_SOCKET,
            SO_REUSEADDR,
            & toggle,
            sizeof ( toggle )
    );

    //define the server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(34000);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    //bind the socket to our specified IP and port
    bind(s, (struct sockaddr * ) &server_address, sizeof(server_address));
    printf("Serverul a pornit\n");
    return s;

}
void handleLoginRequest( int client_socket, char * request, char * response, UserInfo& userInfo)
//cu pipe
{
    if(userInfo.isLoggedIn)
    {
        writeBuffer(client_socket,"You are already logged in!\n");
        return;
    }

    int parentToChild[2];
    int childToParent[2];
    char messageFromChild[BUFFER_LENGTH];

    if(pipe(parentToChild) < 0 )
        printError("Eroare la creare pipe\n");
    if (pipe(childToParent ) < 0)
        printError("Eroare la creare pipe\n");

    pid_t childPid = fork();

    if (childPid < 0 )
        printError("eroare la fork!\n");
    if(childPid != 0 ) //in parinte
    {
        close(childToParent[WRITE]);

        int status;
        waitpid(childPid, &status, 0);
        if(!WIFEXITED(status))
            printf("eroare la copilul care a facut login\n");
        readBuffer(childToParent[READ], &messageFromChild);

        if(strcmp(messageFromChild , "yes" ) == 0 ) {
            userInfo.isLoggedIn = true;

            fflush(stdout);

            strcpy(response, "You are logged in! \n Possible commands: \n logout \n quit \n"
                             " get-logged-users \n get-proc-info : pid\n");

            writeBuffer(client_socket,response);

            memset(response, 0, BUFFER_LENGTH);

            fflush (stdout);
        }

        if(strcmp(messageFromChild, "no") == 0)
            writeBuffer(client_socket,"Login failed!\n");
        if(strcmp(messageFromChild, "already logged in") == 0)
            writeBuffer(client_socket,"You are already logged in!\n");
        close(childToParent[READ]);


    }
    else //in copil
    {
        close(childToParent[READ]);

        switch (loginFunction(request, userInfo)) {
            case true:
                if(userInfo.isLoggedIn == true )
                {
                    writeBuffer(childToParent[WRITE], "already logged in");
                    close(childToParent[WRITE]);
                    exit(0);
                }
                else //daca nu era logat deja
                {
                    printf("logarea s-a realizat cu succes!\n");
                    if (writeBuffer(childToParent[WRITE], "yes"))
                        exit(1);
                    close(childToParent[WRITE]);

                    exit(0);
                }
                break;
            case false:
                printf("logarea a esuat\n");
                if(writeBuffer(childToParent[WRITE], "no") < 0 )
                    exit(1);
                close(childToParent[WRITE]);
                exit(0);

        }

    }

}
void handleLogoutRequest ( int client_socket, char const* request, char * response, UserInfo& user)
//cu pipe
{
    int parentChildPipe[2];
    if( pipe (parentChildPipe ) == -1)
    {
        printf("eroare la pipe in handleLogoutRequest!\n");
    }

    pid_t childPid = fork();
    if (childPid < 0 )
    {
        printf("eroare la fork() in handleLogoutRequest!\n");
    }
    if(childPid ==0 ) //in copil
    {
        close(parentChildPipe[READ]);

        char childMessage[BUFFER_LENGTH];
        if(user.isLoggedIn)
            writeBuffer(parentChildPipe[WRITE], "LOGOUT");
        else
            writeBuffer(parentChildPipe[WRITE], "NOT LOGGED IN YET");

        close(parentChildPipe[WRITE]);
        exit(0);
    }
    else
    {
        close(parentChildPipe[WRITE]);

        int status;
        char messageFromChild[BUFFER_LENGTH];
        waitpid(childPid, &status, 0);
        if(!WIFEXITED(status))
            printf("eroare la copilul care a facut login\n");
        readBuffer(parentChildPipe[READ], &messageFromChild);
        if(strcmp(messageFromChild , "LOGOUT" ) == 0 ) {
            user.isLoggedIn = false;
            strcpy(response,"You are logged out!\n");
            writeBuffer(client_socket, response);
            fflush(stdout);
        }

        if(strcmp(messageFromChild, "NOT LOGGED IN YET") == 0)
        {
            strcpy(response,"You cannot logout!\n");
            writeBuffer(client_socket,response);
            fflush(stdout);
        }
        close(parentChildPipe[READ]);
    }



}

void  handleQuitRequest (int client_socket, char* request,char*  response,UserInfo& user)
{
    writeBuffer(client_socket, "quit");
}
void handleRequest(int client_socket, char * request,  char * response, UserInfo&  user)
{
    if(!user.isLoggedIn)
    {
        if (strstr(request, "login : "))
        {
            handleLoginRequest(client_socket, request, response, user);
        }
        else
        {
            writeBuffer(client_socket, "You need to login first!\n");
        }
    }
    else //if the client is already logged in
    {
        if (strstr(request, "login : "))
        {
            writeBuffer(client_socket, "You are already logged in!\n");
        }
        if(strstr(request, "logout"))
        {
            handleLogoutRequest(client_socket,request, response, user);
        }
        if(strstr(request, "quit"))
        {
            handleQuitRequest(client_socket, request, response, user);
        }

    }



}

void server_loop(int client_socket)
{
    char clientRequest[BUFFER_LENGTH], serverResponse[BUFFER_LENGTH],
            consoleBuffer[BUFFER_LENGTH];
    UserInfo userInfo;

    while(1)
    {
        readBuffer(client_socket,&clientRequest );
        handleRequest(client_socket,clientRequest, serverResponse, userInfo);
        fflush (stdout);

    }

}


#endif //TEMA1_SERVER_H
