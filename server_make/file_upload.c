#include "file_upload.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#define BUFFER_SIZE 1024

void handle_upload(int client_socket, const char *full_path) {
    char file_path[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];
    char client_response[BUFFER_SIZE];
    FILE *file;
    // 파일 경로 구성
    snprintf(file_path, sizeof(file_path), "%s", full_path);

    // 클라이언트에게 준비 상태 전송
    send(client_socket, "READY", strlen("READY"), 0);
       // 파일 열기
    
	int client_length = recv(client_socket, client_response, sizeof(client_response)-1, 0);
	if (client_length <= 0) {
	    // 에러가 발생하거나 클라이언트 연결이 끊긴 경우
	    perror("Error receiving file name from client");
	    return;
	} else {
	    client_response[client_length] = '\0';  // 문자열 종료
	    printf("Received file name from client: %s\n", client_response);

	    // 파일을 "uploaded_file.txt"로 저장
	    snprintf(file_path, sizeof(file_path), "%s/%s", full_path, client_response);
	    printf("Saving file as: %s\n", file_path);
	}

    file = fopen(file_path, "wb");
    if (file == NULL) {
        fprintf(stderr, "File path: %s\n", file_path);
        perror("Error opening file for upload");
        send(client_socket, "Error: Failed to open file\n", 27, 0);
        return;
    }
    
    // 파일 데이터 수신
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
            buffer[bytes_received] = '\0'; // 문자열 종료
        if (strncmp(buffer, "EOF", 3) == 0) {
            send(client_socket, "RECIEVED", strlen("RECEIVED"), 0);
        }
        fwrite(buffer, 1, bytes_received, file); // 데이터 저장
    	fclose(file);
     printf("File '%s' uploaded successfully.\n", file_path);

    send(client_socket, "File upload successful\n", 23, 0);

}
