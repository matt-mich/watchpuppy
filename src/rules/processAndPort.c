/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   processAndPort.c
 * Author: thomasbekman
 *
 * Created on November 22, 2018, 11:57 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFSIZE 128

/*
 * 
 */
int main(int argc, char** argv) {
    char *port;
    char *pid;
     port = argv[1];
     pid = argv[2]; 
     
  
    //Lets create the shell command
    char str[100];
    strcpy(str, "lsof -t -i :");
    strcat(str, argv[1]);
    int pid_on_Port;
    char *cmd = str;    

    char buf[BUFSIZE];
    FILE *fp;

    if ((fp = popen(cmd, "r")) == NULL) {
        return -1;
    }
	
    while (fgets(buf, BUFSIZE, fp) != NULL) {
          //Do whatever you want here...
          pid_on_Port = atoi(buf);
          
    }


    int pid_as_int;
    pid_as_int = atoi(pid);
    //If this loop is not hit, then the process must not be on that port
    if(pid_on_Port == pid_as_int){
	
         char str1[100];
         strcat(str1, "The PID: ");
         strcat(str1, pid);
         strcat(str1, " is operating on the port ");
         strcat(str1, port);
             
        printf("%d", 1);         
        printf("%s", str1);
        return 1;
    }
//Case when process is not operating on the port
    else{
         char str2[100];
         strcat(str2, "The PID: ");
         strcat(str2, pid);
         strcat(str2, " is NOT operating on the port ");
         strcat(str2, port);
             
                 
        printf("%s", str2);
        //This may be redundant
        return 0;
    }
    
     printf("OUTPUT: %s", pid);
     
     //Here is the command to check to check processes on port:
     //lsof -t -i :80

    
    
  return (0);
}

