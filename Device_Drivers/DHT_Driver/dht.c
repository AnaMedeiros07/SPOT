#include <asm/io.h>
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
#include <linux/delay.h>   
#include <linux/err.h>
#include <linux/slab.h>

#define GPIO_4 (4)

#define MAXTIMINGS 85

dev_t dev = 0;
static struct class *dev_class;
static struct cdev dht_cdev;
 
static int __init dht_driver_init(void);
static void __exit dht_driver_exit(void);

static int dht_open(struct inode *inode, struct file *file);
static int dht_release(struct inode *inode, struct file *file);
static ssize_t dht_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t dht_write(struct file *filp, const char *buf, size_t len, loff_t * off);

static struct file_operations fops =
{
  .owner          = THIS_MODULE,
  .read           = dht_read,
  .write          = dht_write,
  .open           = dht_open,
  .release        = dht_release,
};

typedef struct DHT{
    unsigned char data[5];
    char humidity_int;
    char humidity_dec;
    char temperature_int;
    char temperature_dec;
} dht_t;

static int dht_open(struct inode *inode, struct file *file)
{
  pr_info("DHT: Device File Opened\n");
  return 0;
}

static int dht_release(struct inode *inode, struct file *file)
{
  pr_info("DHT: Device File Closed\n");
  return 0;
}



static ssize_t dht_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
  bool laststate = 1;
  uint8_t counter = 0;
  uint8_t j=0, i;
  float f; 

  dht_t sensor;

  //uint8_t ret = sizeof(sensor);

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
              sensor.data[j / 8] <<= 1;
              if ( counter > 16 )
                  sensor.data[j / 8] |= 1;
              j++;
          }
      }
 
	if ( (j >= 40) && (sensor.data[4] == ( (sensor.data[0] + sensor.data[1] + sensor.data[2] + sensor.data[3]) ) ) )
  {
      f = sensor.data[2] * 9. / 5. + 32;
      printk( "Humidity = %d.%d Temperature = %d.%d C\n",
      sensor.data[0], sensor.data[1], sensor.data[2], sensor.data[3]);
      
      sensor.humidity_int = sensor.data[0];
      sensor.humidity_dec = sensor.data[1];
      sensor.temperature_int = sensor.data[2];
      sensor.temperature_dec = sensor.data[3];
      
  } else  
  {
      pr_info("Checksum failed\n");
      return 0;
  }
  
    if( copy_to_user(buf, (void*)&sensor, sizeof(sensor)) )
    {
        pr_err("Data Read : Err!\n");
        return 0;
    }
  
    return sizeof(sensor);
}

static ssize_t dht_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
  return 0;
}

static int __init dht_driver_init(void)
{
  /*Allocating Major number*/
  if((alloc_chrdev_region(&dev, 0, 1, "dht_Dev")) <0){
    pr_err("DHT: Cannot allocate major number\n");
    goto r_unreg;
  }
  pr_info("DHT: Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));
 
  /*Creating cdev structure*/
  cdev_init(&dht_cdev,&fops);
 
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
  
  //Checking the GPIO is valid or not
  if(gpio_is_valid(GPIO_4) == false){
    pr_err("DHT: GPIO %d is not valid\n", GPIO_4);
    goto r_device;
  }
  
  //Requesting the GPIO
  if(gpio_request(GPIO_4,"GPIO_4") < 0){
    pr_err("DHT: ERROR: GPIO %d request\n", GPIO_4);
    goto r_gpio;
  }
  
  //configure the GPIO as input
  gpio_direction_input(GPIO_4);
  
  pr_info("DHT: Device Driver Inserted\n");
  return 0;
 
r_gpio:
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

static void __exit dht_driver_exit(void)
{
  gpio_free(GPIO_4);
  device_destroy(dev_class,dev);
  class_destroy(dev_class);
  cdev_del(&dht_cdev);
  unregister_chrdev_region(dev, 1);
  pr_info("DHT: Device Driver Removed\n");
}
 
module_init(dht_driver_init);
module_exit(dht_driver_exit);
 
MODULE_LICENSE("GPL");