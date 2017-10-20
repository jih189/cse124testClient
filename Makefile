CC = gcc
CFLAGS = -g

all: client


client: tritonClient.o
	$(CC) -o client tritonClient.o $(LIBS)

tritonClient.o: tritonClient.c 

clean:
	rm -f client tritonClient.o




