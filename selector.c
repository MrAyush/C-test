#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>

#define MAX_THREAD 10
#define MAX_CLIENT 10

struct sockaddr_in servaddr, clientaddr;

pthread_t threads[MAX_THREAD];
int clients[MAX_CLIENT] = {-1};
int arr[10];
int number = 0;
int nclient = 0;
int i = 0;

struct client_conn
{
    int client;
    int no;
    struct sockaddr_in clientaddr_s;
};

void make_thread(void *arg) {
	nclient++;
    int n;
    char buff[1024];
    struct client_conn c = *((struct client_conn *)arg);
    printf("[+] Incoming connection form (%s, %d)\n", 
        inet_ntoa(c.clientaddr_s.sin_addr), 
        (int) ntohs(c.clientaddr_s.sin_port));
	while(1) {
        n = read(c.client, buff, 1024);
        printf("Client_thread %d: ", c.client);
        buff[n] = '\0';
        int d;
        sscanf(buff, "%d", &d);
        number += d;
        arr[c.no] = d;
        printf("number -> %d\n", number);
        printf("number- %d\n", d);
        printf("%s\n", buff);
        if (!strcmp(buff, "exit") || n == 0) {
            printf("[-] Closing connection to client (%s)\n", inet_ntoa(c.clientaddr_s.sin_addr));
            close(c.client);
            return;
        }
        /*for (int j = 0; j < MAX_CLIENT; j++) {
        	printf(" %d\n", clients[j]);
            //write(clients[j], buff, n);
        }*/
        if (nclient == 2) {
            	int num = number;
            	if (num % 2 != 0) {
            		if (arr[0] % 2 != 0 ) {
            			printf("TeamA won the toss\n");
            		} else {
            			printf("TeamB won the toss\n");
            		}
            	} else {
            		printf("TIE, and decision on the selector\n");
            	}
            	nclient = 0;
            	i = 0;
            }
    }
}
int main(int argc, char** argv) {
    int len = sizeof(struct sockaddr_in);

    // socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    int client;

	servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(0);

    // bind
    bind(sock, (struct sockaddr *)&servaddr, sizeof(servaddr));
    getsockname(sock, (struct sockaddr *) &servaddr, &len);
	printf("[ ] After bind ephemeral port: %d\n", (int)ntohs(servaddr.sin_port));
	
    // listen
    listen(sock, 5);

    while(1) {
        // accept
        client = accept(sock, (struct sockaddr *) &clientaddr, &len);
        if (client != -1) {
            pthread_t thread;
            struct client_conn c;
            printf("Client_main: %d", client);
            c.client = client;
            c.clientaddr_s = clientaddr;
            c.no = i;
            clients[i++] = client;
            pthread_create(&thread, NULL, (void *) &make_thread, (void *)&c);
            threads[i] = thread;
        }
    }
    int j;
    for (j = 0; j < i; j++) {
        pthread_join(threads[j], NULL);
        close(clients[j]);
    }
    return 0;
}
