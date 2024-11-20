#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024

void handle_list(int client_socket, const char* full_path) {
    char response[BUFFER_SIZE] = {0};
    DIR *dir;
    struct dirent *entry;

    // 지정된 full_path 디렉토리를 연다
    if ((dir = opendir(full_path)) == NULL) {
        perror("opendir");
        send(client_socket, "Failed to open directory\n", 25, 0);
        return;
    }

    // 디렉토리 내부의 파일 및 디렉토리 목록을 response에 추가
    while ((entry = readdir(dir)) != NULL) {
        // '.'와 '..'은 무시 (현재 디렉토리와 상위 디렉토리)
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        strcat(response, entry->d_name);
        strcat(response, "\n");
    }
    closedir(dir);

    // 목록이 비어있을 경우 처리
    if (strlen(response) == 0) {
        strcpy(response, "Directory is empty.\n");
    }

    // 결과를 클라이언트로 전송
    send(client_socket, response, strlen(response), 0);
}

