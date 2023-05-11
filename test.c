#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/select.h>
#include<sys/time.h>
#include<fcntl.h>
#include<errno.h>

#define DEVICE_NODE "/dev/Random_dev"
#define BUFFER_LENGTH 4              
 

int main()
{
    unsigned int ret, fd;
    char result[10];
    char option;

    printf("Enter 'o' to open the device file\n");
    printf("Enter 'r' to read the random number\n");

	fd = 0;
	while (1)
	{	
		printf("You choose: ");
		scanf(" %c" ,&option);

		switch(option)
		{
		case'o':
			fd = open(DEVICE_NODE, O_RDWR);             
			if (!fd)
				printf("Can not open the device file.\n");
			else
				printf("Device file opened!\n");
			break;
		
		case'r':
			if (!fd) 
			{
				printf("The device file is not opened\n");
				break;
			}
			ret = read(fd, &result, sizeof(BUFFER_LENGTH));  
			printf("Random number is: %s.\n", result);
			printf("Quit the application.\n");
			close(fd);
			return 0;
		
		default:
			printf("Invalid input %c.\n", option); 
			break;
		}
	}
}