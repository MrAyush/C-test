#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/poll.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

int main(int argc, char **argv) {
	int sockfd, n, conn, len;
	struct pollfd fds[2];
	//char buff[1024];
	int ret;
	char servip[32];
	int servport;
	struct sockaddr_in servaddr, cli, ser;
	len = sizeof(struct sockaddr_in);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	printf("\nEnter the Server IP addr: ");
	scanf("%s", servip);
	printf("\n Enter the server port addr: ");
	scanf("%d", &servport);

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(servip);
	servaddr.sin_port = htons(servport);

	conn = connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	getsockname(sockfd, (struct sockaddr *) &cli, &len);
	printf("\nMy port: %d", (int)ntohs(cli.sin_port));
	printf("\nMy ip:%s", inet_ntoa(cli.sin_addr));

	getpeername(sockfd, (struct sockaddr *) &ser, &len);
	printf("\nServer port:%d", (int) ntohs(ser.sin_port));
	printf("\nServer Ip:%s", inet_ntoa(ser.sin_addr));
	char d[2];
	printf("Enter a number:");
	scanf("%s", d);
	write(sockfd, d, 2);

	fds[0].fd = 0;
	fds[0].events = POLLIN;
	fds[1].fd = sockfd;
	fds[1].events = POLLIN;
	
	while (1) {
		ret = poll(fds, 2, -1);
		if (fds[0].revents) {
			char buff[1024] = {' '};
			read(0, buff, 1024);
			write(sockfd, buff, strlen(buff) - 1);
		}
		if (fds[1].revents) {
			char recvline[1024];
			n = read(sockfd, recvline, 1024);
			printf("\nNumber of bytes received from srver:%d", n);
			recvline[n] = 0;
			printf("\nMessage from the server: %s\n", recvline);
		}
			
	}
	exit(0);
}
