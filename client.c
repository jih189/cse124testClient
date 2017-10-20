
#include "client.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>



int invoke_client(char * server_ip_str, char * server_port_str,
				  enum Operation op, char * filename)
{
    fprintf(stderr, "Connecting to %s:%s to %d %s\n",
					server_ip_str, server_port_str,
					op, filename);
    int sockfd, portno;
    struct stat stat_buf;
    off_t offset = 0;
    int rc, fd;

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
    if(op == STOR){
        char request[100];
        strcpy(request, "STOR ");
        int f;
        std::string str(filename);
        f = str.find_last_of('/');
        if(f < 0)
            strcat(request,filename);
        else
            strcat(request, filename + f + 1);

        char end[3];
        strcpy(end,"");
        end[0] = (char)13;
        end[1] = (char)10;
        strncat(request,end,2);
        send(sockfd, request, strlen(request),0);
        // find the file which will be sent
        fd = open(filename, O_RDONLY);
        if(fd == -1){
            printf("unable to open the file\n");
            return 1;
        }
        // find the size of the file
        fstat(fd, &stat_buf);
        offset = 0;
        // send file
        rc = sendfile(sockfd, fd, &offset, stat_buf.st_size);
        if(rc == -1){
            printf("faile to send file\n");
            return 1;
        }

        close(fd);
    }else if(op == RETR){
        // build and send first message
        char request[100];
        strcpy(request, "RETR ");
        strcat(request, filename);
        char end[3];
        strcpy(end,"");
        end[0] = (char)13;
        end[1] = (char)10;
        strncat(request,end,2);
        send(sockfd, request, strlen(request),0);
        fd = open(filename, O_CREAT|O_WRONLY|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
        if(fd == -1){
            printf("fail to open file\n");
            return 1;
        }
        char buffer[1024];
        int numBytesRcvd = recv(sockfd, buffer, 1024, 0);
 
        while(numBytesRcvd > 0){
            write(fd, buffer, numBytesRcvd);
            bzero(buffer, 1024);
            numBytesRcvd = recv(sockfd, buffer, 1024, 0);
        }
        close(fd);
    }
    close(sockfd);
    return 0;  // 0 on success, non-zero on error
}
