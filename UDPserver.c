#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h> 
#include <string.h>

void error(char *msg) {
	perror(msg);
	exit(1);
}

int main(int argc, char *argv[]) {
	struct sockaddr_in my_addr, cli_addr, cli_addr_r;
	int socketfd, socketfd_r, cli_len, msg_len;
	char buffer[256];
	
	if(argc < 2) {
		error("ERROR - usage server <port no>\n");
	}
	socketfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(socket < 0) {
		error("Error in creating the socket\n");
	}
	
	bzero((char*)&my_addr, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(atoi(argv[1]));
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if(bind(socketfd, (struct sockaddr*)&my_addr, sizeof(my_addr)) < 0) {
		error("Error binding the socket\n");
	}

	while(1) {
	cli_len = sizeof(cli_addr_r);
	bzero(buffer, sizeof(buffer));
	msg_len = recvfrom(socketfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&cli_addr_r, &cli_len);
	if(msg_len ==0)	{
		error("Error reading from socket\n");
	} else {
		printf("Recieved packet from %s:%d \n Data : %s\n", inet_ntoa(cli_addr_r.sin_addr), inet_ntoa(cli_addr_r.sin_port), buffer);
		if(strcmp(buffer, "exit") == 0) {
			exit(0);
		}
		
		bzero(buffer, sizeof(buffer));
		scanf("%[^\n]",buffer);
        getchar();
		
		bzero((char*)&cli_addr, sizeof(cli_addr));
		cli_addr.sin_family = AF_INET;
		cli_addr.sin_port = htons(32000);
		cli_addr.sin_addr.s_addr = inet_addr(cli_addr_r.sin_addr);
		sendto(socketfd_r, buffer, strlen(buffer), 0, (struct sockaddr*)&cli_addr.sin_addr, sizeof(cli_addr));
	}
	}
	return 0;
}
	
