#include <stdio.h> // this enable the usage of scanf()
#include <stdlib.h>		
#include <fcntl.h> // this enable the usage of open() and close()
#define BUFFER_SIZE 1024

int main()
{
	char choice;
	char buffer[BUFFER_SIZE];
	int openFile = open("/dev/pa2_character_device",O_RDWR); // the usage of O_RDWR

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
				//scanf();
				printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
				break;
			case 'w':
				printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
				printf("Enter data you want to write to the device\n");
				//scanf();
				printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
				break;
			case 's':
				printf("~~~~~~~~~~~~~~~~~~~~~~~~~\n");
				printf("Enter an offset value:\n");
				//scanf();
				printf("Enter a value for whence:\n");
				//scanf();
				printf("~~~~~~~~~~~~~~~~~~~~~~~~\n");
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
    
