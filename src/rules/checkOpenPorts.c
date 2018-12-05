#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

int printLength(int arrrayofNum[1000])
{
        int i;
        int count;
        count = 0;
        for (i = 0; i < 1000; i++) {
            if(arrrayofNum[i]!=0){
                count +=1;
            }
        }
        return count;
} 


int main(int argc, char *argv[])
{
    int n[1000];
    
    //Initialize everything to 0, for length check
    //Has the failure that 0 port cant be checked, easily fixed by initializing to another number
    int i;
    
    //Done so that later on I can determine how many real values are in the function.
    //Real being defined as being not 0. 
    for (i = 0; i < 999; i++) {
        n[i]=0;
    }
    
    for (i=1; i<argc; i++) {
        n[i-1] = atoi(argv[i]);
    }
    

    for(i=0; i< printLength(n);i++){
    int portno     = n[i];
    char *hostname = "localhost";
    
    int sockfd;
    struct sockaddr_in serv_addr;
    struct hostent *server;
 	//SOCK_DGRAM is used for UDP
 	//SOCK_STREAM is used for TCP
    sockfd = socket(2, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("ERROR opening socket");
    }
 
    server = gethostbyname(hostname);
 
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
 
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = 2;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
 
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
        printf("Port is closed");
    } else {
        printf("Port is active");
        printf("%d",portno);
        return 1;
    }
 
    close(sockfd);
    }
    return 0;
}