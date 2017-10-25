// code inspired by http://osr507doc.xinuos.com/en/netguide/dusockT.code_samples.html

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	struct sockaddr_un server;

	
	int sock = socket(AF_UNIX, SOCK_STREAM, 0);
	if(sock < 0) {
		perror("Error creating UNIX socket.\n");
		exit(1);
	}
	
	server.sun_family = AF_UNIX;
	strcpy(server.sun_path, "test.sock");

    if (connect(sock, (struct sockaddr *) &server, sizeof(struct sockaddr_un)) < 0) {
        close(sock);
        perror("connecting stream socket");
        exit(1);
    }
	
	char buf[250];
	int i;
	for (i=0; i<5; i++) {
		sprintf(buf, "linux socket data: %i", i);
		if (write(sock, buf, strlen(buf)) < 0)
			perror("writing on stream socket");
		sleep(1);
	}
    close(sock);

}