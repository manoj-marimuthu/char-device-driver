#include<linux/module.h>
#include<linux/init.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>
#include<linux/fs.h>
#include<linux/device.h>
#include<linux/mutex.h>

#define BUFFER_SIZE 1024

MODULE_AUTHOR("Manoj K M");

static DEFINE_MUTEX(buffer_lock);
static dev_t dev_no;
static struct cdev char_cdev;
static size_t data_len;
static char buffer[BUFFER_SIZE];

static int char_dev_open(struct inode* inode,struct file* file){
	printk("Character device - open operation invoked by kernel \n");
	return 0;
}

static ssize_t char_dev_read(struct file* file, char __user *buf, size_t count,loff_t* offset){
	size_t bytes,remaining;
	size_t len = data_len;
	if(mutex_lock_interruptible(&buffer_lock)){
		return -ERESTARTSYS;
	}
	if(*offset >= len){
		mutex_unlock(&buffer_lock);
		return 0;
	}
	remaining = len - *offset;
	bytes = min(count,remaining);
	if(copy_to_user(buf,buffer + *offset,bytes)){
		mutex_unlock(&buffer_lock);
		return -EFAULT;
	}
	*offset += bytes;
	mutex_unlock(&buffer_lock);
	printk("Character device - read operation invoked by kernel \n");
	return bytes;
}

static ssize_t char_dev_write(struct file* file,const char __user * buf, size_t count, loff_t* offset){
	size_t bytes;
	if(mutex_lock_interruptible(&buffer_lock)){
		return -ERESTARTSYS;
	}
	bytes = min(count, (size_t) BUFFER_SIZE);
	if(copy_from_user(buffer,buf,bytes)){
		mutex_unlock(&buffer_lock);
		return -EFAULT;
	}
	data_len = bytes;
	mutex_unlock(&buffer_lock);
	printk("Character device - write operation invoked by kernel \n");
	return bytes;

}

static struct file_operations fops = {
	.open = char_dev_open,
	.read = char_dev_read,	
	.write = char_dev_write,
};

static struct class *char_dev_class;
static const struct device* char_device;

static int __init char_driver_init(void){
	printk("Initializing Character device driver module\n");
	int res;
	res = alloc_chrdev_region(&dev_no,0,1,"char_device");
	
	if(res < 0){
		printk("Character Device region allocation failed \n");
		return res;
	}

	printk("Received Major number = %d, Minor number = %d",MAJOR(dev_no),MINOR(dev_no));
	cdev_init(&char_cdev,&fops);
	res = cdev_add(&char_cdev,dev_no,1);

	if(res < 0){
		printk("Character device addition failed \n");
		unregister_chrdev_region(dev_no,1);
		return res;
	}
	
	char_dev_class = class_create("char device class");

	if(IS_ERR(char_dev_class)){
		res = PTR_ERR(char_dev_class);
		unregister_chrdev_region(dev_no,1);
		return res;
	}

	char_device = device_create(char_dev_class,NULL,dev_no,NULL,"char_device");

	if(IS_ERR(char_device)){
		res = PTR_ERR(char_dev_class);
		class_destroy(char_dev_class);
		unregister_chrdev_region(dev_no,1);
		return res;
	}
	return 0;
}

static void __exit char_driver_exit(void){
	device_destroy(char_dev_class,dev_no);
	class_destroy(char_dev_class);
	unregister_chrdev_region(dev_no,1);
	printk("Character device driver module terminated \n");
}

module_init(char_driver_init);
module_exit(char_driver_exit);
MODULE_LICENSE("GPL");

