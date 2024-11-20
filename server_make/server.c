#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define UPLOAD_DIR "uploads"  // 작업할 기본 디렉토리

// 클라이언트의 명령어 처리
void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE] = {0};
    char *command = NULL;

    // 디렉토리 준비
    struct stat st = {0};
    char full_path[BUFFER_SIZE];
	
	int bytes_read = recv(client_socket, buffer, BUFFER_SIZE, 0);
	if(bytes_read <= 0){
		perror("Client disconnected during directory path reception");
		return;
	}
	
	buffer[bytes_read] = "\0";
	printf("Received directory name: %s\n", buffer);
	
	snprintf(full_path, sizeof(full_path), "%s/%s", UPLOAD_DIR, buffer);
	
	if(stat(full_path, &st) == -1){
		if(mkdir(full_path, 0777) == 0){
			printf("Directory created: %s\n", full_path);
			send(client_socket, "Directory created successfully\n", 32, 0);
		} else{
			perror("Failed to create directory");
			send(client_socket, "Error: Failed to create directory\n", 35, 0);
		}
	}
	
    send(client_socket, "READY\n", strlen("READY\n"), 0);

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_read = read(client_socket, buffer, BUFFER_SIZE);
        if (bytes_read <= 0) {
            perror("Client disconnected");
            break;
        }

        // 명령어와 인수 분리
        buffer[bytes_read] = '\0';
        printf("Received command: %s\n", buffer);
        command = strtok(buffer, " ");
        printf("%s\n", command);
        printf("%s\n", full_path);

        // 명령 처리
        if (command != NULL) {
            if (strcmp(command, "upload") == 0) {
            handle_upload(client_socket, full_path); // 파일 업로드 처리
            } else if (strcmp(command, "list") == 0) {
                handle_list(client_socket, full_path);
            } else if (strcmp(command, "delete") == 0) {
                handle_delete(client_socket, full_path);
            } else if (strcmp(command, "exit") == 0) {
                send(client_socket, "Goodbye\n", strlen("Goodbye\n"), 0);
                break;
            } else {
                send(client_socket, "Error: Unknown command\n", strlen("Error: Unknown command\n"), 0);
            }
        }
    }

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
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(1);
    }
    listen(server_fd, 3);
    printf("Server listening on port %d...\n", PORT);
    
    while ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) >= 0) {
        printf("Client connected\n");
        handle_client(client_socket);
        printf("Client disconnected\n");
    }

    close(server_fd);
    return 0;
}

