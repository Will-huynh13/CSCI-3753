#include <unistd.h>
#include <stdio.h> // this enable the usage of scanf()
#include <stdlib.h>
#include <malloc.h>
#include <sys/stat.h>		
#include <fcntl.h> // this enable the usage of open() and close()
#include <string.h>
#define BUFFER_SIZE 1024

int main()
{
	char choice;
	int openFile = open("/dev/pa2_character_device",O_RDWR | O_APPEND); // the usage of O_RDWR
	int whence;
	int NewOffset;
	char *buffer;
	int writeSize = 0;
	int length = 0;

	while(1) // this will infinitly loop
	{

		printf("\n");
		printf("\n");
		printf("#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#\n");
		printf("#~~~This is the test app for pa2_character_device~~~#\n");
		printf("#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#\n");
		printf("Input options:\n");
		printf("Press r to read from device\n");
		printf("Press w to write to the device\n");
		printf("Press s to seek into device\n");
		printf("Press e to exit from the device\n");
		printf("Press anything else to keep reading or writing from the device\n");
		printf("Enter command:\n");

		scanf("%c",&choice);

		switch(choice)
		{
			case 'r':
				printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
				printf("Enter the number of bytes you want to read:\n");
				scanf("%d",&length);
				buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));
				read(openFile,buffer,length);
				printf("Data from the device: %s\n",buffer);
				printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
				while (getchar() != '\n');
				free(buffer);
				break;
			case 'w':
			
				printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
				printf("Enter data you want to write to the device\n");
				//scanf("%[^\n]",buffer);
				fgets(buffer,BUFFER_SIZE, stdin);
				buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));
				writeSize =  strlen(buffer);
				write(openFile,buffer,writeSize);
				printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
				while (getchar() != '\n');
				free(buffer);
				break;
			case 's':
				printf("~~~~~~~~~~~~~~~~~~~~~~~~~\n");
				printf("Enter an offset value:\n");
				//scanf();
				printf("Enter a value for whence:\n");
				//scanf();
				printf("~~~~~~~~~~~~~~~~~~~~~~~~\n");
				while (getchar() != '\n');

				break;
			case 'e':
				printf("~~~~~~~~~\n");
				printf("Good bye!\n");
				printf("~~~~~~~~~\n");
				exit(0);
				break;
			default:
				while (getchar() != '\n');
		}
	}
}

    
