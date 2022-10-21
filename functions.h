//
// Created by mirunasav on 10/19/22.
//

#ifndef TEMA1_FUNCTIONS_H
#define TEMA1_FUNCTIONS_H
#include "definitions.h"
#include <cstring>


struct UserInfo{
    bool isLoggedIn = false;
    char username[BUFFER_LENGTH];
    char password[BUFFER_LENGTH];
};

struct ProcessInfo
{
    char processName[BUFFER_LENGTH];
    char processState[BUFFER_LENGTH];
    char PPID[BUFFER_LENGTH];
    char processUID[BUFFER_LENGTH];
    char processVMSIZE[BUFFER_LENGTH];
};

int readBuffer(int fileDescriptor, void *pBuf)
{
    size_t bufferLength = 0;
    memset(pBuf, 0, BUFFER_LENGTH);

    if(read(fileDescriptor, &bufferLength, sizeof(size_t)) < 0 )
        return 0;

    if( read(fileDescriptor, pBuf, bufferLength) < 0 )
        return 0;
    return 1;

}

int writeBuffer( int fileDescriptor, const char* pBuf)
{
    size_t bufferLength = strlen(pBuf);

    if(write(fileDescriptor,&bufferLength, sizeof(size_t)) < 0)
        return 0;

    if(write(fileDescriptor,pBuf, bufferLength) < 0 )
        return 0;

    return 1;


}

int readCommand(void * pBuf)
{
    memset(pBuf, 0, BUFFER_LENGTH);
   if( read(0, pBuf, BUFFER_LENGTH) < 0 )
       return 0;
   //facem abstract de enterul de la final
    char *pEnd;
    pEnd = strchr((char*)pBuf, '\n');
    if(pEnd != nullptr)
        *pEnd = '\0';

   return 1;

}
void separateCommand(char * clientRequest, char *command)
{

    command = strtok (clientRequest, " ");

    clientRequest = strtok ( NULL,  " ");

}
char * separateUserInfoParameter(char *clientRequest)
{
    char * p = strstr(clientRequest, " : ");
    char * parameter = nullptr;
    p+=3;
    parameter = strtok(p ," ");
    return parameter;
}
bool loginFunction(char *pBuf, UserInfo& user)
{
    //taiem bucata cu "login : "
    char *p = strstr(pBuf, " : ");
    p+=3;
    char *username = nullptr, *password = nullptr;
    username = strtok(p, " ");
    password = strtok(NULL, "\0");

    //acum tb sa caut userul peste tot prin fisier; daca gasesc userul
    //pe vreo linie, verific si parola

    FILE *filePointer = fopen ("username_passwords", "r");
    char lineInFile[BUFFER_LENGTH];
    if (filePointer == nullptr)
        -printf("eroare la deschiderea fisierului cu usernames & passwords\n");
    while(!feof(filePointer))
   {
       fgets(lineInFile, BUFFER_LENGTH, filePointer);
       //scoatem '\n' de la finalul fiecarei linii
       char *pEnd;
       pEnd = strchr((char*)lineInFile, '\n');
       if(pEnd != nullptr)
           *pEnd = '\0';

       //pe o linie avem si user si parola deci facem strtok
       char *user_from_file, *password_from_file;

       user_from_file = strtok(lineInFile, " ");
       password_from_file = strtok(NULL, " ");
      // printf("user %s parola %s\n", user_from_file, password_from_file);
       if(strcmp(user_from_file, username) == 0 )
           if(strcmp(password_from_file,password) == 0)
           {
               memset(user.password, 0, BUFFER_LENGTH);
               memset(user.password, 0, BUFFER_LENGTH);
              strcpy( user.username , username );
              strcpy( user.password , password );

               return true;
           }

   }
    return false;

}

void printError(const char * errorMessage)
{
    printf("%s", errorMessage);
}
#endif //TEMA1_FUNCTIONS_H
//login : user1 parola1