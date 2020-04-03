#ifdef MULTI-LOOKUP_H_INCLUDED
#define MULTI-LOOKUP_H_INCLUDED

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

void *requester(void *);
void *resolver();

#endif