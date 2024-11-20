#ifndef FILE_LIST_H
#define FILE_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>


#define BUFFER_SIZE 1024

void handle_list(int client_socket, const char* full_path);

#endif
