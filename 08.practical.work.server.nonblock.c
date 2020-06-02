#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>

void error(char *msg)
{
    perror(msg);
    exit(1);
}
int main(int argc, char *argv[])
{
     int sockfd, newsockfd, clilen, n, last_fd, new_fd;
     char buffer[100];
     struct sockaddr_in serv_addr, cli_addr;

     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");

     bzero((char *) &serv_addr, sizeof(serv_addr));

     last_fd = sockfd;
     
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(8784);
     
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding\n");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);

     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept\n");
     fcntl(last_fd, F_SETFL, O_NONBLOCK);
     fcntl(new_fd, F_SETFL, O_NONBLOCK);
     
     while (1)
     {
     setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));
     int fl = fcntl(sockfd, F_GETFL, 0);
     
     bzero(buffer,100);
     n = read(newsockfd,buffer,99);
     if (n < 0) 
          error("ERROR reading from socket\n");
     printf("Here is the message: %s\n",buffer);
     n = write(newsockfd,"Received your message\n",18);
     fcntl(new_fd, F_SETFL, O_NONBLOCK);
     last_fd = new_fd;
     
     if (n < 0) 
          error("ERROR writing to socket\n");
     return 0;
     
     if(strncmp("/dc", buffer, 3) == 0) 
        { 
          printf("Disconnecting...\n"); 
          shutdown(sockfd, SHUT_RDWR);
          close(sockfd);
        }
      break;
      }
}
