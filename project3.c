#include <linux/init.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/semaphore.h>
#include <linux/slab.h>

MODULE_LICENSE("DUAL BSD/GPL");
MODULE_VERSION("0.1");

// semaphore declaration
static struct semaphore empty;
static struct semaphore full;
static struct semaphore mut;

int readPointer, writePointer, counter, BUFF_SIZE=10;
int numSemaphore=5;
char **BUFFER;
//static int index;

static struct miscdevice charDev;//declaring device

// redifining function prototypes
static int my_open(struct inode *, struct file *);
static int my_close(struct  inode *, struct file *);
static ssize_t my_read(struct file *, char *, size_t, loff_t *);
static ssize_t my_write(struct file *, const char *, size_t, loff_t *);

// callback operations structure
static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = my_open,
	.read = my_read,
	.write = my_write,
	.release = my_close
};


// initialization module
int __init init_module(void) {
	int chk_status;
	// initializing miscellaneous device driver
	charDev.minor = MISC_DYNAMIC_MINOR;
	charDev.name = "mypipe";
	charDev.fops = &fops;

	// initializing buffer space
	chk_status = 0;
	chk_status = misc_register(&charDev);
	
	
	sema_init(&mut, 1);
	sema_init(&full, 0);
	sema_init(&empty, numSemaphore);

	if(chk_status >= 0){
		printk(KERN_ALERT "Character Device mypipe initialized successfully");
		printk(KERN_ALERT "\nCharacter Device Registered Id :%d\n", chk_status);
	}
	else{
		printk(KERN_ALERT "Character Device mypipe initialization failed\n");
	}
	
	if((BUFFER = kmalloc(sizeof(char*) * 25, GFP_KERNEL))<0)
		printk(KERN_ALERT "kmalloc failed\n");

	counter = BUFF_SIZE;
	readPointer = 0;
	writePointer = 0;
	
	return chk_status;
}


// open driver module
static int my_open(struct inode* in, struct file* f) {
	printk(KERN_ALERT"Character Device mypipe opened \n");
	return 0;
}

// close driver module
static int my_close(struct  inode* in, struct file* f) {
	printk(KERN_ALERT "Character Device mypipe closed \n");
	return 0;
}
//producer
static ssize_t my_write(struct file* f, const char* ch, size_t len, loff_t* off) {
	short count = 0;
	int retValue;
	printk(KERN_ALERT "In my write\n");
	// entering critical section
	if( down_interruptible(&empty) < 0){			
		printk(KERN_ALERT "User exit manually");	
		return -1;
	}
	if( down_interruptible(&mut) < 0){			
		printk(KERN_ALERT "User exit manually");	
		return -1;
	}
	if(writePointer==10){
		writePointer = 0;		
		}
	if((BUFFER[writePointer] = kmalloc(sizeof(char) * BUFF_SIZE, GFP_KERNEL))<0)
		printk(KERN_ALERT "kmalloc failed\n");
	printk(KERN_ALERT "Before alert\n");
	
	printk(KERN_ALERT "In while After alert\n");
		printk(KERN_ALERT "ch- - %s\n",ch);
	

		
		
		if( (retValue = copy_from_user(BUFFER[writePointer], ch,len+1))<0)
			printk(KERN_ALERT "copy_from_user error");
		
		//printk(KERN_ALERT "BUFFER - %s	[writePointer] - %d \n",BUFFER,writePointer);
		count=strlen(ch);
		
	
		writePointer++;
		
		
	
	printk(KERN_ALERT "out while After alert\n");
	//printing buffer in msg log for debug
	
		
	printk(KERN_ALERT "\n");
	up(&mut);
	up(&full);
	
	return count;
}

//consumer
static ssize_t my_read(struct file* f, char* ch, size_t len, loff_t* off) {
	int count = 0;
	int retValue;
	// entering critical section
	if( down_interruptible(&full) < 0){			
		printk(KERN_ALERT "User exit manually");	
		return -1;
	}
	if( down_interruptible(&mut) < 0){			
		printk(KERN_ALERT "User exit manually");	
		return -1;
	}
	if(readPointer==10){
		readPointer = 0;		
		}
	
		//printk(KERN_ALERT "BUFFER read - %s [readPointer] - %d	\n",BUFFER,readPointer);
		//int  	retValue;
		
		if( (retValue = copy_to_user(ch,BUFFER[readPointer],len+1))<0)
			printk(KERN_ALERT "copy_to_user error");
		count=strlen(ch);
		
		printk(KERN_ALERT "ch- - %s\n",ch);
		
		counter++;
		kfree(BUFFER[readPointer]);
		readPointer++;
		
	up(&mut);
	up(&empty);
	return count;
}


// exit module
void __exit cleanup_module(void) {
	printk(KERN_ALERT "Character Device mypipe cleaned up and unregistered\n");
	kfree(BUFFER);				/* Free buffer */
	misc_deregister(&charDev);
}
