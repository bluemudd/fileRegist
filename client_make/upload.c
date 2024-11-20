#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "client.h"

#define PORT 8080
#define BUFFER_SIZE 1024


void upload_file(int socket) {
    char file_name[256];
    char buffer[BUFFER_SIZE];
    char server_response[BUFFER_SIZE];
    FILE *file;

    // 서버로 명령어 전송
    snprintf(buffer, sizeof(buffer), "upload");
    send(socket, buffer, strlen(buffer), 0);
    
    memset(server_response, 0, sizeof(server_response));  // 버퍼를 0으로 초기화
        // 서버의 준비 메시지 대기
    int response_length = recv(socket, server_response, sizeof(server_response) - 1, 0);
    if (response_length <= 0 || strncmp(server_response, "READY", 5) != 0) {
        printf("Server not ready: %s\n", server_response);
        fclose(file);
        return;
    }
    else{
    	printf("Server ready!: %s\n", server_response);
    }

    // 업로드할 파일 이름 입력
    printf("Enter the file name to upload: ");
    if (fgets(file_name, sizeof(file_name), stdin) == NULL) {
        perror("Error reading file name");
        return;
    }

    file_name[strcspn(file_name, "\n")] = '\0'; // Newline 제거
    
    if(strlen(file_name) == 0){
    	fprintf(stderr, "File name cannot be Empty.\n");
    	return;
    }
    if(send(socket, file_name, strlen(file_name), 0)<0){
    	perror("Error sending file name");
    	return;
    }

    // 파일 열기
    file = fopen(file_name, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    // 파일 전송
    while (!feof(file)) {
        size_t bytes_read = fread(buffer, 1, sizeof(buffer), file);
        send(socket, buffer, bytes_read, 0);
    }

    // EOF 전송
    send(socket, "EOF", 3, 0);

    fclose(file);

    // 서버의 업로드 완료 메시지 확인
    response_length = recv(socket, server_response, sizeof(server_response) - 1, 0);
    if (response_length > 0) {
        server_response[response_length] = '\0';
        printf("Server response: %s\n", server_response);
    }
}
void handle_mkdir(int socket, const char *dirname) {
    char buffer[BUFFER_SIZE];
    snprintf(buffer, BUFFER_SIZE, "mkdir %s", dirname);
    send(socket, buffer, strlen(buffer), 0);
    recv(socket, buffer, BUFFER_SIZE, 0);
    printf("Server response: %s\n", buffer);
}

void handle_list(int socket) {
    char buffer[BUFFER_SIZE];
    snprintf(buffer, BUFFER_SIZE, "list");
    send(socket, buffer, strlen(buffer), 0);
    recv(socket, buffer, BUFFER_SIZE, 0);
    printf("Server response:\n%s\n", buffer);
}

void handle_delete(int socket) {
    char buffer[BUFFER_SIZE];
    char file_name[256];
    char server_response[BUFFER_SIZE];
    
    snprintf(buffer, sizeof(buffer), "delete");
    send(socket, buffer, strlen(buffer), 0);
    
    memset(server_response, 0, sizeof(server_response));  // 버퍼를 0으로 초기화
        // 서버의 준비 메시지 대기
    int response_length = recv(socket, server_response, sizeof(server_response) - 1, 0);
    if (response_length <= 0 || strncmp(server_response, "READY", 5) != 0) {
        printf("Server not ready: %s\n", server_response);
        return;
    }
    else{
    	printf("Server ready!: %s\n", server_response);
    }
    
    printf("Enter the file name to delete: ");
    if (fgets(file_name, sizeof(file_name), stdin) == NULL) {
        perror("Error reading file name");
        return;
    }
    file_name[strcspn(file_name, "\n")] = '\0'; // Newline 제거
    
    if(strlen(file_name) == 0){
    	fprintf(stderr, "File name cannot be Empty.\n");
    	return;
    }
    if(send(socket, file_name, strlen(file_name), 0)<0){
    	perror("Error sending file name");
    	return;
    }
    recv(socket, buffer, BUFFER_SIZE, 0);
    printf("Server response: %s\n", buffer);
}
