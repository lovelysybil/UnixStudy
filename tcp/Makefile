PROG:timeServer timeClient

timeServer: daytimetcpsrv.c
	gcc daytimetcpsrv.c -o timeServer

timeClient: daytimetcpcli.c
	gcc daytimetcpcli.c -o timeClient

clean:
	rm -rf *.o timeServer timeClient
