OBJECTS = server.o file_upload.o


server: $(OBJECTS)
	gcc -o server $(OBJECTS)


server.o : file_upload.h server.c
	   gcc -c server.c

upload.o : file_upload.h file_upload.c
	   gcc -c file_upload.c


clean :
	rm server $(OBJECTS)
	ls -l

