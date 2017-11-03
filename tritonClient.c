/* programmer: jiaming hu
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    printf( "./client <ip_address> <port_number> <url> [url]\n");
    char * server_ip_str = argv[1];
    char * server_port_str = argv[2];
    int sockfd, portno;
    off_t offset = 0;
    int rc, fd;
    char message[1024];
    bzero(message, 1024);
    strcat(message, "GET ");
    strcat(message, "/");
    char ver[12];
    bzero(ver, 12);
    strcat(ver, " HTTP/1.1");
    ver[9] = '\0';
    char crlf[3];
    crlf[0] = (char)13;
    crlf[1] = (char)10;
    crlf[2] = '\0';
    strcat(message, ver);
    strcat(message, crlf);
    strcat(message, "Client_name: frank");
    strcat(message, crlf);
    strcat(message, crlf);
    strcat(message, "GET ");
    strcat(message, "/test2.html");
    strcat(message, ver);
    strcat(message, crlf);
    strcat(message, "Client_name: frank");
    strcat(message, crlf);
    strcat(message, crlf);
    strcat(message, "GET ");
    strcat(message, "/test3.html");
    strcat(message, ver);
    strcat(message, crlf);
    strcat(message, "Client_name: frank");
    strcat(message, crlf);
    strcat(message, "Connection: close");
    strcat(message, crlf);
    strcat(message, "JJ: big");
    strcat(message, crlf);
    strcat(message, crlf);
    printf("the request: %s", message);
    struct sockaddr_in serv_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    portno = atoi(server_port_str);
    if (sockfd < 0) {
        printf("socket error\n");
        return 1;
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    int rtnVal = inet_pton(AF_INET, server_ip_str, &serv_addr.sin_addr.s_addr);
    if(rtnVal == 0){
        printf("fail to convert ip to binary form\n");
        return 1;
    }
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) {
        printf("fail to connect server\n");
        return 1;
    }
    char buffer[4000];
    bzero(buffer, 4000);
    send(sockfd, message, strlen(message),0);

    int numBytesRcvd = recv(sockfd, buffer, 4000, 0);
    printf("buffer: \n%s", buffer);
    while(numBytesRcvd != 0){

        bzero(buffer, 4000);
        numBytesRcvd = recv(sockfd, buffer, 4000, 0);
        if(numBytesRcvd == 0)
            break;
        printf("buffer: \n%s", buffer);
    }
    close(sockfd);
    return 0;
}
