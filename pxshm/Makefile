PROG:server client server2 client2

server:server1.c
	gcc server1.c -o server -pthread -lrt

client:client1.c
	gcc client1.c -o client -pthread -lrt

server2:server2.c
	gcc server2.c -pthread -lrt -o server2

client2:client2.c
	gcc client2.c -pthread -lrt -o client2

clean:
	rm -rf *.o server client server2 client2
