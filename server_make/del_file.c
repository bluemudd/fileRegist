#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <dirent.h>

#define BUFFER_SIZE 1024

void handle_delete(int client_socket, const char * full_path){

   	char file_path[BUFFER_SIZE];
    	char file_path_name[BUFFER_SIZE];
    	char buffer[BUFFER_SIZE];
    	char client_response[BUFFER_SIZE];
	// ready path
    	snprintf(file_path, sizeof(file_path), "%s", full_path);
    
    	send(client_socket, "READY", strlen("READY"),0);
    

	int client_length = recv(client_socket, client_response, sizeof(client_response)-1, 0);
	if (client_length <= 0) {
	   
	    perror("Error receiving file name from client");
	    return;
	} else {
	    client_response[client_length] = '\0';
	    printf("Received file name from client: %s\n", client_response);
	}
	
	//int bytes_receiver = recv(client_socket, buffer, sizeof(buffer), 0);
	//buffer[bytes_receiver] = '\0';
	
	snprintf(file_path_name, sizeof(file_path_name), "%s/%s", full_path, client_response);
	printf("%s\n", file_path_name);	

	if(remove(file_path_name) == 0){
		send(client_socket, "File deleted sucessfully", 25, 0);
	} else{
		perror("remove");
		send(client_socket, "Failed to delete file", 22, 0);
	}


}
