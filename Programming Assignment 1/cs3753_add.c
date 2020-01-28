#include <linux/kernel.h>
#include <linux/linkage.h>
#include <linux/uaccess.h>


asmlinkage long sys_cs3753_add(int x,int y, int* z)
{	
	int r = x + y;
	printk(KERN_ALERT "x has a value of %d\n",x);
	printk(KERN_ALERT "y has a value of %d\n",y);
        put_user(r,z);
	//copy_to_user(z,&r,sizeof(r);// z is in the user space
// r is handleing x + y
	printk(KERN_ALERT " The sum of x + y is %d\n",r);
	return 0;
}


