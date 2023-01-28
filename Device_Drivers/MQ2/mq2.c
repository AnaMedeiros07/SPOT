#include <asm/io.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/hrtimer.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/interrupt.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/gpio.h>
#include <linux/slab.h>

#define GPIO_18 (18)

MODULE_LICENSE("GPL v2");

dev_t dev = 0;
static struct class *dev_class;
static struct cdev mq2_cdev;

uint8_t kernel_buffer;

int mq2_open(struct inode *, struct file *);
int mq2_release(struct inode *, struct file *);
static ssize_t mq2_read(struct file *filp, char __user *buf, size_t len,loff_t * off);

struct file_operations mq2_fops = {
	.open           = mq2_open,
	.release        = mq2_release,
  .read           = mq2_read,
};

int mq2_major = 60;

bool state = 0;

int mq2_open(struct inode *inode, struct file *filp)
{
	printk("Device Driver Opened");
	return 0;
}

int mq2_release(struct inode *inode, struct file *filp)
{
	printk("Device Driver Closed");
	return 0;
}

static ssize_t mq2_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
  uint8_t gpio_state = 0;
  
  //reading GPIO value
  gpio_state = gpio_get_value(GPIO_18);
  kernel_buffer = gpio_get_value(GPIO_18);
  //snprintf(kernel_buffer, mem_size , "%d", gpio_state);
  //write to user
  printk("%d", kernel_buffer);
  if( copy_to_user(buf, &kernel_buffer, 1)) {
    pr_err("ERROR\n");
  }
  
  pr_info("Read function : GPIO_18 = %d \n", gpio_state);
  
  return 0;
}
//--------------------------------------------------------------------------------------

static int __init ModuleInit(void)
{
  /*Allocating Major number*/
  if((alloc_chrdev_region(&dev, 0, 1, "mq2_Dev")) <0){
    pr_err("Cannot allocate major number\n");
    goto r_unreg;
  }
  pr_info("Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));
  /*Creating cdev structure*/
  cdev_init(&mq2_cdev,&mq2_fops);
  /*Adding character device to the system*/
  if((cdev_add(&mq2_cdev,dev,1)) < 0){
    pr_err("Cannot add the device to the system\n");
    goto r_del;
  }
  /*Creating struct class*/
  if(IS_ERR(dev_class = class_create(THIS_MODULE,"mq2_class"))){
    pr_err("Cannot create the struct class\n");
    goto r_class;
  }
  /*Creating device*/
  if(IS_ERR(device_create(dev_class,NULL,dev,NULL,"mq2_device"))){
    pr_err( "Cannot create the Device \n");
    goto r_device;
  }
  
  //Allocate Kernel Buffer space

  //Input GPIO configuratioin
  //Checking the GPIO is valid or not
  if(gpio_is_valid(GPIO_18) == false){
    pr_err("GPIO %d is not valid\n", GPIO_18);
    goto r_gpio_in;
  }
  
  //Requesting the GPIO
  if(gpio_request(GPIO_18,"GPIO_18") < 0){
    pr_err("ERROR: GPIO %d request\n", GPIO_18);
    goto r_gpio_in;
  }
  
  //configure the GPIO as input
  gpio_direction_input(GPIO_18); 
  
  pr_info("MQ2 Driver Insert...Done!!!\n");
  return 0;
r_gpio_in:
  gpio_free(GPIO_18);
r_device:
  device_destroy(dev_class,dev);
r_class:
  class_destroy(dev_class);
r_del:
  cdev_del(&mq2_cdev);
r_unreg:
  unregister_chrdev_region(dev,1);
  
  return -1;
}
//--------------------------------------------------------------------------------------

static void __exit ModuleExit(void)
{
  gpio_free(GPIO_18);
  device_destroy(dev_class,dev);
  class_destroy(dev_class);
  cdev_del(&mq2_cdev);
  unregister_chrdev_region(dev, 1);
  pr_info("Device Driver Remove...Done!!\n");
}
//--------------------------------------------------------------------------------------
module_init(ModuleInit);
module_exit(ModuleExit);