#ifndef FILE_DELETE_H
#define FILE_DELETE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>


#define BUFFER_SIZE 1024

void handle_delete(int client_socket, const char *full_path);

#endif
