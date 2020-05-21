#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, clilen, n;
     char buffer[100];
     struct sockaddr_in serv_addr, cli_addr;

     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");

     bzero((char *) &serv_addr, sizeof(serv_addr));

     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(8784);

     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding\0");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);

     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept\0");

     bzero(buffer,100);
     n = read(newsockfd,buffer,99);
     if (n < 0) 
          error("ERROR reading from socket\0");
     printf("Here is the message: %s\n",buffer);
     n = write(newsockfd,"Received your message\0",18);
     if (n < 0) 
          error("ERROR writing to socket\0");
     return 0; 
}

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int close(int sockfd);
