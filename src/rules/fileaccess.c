#include <stdio.h>
#include <stdlib.h>

int main()
{
    char path[10000]= {'\0'};
    printf("Enter the path you want to monitor\n");
    scanf("%s",path);
	int option;
	printf("Enter the number of operation you want to do:\n");
	printf("1.Create rule\n");
	printf("2.Get result\n");
	scanf("%d",&option);
	if(option==1)
	{
	    char creRule[100000]= {'\0'};
        sprintf(creRule,"auditctl -w %s -p rwxa",path);
        system(creRule);
        system("auditctl -l");
		return 0;
	}
    else if(option==2)
    {
		char getResult[100000]= {'\0'};
        sprintf(getResult, "ausearch -f %s > result.txt",path);
        system(getResult);
        system("awk '/pid.*/ {print $13,$17,$25}' result.txt > pideuidcomm.txt");
		return 0;
	}
    else
	{
		printf("Wrong number!");
		return 0;
	}
}

