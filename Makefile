PROG: mycat2

mycat2:mycat2.c
	gcc -pthread mycat2.c -o mycat2

clean:
	rm -rf *.o mycat2
