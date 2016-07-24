#include<stdio.h>
#include<fcntl.h>
#include<string.h>

#define MAX 25

void main(){
	int fd,n,i;
	char string[25];	
	size_t len;

	//open mypipe
	if((fd=open("/dev/mypipe",O_RDONLY))<0)
		printf("Mypipe open error\n");

	//read string from mypipe
	if((n=read(fd,string,MAX))<0)
		printf("Error reading Mypipe\n");
	else{

	//printf("string is %s\n",string);
	len=strlen(string);
	printf("String -  %s - is successfully read from Consumer\n",string);
	
	}
	//close mypipe
	close(fd);
}
