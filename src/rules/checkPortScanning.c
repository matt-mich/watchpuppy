/*

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/time.h>

#define BUFSIZE 2048

int
main(int argc, char **argv)
{
    

	struct sockaddr_in myaddr;	/* our address */
	struct sockaddr_in remaddr;	/* remote address */
	socklen_t addrlen = sizeof(remaddr);		/* length of addresses */
	int recvlen;			/* # bytes received */
	int fd;				/* our socket */
	unsigned char buf[BUFSIZE];	/* receive buffer */


	/* create a UDP socket */
	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("cannot create socket\n");
		return 0;
	}

	/* bind the socket to any valid IP address and a specific port */

	//memset((char *)&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myaddr.sin_port = htons(5000);

	if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
		perror("bind failed");
		return 0;
	}
                struct timeval tv;
             
        //Sets the timeouts on listening of sockets
        tv.tv_sec = 1;
        tv.tv_usec = 100000;
        if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) {
            perror("Error");
        }


		recvlen = recvfrom(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen);
		if (recvlen > 0) {
                    return 1;
		}
                return 0;
	

}
