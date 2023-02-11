#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/uaccess.h>  
#include <linux/gpio.h>     
#include <linux/err.h>

#define GPIO_18 (18)

dev_t dev = 0;
static struct class *dev_class;
static struct cdev mq2_cdev;
 
static int __init mq2_driver_init(void);
static void __exit mq2_driver_exit(void);

static int mq2_open(struct inode *inode, struct file *file);
static int mq2_release(struct inode *inode, struct file *file);
static ssize_t mq2_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t mq2_write(struct file *filp, const char *buf, size_t len, loff_t * off);

static struct file_operations fops =
{
  .owner          = THIS_MODULE,
  .read           = mq2_read,
  .write          = mq2_write,
  .open           = mq2_open,
  .release        = mq2_release,
};

static int mq2_open(struct inode *inode, struct file *file)
{
  pr_info("MQ2: Device File Opened\n");
  return 0;
}

static int mq2_release(struct inode *inode, struct file *file)
{
  pr_info("MQ2: Device File Closed\n");
  return 0;
}

static ssize_t mq2_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
  uint8_t gpio_state = 0;
  
  //reading GPIO value
  gpio_state = gpio_get_value(GPIO_18);
  
  //write to user
  len = 1;
  if( copy_to_user(buf, &gpio_state, len) > 0) {
    pr_err("MQ2: ERROR: Data not copied\n");
  }
  
  pr_info("MQ2: Read function : GPIO_18 = %d \n", gpio_state);
  
  return len;
}

static ssize_t mq2_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
  return 0;
}

static int __init mq2_driver_init(void)
{
  //Allocating Major number
  if((alloc_chrdev_region(&dev, 0, 1, "mq2_Dev")) <0){
    pr_err("MQ2: Cannot allocate major number\n");
    goto r_unreg;
  }
  pr_info("MQ2: Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));
 
  //Creating cdev structure
  cdev_init(&mq2_cdev,&fops);
 
  //Adding character device to the system
  if((cdev_add(&mq2_cdev,dev,1)) < 0){
    pr_err("MQ2: Cannot add the device to the system\n");
    goto r_del;
  }
 
  //Creating struct class
  if(IS_ERR(dev_class = class_create(THIS_MODULE,"mq2_class"))){
    pr_err("MQ2: Cannot create the struct class\n");
    goto r_class;
  }
 
  //Creating device
  if(IS_ERR(device_create(dev_class,NULL,dev,NULL,"mq2_device"))){
    pr_err( "MQ2: Cannot create the Device \n");
    goto r_device;
  }
  
  //Checking the GPIO is valid or not
  if(gpio_is_valid(GPIO_18) == false){
    pr_err("MQ2: GPIO %d is not valid\n", GPIO_18);
    goto r_device;
  }
  
  //Requesting the GPIO
  if(gpio_request(GPIO_18,"GPIO_18") < 0){
    pr_err("MQ2: ERROR: GPIO %d request\n", GPIO_18);
    goto r_gpio;
  }
  
  //configure the GPIO as input
  gpio_direction_input(GPIO_18);
  
  pr_info("MQ2: Device Driver Inserted\n");
  return 0;
 
r_gpio:
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

static void __exit mq2_driver_exit(void)
{
  gpio_free(GPIO_18);
  device_destroy(dev_class,dev);
  class_destroy(dev_class);
  cdev_del(&mq2_cdev);
  unregister_chrdev_region(dev, 1);
  pr_info("MQ2: Device Driver Removed\n");
}
 
module_init(mq2_driver_init);
module_exit(mq2_driver_exit);
 
MODULE_LICENSE("GPL");