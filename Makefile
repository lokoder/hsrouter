all:
	gcc -o main main.c _log.c _socket.c _connection.c -lpthread -g -Wall

clean:
	rm -f main 


