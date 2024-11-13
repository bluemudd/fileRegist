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
    char buffer[BUFFER_SIZE] = {0};
    
    // 업로드할 파일 이름 입력
    printf("Enter the file name to upload: ");
    scanf("%s", file_name);
    
    // 파일 열기
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    printf("uploading file %s\n", file_name); 
    // 서버로 "upload" 명령어 전송
    send(socket, "upload", strlen("upload"), 0);
    usleep(1000);  // 서버의 응답 시간을 기다리기 위해 약간의 대기
    
    // 파일 이름 전송
    send(socket, file_name, strlen(file_name), 0);
    usleep(1000);  // 서버의 응답 시간을 기다리기 위해 약간의 대기
    
    // 파일 내용을 버퍼로 읽어 서버에 전송
    while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
        send(socket, buffer, strlen(buffer), 0);
        memset(buffer, 0, BUFFER_SIZE);  // 버퍼 초기화
        usleep(1000);  // 서버가 데이터를 처리할 시간을 기다림
    }

    // 전송 종료 신호로 "EOF" 전송
    send(socket, "EOF", strlen("EOF"), 0);

    // 파일 닫기
    fclose(file);
    printf("File upload completed.\n");
}
