#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>

#define MAX 25
static int counter=0;
void main(int argc, char *argv[]){
	int i;
	int ncons,counter=0;
	
	pid_t pid[ncons];
	ncons=atoi(argv[1]);
	
	if(argc<2){
		printf("Invalid number of arguments\n");
		exit(0);
	}
	
	for(i=0;i<ncons;i++){
		pid[i]=fork();
		counter++;
		int fd,n,status;
		
		if(pid[i]==-1){
			printf("Error creating process\n");	
			exit(-1);	
		}
		else if(pid[i]==0){
			
			//open mypipe
			if((fd=open("/dev/mypipe",O_RDONLY))<0)
			printf("Mypipe open error\n");
			int numStr=4;
			//loop for 1 consumer consuming many strings
			while(numStr>0){			
			char string[MAX];	
			size_t len;
			//read atring from mypipe
			if((n=read(fd,string,MAX))<0)
			printf("Error reading Mypipe\n");
			else{

			//printf("string is %s\n",string);
			len=strlen(string);
			printf("String %s of length %zd is successfully read by Consumer %d\n",string,len,counter);
	
			}
			numStr--;
			}
			
			//close mypipe
			close(fd);	
		
	}
}
}
