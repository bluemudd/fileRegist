#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

#define BUFFER_SIZE 1024

void handle_mkdir(int client_socket, const char* received_command) {
    char dirname[BUFFER_SIZE] = {0};

    printf("Inside handle_mkdir...\n");

    // 수신한 명령어를 디버깅 출력
    printf("Received command in handle_mkdir: '%s'\n", received_command);  // 디버깅: 명령어 확인

    // received_command가 NULL인지 확인
    if (received_command == NULL) {
        printf("Error: received_command is NULL\n");
        return;
    }

    // received_command를 수정하지 않도록 복사하여 사용
    char command_copy[BUFFER_SIZE];
    strncpy(command_copy, received_command, BUFFER_SIZE);

    // 명령어를 공백으로 구분하여 "mkdir"과 디렉토리 이름을 분리
    char *argument = strtok(command_copy, " ");  // "directory name"

    // 분리된 명령어와 인자 확인
    printf("Argument: '%s'\n",argument);

    // strtok 결과 확인
    if (argument == NULL) {
        printf("Parsing error: command or argument is NULL\n");
        send(client_socket, "Error: Invalid command format. Use 'mkdir directory_name'", strlen("Error: Invalid command format. Use 'mkdir directory_name'"), 0);
        return;
    }

    // 디렉토리 이름을 dirname에 저장
    strncpy(dirname, argument, BUFFER_SIZE - 20);  // "./uploads/"를 위한 공간 확보

    // 디렉토리 생성 경로 설정: ./uploads/디렉토리 이름
    char dir_path[BUFFER_SIZE];
    int snprintf_result = snprintf(dir_path, sizeof(dir_path), "uploads/%s", dirname);

    // snprintf 결과 확인
    if (snprintf_result >= sizeof(dir_path)) {
        printf("Directory path is too long\n");
        send(client_socket, "Error: Directory name too long", strlen("Error: Directory name too long"), 0);
        return;
    }

    printf("Creating directory at path: %s\n", dir_path);

    // 디렉토리 생성
    if (mkdir(dir_path, 0755) == 0) {
        printf("Directory created successfully\n");
        send(client_socket, "Directory created successfully", strlen("Directory created successfully"), 0);
    } else {
        // 디렉토리 생성 실패
        char error_message[BUFFER_SIZE];
        snprintf(error_message, BUFFER_SIZE, "Error: Failed to create directory (%s)", strerror(errno));
        send(client_socket, error_message, strlen(error_message), 0);
        perror("mkdir failed");
    }
}
