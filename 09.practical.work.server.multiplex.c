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
     setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));
     if (sockfd < 0) 
        error("ERROR opening socket");

     bzero((char *) &serv_addr, sizeof(serv_addr));
     
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
     
     while (1)
     {
        fd_set set;
        FD_ZERO(&set);
        FD_SET(sockfd, &set);
        int maxfd = sockfd;
        for (int i = 0;i< 100; i++) 
        {
              if (clientfds[i] > 0) FD_SET(clientfds[i], &set);
              if (clientfds[i] > maxfd) maxfd = clientfds[i];
        }
        select(maxfd+1, &set, NULL, NULL, NULL);
        
        int fl = fcntl(sockfd, F_GETFL, 0);
        fl |= O_NONBLOCK;
        fcntl(fd, F_SETFL, fl);
         
        bzero(buffer,100);
        n = read(newsockfd,buffer,99);
        if (n < 0) 
              error("ERROR reading from socket\n");
        printf("Here is the message: %s\n",buffer);
        n = scanf(newsockfd,"Received your message\n",21);
     
        if (n < 0) 
              error("ERROR writing to socket\n");
     
        if(strncmp("/dc", buffer, 3) == 0) 
        { 
              printf("Disconnecting...\n"); 
              shutdown(newsockfd, SHUT_RDWR);
              close(sockfd);
        }
      break;
      }
}
