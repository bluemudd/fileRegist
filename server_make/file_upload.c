// file_upload.c
#include "file_upload.h"
#include <sys/stat.h>
#include <sys/types.h>


#define UPLOAD_DIR "uploads" // file save directory

void handle_upload(int client_sock) {
    char buffer[BUFFER_SIZE];
    int bytes_read;
    FILE *file;
    char file_path[BUFFER_SIZE + 50];
    struct stat st = {0};
    
    // upload exist?
    if(stat(UPLOAD_DIR, &st) == -1){
    	if(mkdir(UPLOAD_DIR, 0755)== 0){
    		printf("Directory '%s' created. \n", UPLOAD_DIR);
	} else{
		perror("Failed to createDirectory");
		return;
    	}
    }



    // 파일 이름 수신
    bytes_read = read(client_sock, buffer, BUFFER_SIZE);
    if (bytes_read <= 0) {
        printf("Failed to receive file name.\n");
        return;
    }

    buffer[bytes_read] = '\0';  // null-terminate the received file name
    printf("Uploading file: %s\n", buffer);
    
    // save file path on uploads directory
    snprintf(file_path, sizeof(file_path), "%s/%s", UPLOAD_DIR, buffer);



    // 파일 열기
    file = fopen(file_path, "wb");
    if (file == NULL) {
        perror("fopen failed");
        return;
    }


    // 파일 데이터 수신
    while ((bytes_read = read(client_sock, buffer, BUFFER_SIZE)) > 0) {
        fwrite(buffer, sizeof(char), bytes_read, file);

	printf("Received data: %.*s\n", bytes_read, buffer);
    }

    printf("File upload completed.\n");

    // 파일 닫기
    fclose(file);
}

