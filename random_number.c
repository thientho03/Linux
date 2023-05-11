#include <linux/module.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/random.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/string.h>

#define DEVICE_NAME "random_number"

static char number[10];

struct Random_drv 
{
    dev_t dev_num;
    struct class *dev_class;
    struct device *dev;  
    struct cdev randdev;
} Random_drv;

// Cac ham entry POINT
static int drive_open(struct inode *inode, struct file *filp)
{
	printk("Drive: open()\n");
    return 0;
}

static ssize_t drive_read(struct file *filp, char __user *buffer, size_t len, loff_t *offset)
{
	unsigned int rand;
	char temp[10];
	int i = 0, j;
	
	if (*offset > 0)
		return 0;

	get_random_bytes(&rand, sizeof(rand));
	rand = rand % __INT_MAX__;
	printk("Drive: read()\n");
	printk(" %d\n", rand);

	while (i < 9 && rand > 0)
	{	
		int x = rand % 10;
		temp[i++] = x + '0';
		rand /= 10;
	}
	temp[i] = '\0';
	for (j = 0; j < i; j++)
		number[j] = temp[i - j - 1]; 
	number[i] = '\0';

	if (copy_to_user(buffer, number, strlen(number)) != 0)
		return -EFAULT;

	buffer[i] = '\0';
	(*offset) += strlen(number);

    return strlen(number);
}

static int drive_release(struct inode *inode, struct file *filp)
{
	printk("Drive: close()\n");
    return 0;
}

static struct file_operations fops =
{
    .owner = THIS_MODULE,
    .open = drive_open,
	.read = drive_read,
	.release = drive_release
};

static int __init init_random_number(void)
{
    int ret = 0;

	// Cap phat dong Device Number
    Random_drv.dev_num = 0;
    ret = alloc_chrdev_region(&Random_drv.dev_num, 0, 1, "Random_device");
    if (ret < 0)
    {
        printk("Failed to register device number dynamically\n");
        return ret;
    }
    printk("Allocated device number (%d,%d)\n", MAJOR(Random_drv.dev_num), MINOR(Random_drv.dev_num));
	
	// Tao Device File
    Random_drv.dev_class = class_create(THIS_MODULE, "class_Random_dev");
    if (IS_ERR(Random_drv.dev_class))
    {
        printk("Failed to create a device class\n");
        unregister_chrdev_region(Random_drv.dev_num, 1);
		return -1;
    }

    Random_drv.dev = device_create(Random_drv.dev_class, NULL, Random_drv.dev_num, NULL, "Random_dev");
	if(IS_ERR(Random_drv.dev))
	{
		printk("Failed to create device file");
		class_destroy(Random_drv.dev_class);
		unregister_chrdev_region(Random_drv.dev_num, 1);
		return -1;
	}

	// Dang ky Entry Point
    cdev_init(&Random_drv.randdev, &fops);
	ret = cdev_add(&Random_drv.randdev, Random_drv.dev_num, 1);
	if (ret < 0)
	{
		printk("Failed to add entry point!");
    	device_destroy(Random_drv.dev_class, Random_drv.dev_num);
		class_destroy(Random_drv.dev_class);
		unregister_chrdev_region(Random_drv.dev_num, 1);

		return -1;
	}

    printk("Initialize random number driver successfully\n");
    return 0;
}

static void __exit exit_randum_number(void)
{
    cdev_del(&Random_drv.randdev);
    device_destroy(Random_drv.dev_class, Random_drv.dev_num);
	class_destroy(Random_drv.dev_class);
    unregister_chrdev_region(Random_drv.dev_num, 1);

    printk("Random number driver exited\n");
}

module_init(init_random_number);
module_exit(exit_randum_number);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ngan-Ngoc-Tho");
MODULE_DESCRIPTION(DEVICE_NAME);
MODULE_SUPPORTED_DEVICE("testdevice");