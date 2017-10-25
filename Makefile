test: wsclient.o base64.o sha1.o test.o
	$(CC) -o test -pthread wsclient.o base64.o sha1.o test.o -lssl

wsclient.o: wsclient.c
	$(CC) -c wsclient.c -lssl -pthread

base64.o: base64.c
	$(CC) -c base64.c -lssl -pthread
	
sha1.o: sha1.c
	$(CC) -c sha1.c -lssl -pthread
	
test.o: test.c
	$(CC) -c test.c -lssl -pthread

