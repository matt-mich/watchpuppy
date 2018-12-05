#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "common.c"

/*
void getPIDfromName(char* name,int** pid_arr, int* length){
    int pid_len = 100;
    *pid_arr = malloc(40 * pid_len);

    FILE *pid;
    char path[255];

    strcpy(path,"/usr/bin/pgrep ");
    strcat(path,name);

    pid = popen(path, "r");

    if (pid == NULL) {
        printf("pgrep failure \n" );
        return(-1);
    }

    int counter = 0;
    
    while (fgets(path, sizeof(path)-1, pid) != NULL) {
        pid_arr[0][counter] = atoi(path);
        counter++;
        
        if(counter >= pid_len){
            *pid_arr = realloc(pid_arr, (pid_len + 5) * sizeof *pid_arr);
            pid_len = sizeof(pid_arr) / sizeof(int);
        }
    }

    *length = counter;
    return 0;
}
*/

char * itoa(int i){
    static char str[16];
    snprintf(str, 16*sizeof(char), "%d", i);
    return str;
}


int main(int argc, char *argv[]){
    
    int pid_len = 100;
    int *pid = malloc(40 * pid_len);
    FILE *fpt;
    int length;
    char name[128];

    int violation = 0;
    
    strcpy(name,argv[1]);
    getPIDfromName(name,pid,&length);

    for(int i = 0; i < length; i++){
        strcpy(name,"kill 15 ");
        strcat(name,itoa(pid[i]));
        fpt = popen(name,"r");

        while (fgets(name, 1000, fpt) != NULL){
            printf("%s", name);
        }
        pclose(fpt);
    }

    return 0;
}