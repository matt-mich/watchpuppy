#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>



int main()
{

    //SSH occurs over TCP port 22
    //SO if this returns a failure then SSH is occuring
    int portno     = 22;
    char *hostname = "localhost";
    
    int sockfd;
    struct sockaddr_in serv_addr;
    struct hostent *server;
 
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("ERROR opening socket");
    }
 
    server = gethostbyname(hostname);
 
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
 
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
 
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
        //TCP PORT 22 is closed. SSH is not occurring
        close(sockfd);
	printf("%d", 0);
        return 0;
    } else {
        //TCP PORT 22 is open. SSH is occuring. Scary (maybe, probably not. Calm down.)
        close(sockfd);
	printf("%d", 1);
        return 1;
    }
}

   
