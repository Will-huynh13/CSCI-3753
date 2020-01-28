#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <linux/kernel.h>

int main()
{       long int s = syscall(333); // this is the call ID
	printf("System call .: sys_helloworld :. returned %ld \n",s);
	int z;
	long int result = syscall(334,1,10,&z); // this is the call ID for cs3753_add
	printf("cs3753_add stores 10 + 1, which is %d in variable z \n",z);
	return 0;
}
