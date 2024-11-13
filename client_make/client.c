#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "client.h"


#define PORT 8080
#define BUFFER_SIZE 1024



int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    char *command;
    char *argument;
    
    // 소켓 생성 및 서버 연결
    sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    // 명령어 입력
    printf("Enter command (upload, mkdir, list, delete): ");
    fgets(buffer, BUFFER_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = 0;  // 줄 바꿈 제거
    

    command = strtok(buffer, " ");  // 첫 번째 토큰은 명령어
    argument = strtok(NULL, " ");   // 두 번째 토큰은 명령어의 인자 (파일 이름 또는 디렉토리 이름)

    // 명령어 출력 (디버깅용)
    printf("Command: %s\n", command);
    if (argument != NULL) {
        printf("Argument: %s\n", argument);
    } else {
        printf("No argument provided.\n");
    }

    if(strcmp(command, "upload") == 0){
    	upload_file(sock);
    	//upload    
    }
    else{

    // 서버로 명령어 전송
   	send(sock, buffer, strlen(buffer), 0);
	
    // 서버 응답 수신
    	read(sock, buffer, BUFFER_SIZE);
    	printf("Server response: %s\n", buffer);
    }
    close(sock);
    return 0;
}
