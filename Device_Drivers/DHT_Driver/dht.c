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
#include <linux/delay.h>
#include <linux/slab.h>

#define mem_size        256

#define GPIO_4 (4)
#define MAXTIMINGS	85

MODULE_LICENSE("GPL v2");

uint8_t *kernel_buffer;

dev_t dev = 0;
static struct class *dev_class;
static struct cdev dht_cdev;

int dht_open(struct inode *, struct file *);
int dht_release(struct inode *, struct file *);
static ssize_t dht_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t dht_write(struct file *filp, const char *buf, size_t len, loff_t * off);

struct file_operations dht_fops = {
	.owner = THIS_MODULE,
  .open = dht_open,
	.release = dht_release,
  .read = dht_read,
  .write = dht_write,
};

int dht_open(struct inode *inode, struct file *filp)
{
	printk("DHT: Device Driver Opened");
	return 0;
}

int dht_release(struct inode *inode, struct file *filp)
{
	printk("DHT: Device Driver Closed");

	return 0;
}

static ssize_t dht_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{

  bool laststate	= 1;
	uint8_t counter		= 0;
	uint8_t j		= 0, i;
	float	f; 

  uint8_t dht11_dat[5] = { 0, 0, 0, 0, 0 };

  pr_info("DHT: Read Operation\n");

  gpio_direction_output(GPIO_4,0);
  mdelay(20);
  gpio_direction_output(GPIO_4,1);
  udelay(30);
  gpio_direction_input(GPIO_4);

for ( i = 0; i < MAXTIMINGS; i++ )
	{
		counter = 0;
		while ( gpio_get_value(GPIO_4) == laststate )
		{
			counter++;
      udelay(1);
			if ( counter == 255 )
			{
				break;
			}
		}
		laststate = gpio_get_value(GPIO_4);
 
		if ( counter == 255 )
			break;
 
		if ( (i >= 4) && (i % 2 == 0) )
		{
			dht11_dat[j / 8] <<= 1;
			if ( counter > 16 )
				dht11_dat[j / 8] |= 1;
			j++;
		}
	}
 
	if ( (j >= 40) && (dht11_dat[4] == ( (dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) ) ) )
  {
    f = dht11_dat[2] * 9. / 5. + 32;
    printk( "Humidity = %d.%d Temperature = %d.%d C\n",
      dht11_dat[0], dht11_dat[1], dht11_dat[2], dht11_dat[3]);
    
    snprintf(kernel_buffer, mem_size , "%d.%d %d.%d ", 
      dht11_dat[0], dht11_dat[1], dht11_dat[2], dht11_dat[3]);
  }
  else  
  {
    pr_info("Checksum failed\n");
    snprintf(kernel_buffer, mem_size , "Checksum failed");
	}
  
  if( copy_to_user(buf, kernel_buffer, mem_size) )
  {
          pr_err("Data Read : Err!\n");
  }
  
  return mem_size;
}

static ssize_t dht_write(struct file *filp, const char *buf, size_t len, loff_t * off)
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
    pr_err("DHT: Cannot add the device to the system\n");
    goto r_del;
  }
  /*Creating struct class*/
  if(IS_ERR(dev_class = class_create(THIS_MODULE,"dht_class"))){
    pr_err("DHT: Cannot create the struct class\n");
    goto r_class;
  }
  /*Creating device*/
  if(IS_ERR(device_create(dev_class,NULL,dev,NULL,"dht_device"))){
    pr_err( "DHT: Cannot create the Device \n");
    goto r_device;
  }
  
  if((kernel_buffer = kmalloc(mem_size, GFP_KERNEL)) == 0 ){
    pr_info("Cannot allocate memory in kernel");
    goto r_device;
  }

  if(gpio_is_valid(GPIO_4) == 0){
    pr_err("GPIO %d is not valid\n", GPIO_4);
    goto r_gpio_in;
  }

  if(gpio_request(GPIO_4,"GPIO_4") < 0){
    pr_err("ERROR: GPIO %d request\n", GPIO_4);
    goto r_gpio_in;
  }

  //gpio_direction_output(GPIO_4,1);
  gpio_direction_input(GPIO_4);
  pr_info("DHT: Device Inserted\n");
  return 0;

r_gpio_in:
  gpio_free(GPIO_4);
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
  gpio_free(GPIO_4);
  kfree(kernel_buffer);
  device_destroy(dev_class,dev);
  class_destroy(dev_class);
  cdev_del(&dht_cdev);
  unregister_chrdev_region(dev, 1);
  pr_info("DHT: Device Driver Remove...Done!!\n");
}
//--------------------------------------------------------------------------------------
module_init(ModuleInit);
module_exit(ModuleExit);