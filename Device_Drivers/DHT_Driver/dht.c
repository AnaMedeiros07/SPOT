#include <asm/io.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/hrtimer.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/interrupt.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/gpio.h>

dev_t dev = 0;
static struct class *dev_class;
static struct cdev dht_cdev;

int dht_open(struct inode *, struct file *);
int dht_release(struct inode *, struct file *);
long dht_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

struct file_operations dht_fops = {
	.open = dht_open,
	.release = dht_release,
	.unlocked_ioctl = dht_ioctl,
};

int dht_open(struct inode *inode, struct file *filp)
{
	printk("Device Driver Opened");
	return 0;
}

int dht_release(struct inode *inode, struct file *filp)
{
	printk("Device Driver Closed");

	return 0;
}

long dht_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	return 0;
}

static int __init ModuleInit(void)
{
  /*Allocating Major number*/
  if((alloc_chrdev_region(&dev, 0, 1, "dht_Dev")) <0){
    pr_err("Cannot allocate major number\n");
    goto r_unreg;
  }
  pr_info("Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));
  /*Creating cdev structure*/
  cdev_init(&dht_cdev,&dht_fops);
  /*Adding character device to the system*/
  if((cdev_add(&dht_cdev,dev,1)) < 0){
    pr_err("Cannot add the device to the system\n");
    goto r_del;
  }
  /*Creating struct class*/
  if(IS_ERR(dev_class = class_create(THIS_MODULE,"dht_class"))){
    pr_err("Cannot create the struct class\n");
    goto r_class;
  }
  /*Creating device*/
  if(IS_ERR(device_create(dev_class,NULL,dev,NULL,"dht_device"))){
    pr_err( "Cannot create the Device \n");
    goto r_device;
  }
  
  pr_info("Device Driver Insert...Done!!!\n");
  return 0;

r_device:
  device_destroy(dev_class,dev);
r_class:
  class_destroy(dev_class);
r_del:
  cdev_del(&dht_cdev);
r_unreg:
  unregister_chrdev_region(dev,1);
  
  return -1;
}
//--------------------------------------------------------------------------------------

static void __exit ModuleExit(void)
{
  device_destroy(dev_class,dev);
  class_destroy(dev_class);
  cdev_del(&dht_cdev);
  unregister_chrdev_region(dev, 1);
  pr_info("Device Driver Remove...Done!!\n");
}
//--------------------------------------------------------------------------------------
module_init(ModuleInit);
module_exit(ModuleExit);