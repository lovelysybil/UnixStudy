PROG:incr1 incr2

incr1:incr1.c
	gcc incr1.c -o incr1 -pthread

incr2:incr2.c
	gcc incr2.c -o incr2 -pthread

clean:
	rm -rf *.o incr1 incr2
