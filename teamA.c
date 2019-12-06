#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

int main(int argc, char **argv) {
	int sockfd, n, conn, len;
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
	srand(time(0));
	int x = rand() % 20;
	char d[2];
	sprintf(d, "%d", x);
	write(sockfd, d, 2);
	read(sockfd, d, 2);
	exit(0);
}
