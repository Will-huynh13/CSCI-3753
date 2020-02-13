#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h> // contains the functions that are related to device driver coding
#include<linux/slab.h> // allows the usage of kmalloc() and kfree()
#include<linux/uaccess.h> // enables me to access data from user-space in the kernel

#define BUFFER_SIZE 1024
#define MAJOR_NUMBER 247 // this is the specific major number for the device driver
#define DEVICE_NAME "pa2_char_driver" // this is the name of the device driver


MODULE_AUTHOR("Will Huynh"); //this is gives the module an author
MODULE_LICENSE("GPL"); // General Public License




/* Define device_buffer and other global data structures you will need here */

int num_open; // variable that counts the number of times its been open
int num_close; // variable that counts the number of times its been closed
static char *device_buffer; // dynamically allocated kernel buffer to store the data written by the user

ssize_t pa2_char_driver_read (struct file *pfile, char __user *buffer, size_t length, loff_t *offset)
{
	/* *buffer is the userspace buffer to where you are writing the data you want to be read from the device file*/
	/* length is the length of the userspace buffer*/
	/* offset will be set to current position of the opened file after read*/
	/* copy_to_user function: source is device_buffer and destination is the userspace buffer *buffer */

	printk(KERN_ALERT "We are now in %s function \n",__FUNCTION__);
	int max_bytes = BUFFER_SIZE - *offset; // this will calculate the max number of bytes
	int bytes_in;// bytes to read in
	int bytes_read;// bytes already read in
	if(max_bytes > length)// checks if the max number of bytes is bigger than length if so, set to length
	{
	    bytes_in = length;
	}
	else // if not read in as many bytes as possible
	{
	    bytes_in = max_bytes;
	}
	if(bytes_in == 0)	
	{	
	    printk(KERN_ALERT "No bytes have been read in!!\n");
	} 

	// now printing the number of bytes and coping to user
	bytes_read = bytes_in - copy_to_user(buffer,device_buffer + *offset,bytes_in);
	printk(KERN_ALERT "The Device read : %d Bytes \n", bytes_read);
	*offset += bytes_read; // changed the position of the offset
	return bytes_read;
}

//this is the write function
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

ssize_t pa2_char_driver_write (struct file *pfile, const char __user *buffer, size_t length, loff_t *offset)
{
	/* *buffer is the userspace buffer where you are writing the data you want to be written in the device file*/
	/* length is the length of the userspace buffer*/
	/* current position of the opened file*/
	/* copy_from_user function: destination is device_buffer and source is the userspace buffer *buffer */

	printk(KERN_ALERT "We are now in %s function \n",__FUNCTION__);
	int max_bytes = BUFFER_SIZE - *offset; // this will calculate the max number of bytes
	int byte_write;
	int byte_to_write;
	if(max_bytes > length) //checks if max bytes is greater than length, if so set to length
	{
	   byte_to_write = length;
	}
	else
	{
	   byte_to_write = max_bytes;
	}
	byte_write = byte_to_write - copy_to_user(device_buffer + *offset, buffer, byte_to_write);
	printk(KERN_ALERT "The Device wrote : %d Bytes \n", byte_write);
	*offset += byte_write;
	

	return byte_write;
}

//this is the open function
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

int pa2_char_driver_open (struct inode *pinode, struct file *pfile) //inode represents the actual physical file on hard disk|| the abstract file contains all file operations
{
	/* print to the log file that the device is opened and also print the number of times this device has been opened until now*/
	printk(KERN_ALERT "The Device is open \n");
	num_open++; // keeps track of the number of times its been open
	printk(KERN_ALERT "The Device has opened this many times %d \n",num_open);
	return 0;
}

//this is the close function
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

int pa2_char_driver_close (struct inode *pinode, struct file *pfile)
{
	/* print to the log file that the device is closed and also print the number of times this device has been closed until now*/
	printk("The Device is closed \n");
	num_close++; //keeps track of number of time its been closed
	printk("The Device has been closed this many times %d \n",num_close);
	return 0;
}

//this is the seek function
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

loff_t pa2_char_driver_seek (struct file *pfile, loff_t offset, int whence) //in f.sh it seems like the "note" was already changed
{
	/* Update open file position according to the values of offset and whence */
	//loff_t is a long offset and is at least 64 bits wide

	printk(KERN_ALERT "We are now inside %s function \n",__FUNCTION__); // this shows that we are in the function 
	loff_t new_pos = 0; // this creates a new long offset
	switch(whence)
	{
	   case 0:// this is the SEEK_SET, it moves the file pointer to the beginning of the file
		new_pos = offset; // when the whence value is 0, the current position will be set to the value of the offset 
		break;
	   case 1:// this is SEEK_CUR, it moves the filer pointer to the given location (by incrementing offset bytes)
		new_pos = pfile->f_pos + offset; // the file pointer is changed to current location
		break;
	   case 2: // This is SEEK_END, the current position is set to offset bytes before the end of the file (moves the file pointer to the end of file)
		new_pos = BUFFER_SIZE - offset; // 1024 is the buffer size
		break;
	}

	if(new_pos < 0) // this checks if the user attempts to seek before the beginning of the file
	{
	   printk(KERN_ALERT "ERROR: cannot seek prior to buffer start");
	}
	if(new_pos > BUFFER_SIZE) // this checks if the user is trying to seek beyond end of the file
	{
	   printk(KERN_ALERT "ERROR: cannot seek past the buffer end");
	}

	pfile->f_pos = new_pos; // updates pointer
	
	printk(KERN_ALERT "Current offset after seeking is %d \n",new_pos);
		
	return new_pos;
}


//this is the file ops struct
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

struct file_operations pa2_char_driver_file_operations = {

	.owner   = THIS_MODULE,
	.open	 = pa2_char_driver_open,
	.read 	 = pa2_char_driver_read,
	.write   = pa2_char_driver_write,
        .release = pa2_char_driver_close, //"close" does not exist, so then we use release
	.llseek  = pa2_char_driver_seek   //llseek used
	/* add the function pointers to point to the corresponding file operations. look at the file fs.h in the linux souce code*/
};

//this is the init function
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static int pa2_char_driver_init(void)
{
	/* print to the log file that the init function is called.*/
	/* register the device */
	//Buffer size is set to 1024, GFP_KERNEL flag specifies the behavior of the memory allocator while trying to obtain the memory to return to the caller of kmalloc().
	//register_chrdev registers a major number for character devices
	
	printk(KERN_ALERT "We are now inside %s function\n",__FUNCTION__); // shows that this function is active
	device_buffer = kmalloc(BUFFER_SIZE,GFP_KERNEL); // allocates mem for objects smaller than page size in the kernel. (typical 4kb)
	register_chrdev(MAJOR_NUMBER,DEVICE_NAME, &pa2_char_driver_file_operations); // this function takes 3 params: major #, name of the device,and a pointer to the file ops

	return 0; // returns 0 if successful 
}

//this is the exit function
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static void pa2_char_driver_exit(void)
{
	/* print to the log file that the exit function is called.*/
	/* unregister  the device using the register_chrdev() function. */

	printk(KERN_ALERT "We are now inside %s, function\n",__FUNCTION__); // shows that this function is active
	if(device_buffer != NULL) // checks if device_buffer still active
	{
	   kfree(device_buffer); //frees a block of memory previously allocated with kmalloc()
	}
	else
	{
	   printk("kfree did not work properly");
	}
	
	unregister_chrdev(MAJOR_NUMBER,DEVICE_NAME); // this unregisters the character device driver this functions takes the major # and device name	
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/* add module_init and module_exit to point to the corresponding init and exit function*/

module_init(pa2_char_driver_init); // checks if the module is opened correctly
module_exit(pa2_char_driver_exit); // checks if the module is closed correctly
