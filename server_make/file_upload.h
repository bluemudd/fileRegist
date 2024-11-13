#ifndef FILE_UPLOAD_H
#define FILE_UPLOAD_H


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>


#define BUFFER_SIZE 1024


void handle_upload(int client_sock);


#endif
