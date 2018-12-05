#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char *argv[]){

    char str_msg[255];

    if (argc == 2){
        strcpy(str_msg,argv[1]);
    }else{
        strcpy(str_msg,"ACTION TRIGGERED");
    }

    FILE *log;
    log = fopen("../bad.log", "a");

    time_t _time;
    time (&_time);
    
    char str_time[50];
    strcpy(str_time,ctime(&_time));
    strtok(str_time, "\n");
    struct timespec ns, ns2;
    ns.tv_nsec = 100000;
    ns.tv_sec = 0;

    while (1){
        fprintf(log,"%s: %s \n", str_time,str_msg);
        nanosleep(&ns,&ns2);
    }
    fclose(log);
    return 0;
}