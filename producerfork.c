
#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>

static counter=0;
void main(int argc, char *argv[]){
	int nprod,i,j;
	nprod=atoi(argv[1]);
	pid_t pid[nprod];
	if(argc<2){
		printf("Invalid number of arguments\n");
		exit(0);
	}		
	for(i=0;i<nprod;i++){
		pid[i]=fork();
		counter++;
		int fd,n,status;
		if(pid[i]==-1){
			printf("Error creating process\n");	
			exit(-1);	
		}
		else if(pid[i]==0){
			if((fd=open("/dev/mypipe",O_WRONLY))<0)
				printf("Mypipe file open error\n");
			int numStr = 4;
			while(numStr>0){			
			
			char str[25]="This is Producer ";
			size_t len;
			len=strlen(str);

			str[len]=counter+'0';
			len++;
			str[len]=32;
			len++;
			str[len]=numStr+'0';
			len++;
			str[len]='\0';
			//str_length++;
			len=strlen(str);
			printf("String Produced - %s - length %zd\n",str,len);
			int n_bytes;
			if((n_bytes=write(fd,str,len+1))<0)
				printf("Mypipe write error\n");
			else
				printf("String Successfully written by Producer %d %d\n",counter,numStr);
				numStr--;
			}
			close(fd);
			exit(0);
		}
				
	}
	
		
}

