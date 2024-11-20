#ifndef UPLOAD_H
#define UPLOAD_H


void  upload_file(int socket);

void handle_mkdir(int socket, const char *dirname);

void handle_list(int socket);

void handle_delete(int socket);




#endif
