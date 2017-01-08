PROG:timeServer timeClient tcpClient tcpServer CalClient CalServer

timeServer: daytimetcpsrv.c
	gcc daytimetcpsrv.c -o timeServer

timeClient: daytimetcpcli.c
	gcc daytimetcpcli.c -o timeClient

tcpServer: tcpserv01.o rdwt.o
	gcc tcpserv01.o rdwt.o -o tcpServer

tcpClient: tcpcli01.o rdwt.o
	gcc tcpcli01.o rdwt.o -o tcpClient

tcpserv01.o: tcpserv01.c
	gcc -c tcpserv01.c

tcpcli01.o: tcpcli01.c
	gcc -c tcpcli01.c

rdwt.o: rdwt.c
	gcc -c rdwt.c

CalClient: tcpCalClient.c
	gcc tcpCalClient.c -o CalClient

CalServer: tcpCalServer.c
	gcc tcpCalServer.c -o CalServer

clean:
	rm -rf *.o timeServer timeClient tcpServer tcpClient CalClient CalServer
