
#include<stdio.h>
#include<fcntl.h>
#include<string.h>

void main(){
	int i,rnum,fd,n_bytes;
	
	//size_t str_length;
	char str[25]="This is Producer ";
			size_t str_length;
			str_length=strlen(str);

			str_length++;
			str[str_length]='\0';
			//str_length++;
			str_length=strlen(str);
			printf("String Produced - %s - length %zd\n",str,str_length);

	if((fd=open("/dev/mypipe",O_WRONLY))<0)
		printf("Mypipe file open error\n");
	if((n_bytes=write(fd,str,str_length))<0)
		printf("Mypipe write error\n");
	else
	printf("String Successfully written to Producer\n");
	close(fd);
	
}
