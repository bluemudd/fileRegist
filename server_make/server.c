#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "file_upload.h"

#define PORT 8080
#define BUFFER_SIZE 1024

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE] = {0};
    
    // 명령어 수신
    read(client_socket, buffer, BUFFER_SIZE);
    printf("Received command: %s\n", buffer);

    // 명령어에 따른 작업 수행
    if (strcmp(buffer, "upload") == 0) {
        handle_upload(client_socket);
    } else if (strcmp(buffer, "mkdir") == 0) {
        // 디렉토리 생성 구현
    } else if (strcmp(buffer, "list") == 0) {
        // 파일 목록 조회 구현
    } else if (strcmp(buffer, "delete") == 0) {
        // 파일 삭제 구현
    }

    // 클라이언트에 응답 전송
    send(client_socket, "Done", strlen("Done"), 0);
    close(client_socket);
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // 소켓 생성 및 설정
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 바인딩 및 리스닝
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    while ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) >= 0) {
        handle_client(client_socket);
    }

    return 0;
}
