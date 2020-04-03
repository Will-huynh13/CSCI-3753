
    #include <pthread.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <sys/types.h>
    #include <sys/syscall.h>
    #include "util.h"
    #include "multi-lookup.h"
    #include "sys/time.h"
struct MUTEX 
{
    pthread_mutex_t MutResultsLock;  
    pthread_mutex_t Mut_lock[10];
    pthread_mutex_t sharedLock;
    pthread_mutex_t MutService_lock;
};
struct SHARED_VARIABLES
{
    char sharedBuffer[2000][20];
	char files_to_service[20][20];
    char FileResults[20];
    int Buffer_position;
    char FilesServiced[20];
    int NumFiles;
};

struct THREAD_INFO
{
    char *PointerToArray;
};

struct GLOBALSTRUCT
{
    struct THREAD_INFO *array;
    struct SHARED_VARIABLES variables;
    struct MUTEX mxt;

};
void *RequesterThreads(void *arg)
{
	
	pid_t THEADID = syscall(SYS_gettid); // getting the thread ID
	struct GLOBALSTRUCT *global = arg;

	for(int i = 0; i < global->variables.NumFiles; i++) // loops through the files
	{
		
		if(!pthread_mutex_trylock(&global->mxt.Mut_lock[i])) // if there is no lock present, after this IF then there will be a lock
		{
			char FileToService[20];
			
			strcpy(FileToService, (global->array->PointerToArray + (i*20))); //desintation, source
			
			printf("\n%d is servicing %s\n", THEADID, FileToService); // displays what thread is servicing what file

			pthread_mutex_lock(&global->mxt.MutService_lock);  // lock the service mutex
		
			FILE *file_IN = fopen(global->variables.FilesServiced, "a"); // opens the service file

			if(file_IN == NULL)
			{
				printf("could not open file to be Serviced.\n");
				exit(1);
			}
				
			fprintf(file_IN, "thread: %d is servicing %s\n", THEADID, FileToService);
			fclose(file_IN);
			pthread_mutex_unlock(&global->mxt.MutService_lock);  // unlocking the service lock before moving on
			FILE* inputFile = fopen(FileToService, "r"); // reads in the file

			if(!inputFile){printf("Error: input file\n");} // ERROR check if the file cannot be opened
			char Domain[20];
			
			while(fgets(Domain, sizeof(Domain)+1, inputFile)) // using fgets, it takes 3 params, destination, chars that need to be read, then the stream
			{
				
				Domain[strlen(Domain)-1] = '\0'; // parses the domain and stores them in the buffer

				strcpy(global->variables.sharedBuffer[global->variables.Buffer_position], Domain);

				global->variables.Buffer_position++; // updates the buffer position

			}
			fclose(inputFile);
			pthread_mutex_destroy(&global->mxt.Mut_lock[i]); // destroy the mutex
		}


	}
	return 0;
}


void *resolverThreads(void *arg)
{
	struct GLOBALSTRUCT *global = arg;
	if(!pthread_mutex_trylock(&global->mxt.sharedLock)) // check if the lock exist, if not apply it
	{
		for(int i = 0; i < global->variables.Buffer_position; i++) // looping through the buffer position
		{ 
			char Domain[20];
			char IPs[20];
			
			strcpy(Domain, global->variables.sharedBuffer[i]); //coping the sharedbuffer data to domain
			int check = 0;  //
			if(dnslookup(Domain, IPs, sizeof(IPs))) // if it does not return 0, then it failed
			{
				fprintf(stderr, "invalid hostname: %s \n", Domain);
				check = 1;		
				printf("\n");
			}
			else
			{
				printf("%s\n", IPs); 
			}

			pthread_mutex_lock(&global->mxt.MutResultsLock); // locking the results so that its safe
			
			FILE *file_IN = fopen(global->variables.FileResults, "a"); // append the new IP to the results file
			if(file_IN == NULL)
			{
				printf("ERROR: OUTPUT FILE FAILED \n"); // prints if the output file failed
				exit(1);
			}
			if(!check) //  if the check is not true
			{
					
				fprintf(file_IN, "%s, %s\n", Domain, IPs);
			}
			else
			{
				fprintf(file_IN, "%s, \n", Domain);
			}

			fclose(file_IN);

		pthread_mutex_unlock(&global->mxt.MutResultsLock); // unlocking the results mutex
		}
		
	pthread_mutex_destroy(&global->mxt.sharedLock); // destroy the shared lock
	}
	return 0;	

}


int main(int argc, char *argv[])
{
	
	struct timeval start_time, finish_time;

    gettimeofday(&start_time, NULL); // starting the timer

	struct GLOBALSTRUCT global;
	

	global.variables.Buffer_position = 0; // setting the buffer position to 0


	strcpy(global.variables.FilesServiced, argv[4]); // writes into the serviced.txt


	strcpy(global.variables.FileResults, argv[3]); // writes into the results.txt



	int Number_of_Requesters = atoi(argv[1]); // takes the number of requestors

	Number_of_Requesters = (Number_of_Requesters <= 5) ? Number_of_Requesters : 5;// checks if it is over the limit, if so then set it to 5

	pthread_t RequesterTHREADS[Number_of_Requesters]; // array of requestors
	struct THREAD_INFO THREAD_INFO[Number_of_Requesters];
	
	global.array = THREAD_INFO; // initalizing the shared array

	
	int Number_of_Resolvers = atoi(argv[2]); // takes the number of resolvers

	Number_of_Resolvers = (Number_of_Resolvers <= 10) ? Number_of_Resolvers : 10; // if the number of resolvers are over 10, reset it to 10
	
	pthread_t RESOLVERINFO[Number_of_Resolvers];
	char file_IN[20][20];
	
	int NumFiles = argc-5; // gets the number of input files
	global.variables.NumFiles = NumFiles;
	for(int i = 0; i < NumFiles; i++)
	{
		
		strcpy(file_IN[i], argv[i+5]); // copying data from names
	}

// initializing the mutexes with NUll for default values
	pthread_mutex_init(&global.mxt.MutResultsLock, NULL);
	pthread_mutex_init(&global.mxt.MutService_lock, NULL);
	pthread_mutex_init(&global.mxt.sharedLock, NULL);

    int i = 0;
	while(i < 10)
	{
		pthread_mutex_init(&global.mxt.Mut_lock[i], NULL); // initialize the locks
        i++;
	}

	for (int i = 0; i < Number_of_Requesters; ++i)
	{
		THREAD_INFO[i].PointerToArray = &file_IN; // populates the thread array and the files that needs to be serviced
		pthread_create(&RequesterTHREADS[i], NULL, RequesterThreads, &global); // creates threads
	}

	printf("hellooooooooo i suck at c");
	
	for(int i = 0; i < Number_of_Resolvers; i++)
    {
		pthread_join(RequesterTHREADS[i], NULL); // calling threads wait untl the target threads are done
		pthread_create(&RESOLVERINFO[i], NULL, resolverThreads, &global); // creating resolvers threads
		pthread_join(RESOLVERINFO[i], NULL); // joining resolver threads
	}

	pthread_mutex_destroy(&global.mxt.MutResultsLock);
	pthread_mutex_destroy(&global.mxt.MutService_lock);
	pthread_mutex_destroy(&global.mxt.sharedLock);


	gettimeofday(&finish_time,NULL); // ending the time
	// not sure if this is the correct units of time but this is how it is calculated
	printf("Time elapsed (ms) = %d \n", (((finish_time.tv_sec * 1000000) + finish_time.tv_usec) - ((start_time.tv_sec * 1000000) + start_time.tv_usec)));
}
