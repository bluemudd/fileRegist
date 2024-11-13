OBJECTS = client.o upload.o

client: $(OBJECTS)
	gcc -o client $(OBJECTS)


client.o : client.h client.c
	   gcc -c client.c

upload.o : client.h upload.c
	   gcc -c upload.c


clean: 
	rm client $(OBJECTS)
	ls -l

