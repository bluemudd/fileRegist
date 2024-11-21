#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdint.h>
#include <setjmp.h>
#include <inttypes.h>
#include "client.h"

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    char *command;
    char *argument;
    char dir_path[BUFFER_SIZE] = {0};

    // 소켓 생성 및 서버 연결
    sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        return -1;
    }

    // 디렉토리 입력 받기
    printf("Enter directory path: ");
    fgets(dir_path, sizeof(dir_path), stdin);
    dir_path[strcspn(dir_path, "\n")] = 0;  // 줄 바꿈 제거

    // 서버에 디렉토리 경로 전송하여 디렉토리 확인 및 생성 요청
    send(sock, dir_path, strlen(dir_path) + 1, 0);

    // 서버 응답 대기 (디렉토리 생성 확인)
    recv(sock, buffer, sizeof(buffer), 0);
    printf("Server response: %s\n", buffer);

    // 클라이언트 명령어 처리
    while (1) {
        // 명령어 입력
        printf("Enter command (upload, list, delete, exit): ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0;  // 줄 바꿈 제거

        command = strtok(buffer, " ");  // 첫 번째 토큰은 명령어
        argument = strtok(NULL, " ");   // 두 번째 토큰은 명령어의 인자 (파일 이름 또는 디렉토리 이름)

        // 명령어 처리
        if (strcmp(command, "upload") == 0) {
            upload_file(sock);
        } else if (strcmp(command, "list") == 0) {
            handle_list(sock);
        } else if (strcmp(command, "delete") == 0) {
            handle_delete(sock);
        } else if (strcmp(command, "exit") == 0) {
            printf("Exiting...\n");
            break;
        } else {
            printf("Unknown command: %s\n", command);
        }
    }

    close(sock);
    return 0;
}

