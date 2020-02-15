#include <unistd.h>
#include <stdio.h> // this enable the usage of scanf()
#include <stdlib.h>
#include <malloc.h>
#include <sys/stat.h>		
#include <fcntl.h> // this enable the usage of open() and close()
#include <string.h>
#include <stdbool.h>

#define DEVICENAME "/dev/pa2_character_device"
#define Buffer_size 1024

int main(){
	char command;
	int length, whence, new_offset, writeSize;
	char *buffer;
    
	int file = open(DEVICENAME, O_RDWR  | O_APPEND);
	bool running = true;
	while(running){
		printf("COMMANDS:\n");
		printf("	'r' to read from device\n");
		printf("	'w' to write to device\n");
		printf("	's' to seek from device\n");
		printf("	'e' to exit from device\n");
		printf("	anything else brings up main menu\n");
		printf("/pa2_character_device/command$> ");
		scanf("%c", &command);

		switch(command){
			case 'r':
				buffer = (char *)malloc(Buffer_size * sizeof(char));
				printf("/pa2_character_device/read$> How many bytes to read?: ");
				scanf("%d", &length);
				read(file, buffer, length);
				printf("/pa2_character_device/read$> %s\n", buffer); // print that buffer
				while(getchar() != '\n'); // check for end line
				free(buffer);
				break;
			case 'w':
				buffer = (char *)malloc(Buffer_size * sizeof(char));
				printf("/pa2_character_device/write$> ");
				scanf("%s", buffer);
				writeSize = strlen(buffer);
				write(file, buffer,writeSize); // write the buffer to file
				while (getchar() != '\n'); // check for end line
				free(buffer);
				break;
			case 's':
					new_offset = 0; whence= 0;
					printf("/pa2_character_device/seek$> Enter whence: ");
					scanf("%d", &whence);
					printf("\n/pa2_character_device/write$> Enter an offset value: ");
					scanf("%d", &new_offset);
					while (getchar() != '\n');
				switch(whence)
				{
					case 0: 
						lseek(file, new_offset, whence);
						printf("i am in SEEK_SET\n");
						break;
					case 1:
						lseek(file, new_offset, whence);
					
						break;
					case 2:
						lseek(file, new_offset, whence);
			
						break;
					default:
						printf("Invalid whence, enter again\n");
				}
	
				break;
			case 'e':
				printf("/pa2_character_device/exit$> Exiting\n");
				running = false;
				break;
			default:
				printf("\n/pa2_character_device/error$> error: not a valid command\n");
				break;
		}
	}
	close(file);
	return 0;
}