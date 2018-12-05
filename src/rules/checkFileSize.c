#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    char *path;
    path = argv[1];
    //path = "/Users/thomasbekman/Downloads/project_code.7z";
    //path = "/Users/thomasbekman/Downloads/en_windows_10_consumer_editions_version_1803_updated_march_2018_x64_dvd_12063379.iso";
    int lower_bound;
    int upper_bound;
    sscanf(argv[2],"%d",&lower_bound); 
    sscanf(argv[3],"%d",&upper_bound); 
    //upper_bound = argv[3];
    
    
    char creRule[100000]= {'\0'};
    sprintf(creRule,"wc -c %s > result.txt",path);

    system(creRule);

     
    FILE *fp;
    char sizec[100]={'\0'};
    fp=popen("awk '{print $1}' result.txt","r");
    fgets(sizec,sizeof(sizec),fp);
    pclose(fp);
    int size;
    size=atoi(sizec);
   //printf("%s", "!!!!!!!!!!!");
    //printf("%d", size);
    //printf("%s", "\n");

    
    if(size<lower_bound){
        return 1;
    }
    else if(size>upper_bound){
        return 1;
    }
    else{
        return 0;
    }
}


