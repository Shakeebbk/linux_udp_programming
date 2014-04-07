#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>

void error(char *msg) {
	perror(msg);
	exit(1);
}

int main(int argc, char *argv[]) {
	struct sockaddr_in server_addr, cli_addr;
	int socketfd, socketfd_r, portno;
	char buffer[256];
	if(argc < 3) {
		error("Incorrect arguments\n");
	}
	
	portno = atoi(argv[2]);
	
	socketfd = socket(AF_INET, SOCK_DGRAM, 0);
	socketfd_r = socket(AF_INET, SOCK_DGRAM, 0);
	
	if(socketfd < 0) {
		error("Error getting a socket descriptor\n");
	}
	
	bzero((char*)&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	
	server_addr.sin_port = htons(portno);
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	
	bzero((char*)&cli_addr, sizeof(cli_addr));
	cli_addr.sin_family = AF_INET;
	cli_addr.sin_port = htons(32000);
	cli_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	while(1) {
		bzero(buffer, sizeof(buffer));
		printf("Please enter the message. Type exit and enter to quit\n");
		scanf("%[^\n]",buffer);
        getchar();
		if( strcmp(buffer, "exit") == 0) {
			sendto(socketfd, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
			exit(0);
		} else {
		if(sendto(socketfd, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
			error("Error sending the packets\n");
		}
		bzero(buffer, sizeof(buffer));
		bind(socketfd_r, (struct sockaddr *)&cli_addr, sizeof(cli_addr)); 
		recvfrom(socketfd_r, buffer, sizeof(buffer), 0, NULL, NULL);
		printf("%s\n", buffer);
		}
	}
	close(socketfd);
	exit(0);
}
