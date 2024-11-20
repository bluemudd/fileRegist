#ifndef FILE_MKDIR_H
#define FILE_MKDIR_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>


#define BUFFER_SIZE 1024

void handle_mkdir(int client_socket, const char* received_command);

#endif

