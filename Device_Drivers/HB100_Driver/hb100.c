/* 
Raspberry Pi / BCM2711 Software-based UART Linux device driver
Copyright (C) 2014 Leonardo Ciocari

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

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

#define REG_CURRENT_TASK _IOW('a','a',int32_t*)

#define SIGETX 44
#define IRQ_NO 11
#define GPIO_25_IN (25)

static struct task_struct *task = NULL;
static int signum = 0;

unsigned int GPIO_irqNumber;

static irqreturn_t gpio_irq_handler(int irq,void *dev_id) 
{
  static unsigned long flags = 0;
  struct kernel_siginfo info;

#ifdef EN_DEBOUNCE
   unsigned long diff = jiffies - old_jiffie;
   if (diff < 20)
   {
     return IRQ_HANDLED;
   }
  
  old_jiffie = jiffies;
#endif  
  local_irq_save(flags);
  pr_info("Interrupt Occurred");
  
  memset(&info, 0, sizeof(struct kernel_siginfo));
  info.si_signo = SIGETX;
  info.si_code = SI_QUEUE;
  info.si_int = 1;

  if (task != NULL) {
        printk(KERN_INFO "Sending signal to app\n");
          if(send_sig_info(SIGETX, &info, task) < 0) {
              printk(KERN_INFO "Unable to send signal\n");
          }	
    }

  local_irq_restore(flags);
  return IRQ_HANDLED;
}

dev_t dev = 0;
static struct class *dev_class;
static struct cdev hb100_cdev;

int hb100_open(struct inode *, struct file *);
int hb100_release(struct inode *, struct file *);
long hb100_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
static ssize_t hb100_read(struct file *filp, char __user *buf, size_t len, loff_t * off);
struct file_operations hb100_fops = {
	.open = hb100_open,
	.release = hb100_release,
	.unlocked_ioctl = hb100_ioctl,
	.read = hb100_read, 
};

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Leonardo Ciocari");
MODULE_DESCRIPTION("Raspberry Pi / BCM2711 Soft-UART Driver");

int hb100_major = 60;

bool state = 0;

int hb100_open(struct inode *inode, struct file *filp)
{
	printk("Device Driver Opened");
	return 0;
}

int hb100_release(struct inode *inode, struct file *filp)
{
	struct task_struct *ref_task = get_current();
	printk("Device Driver Closed");

	if(ref_task == task)
		task = NULL;
	return 0;
}

long hb100_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	if (cmd == REG_CURRENT_TASK) {
        printk(KERN_INFO "REG_CURRENT_TASK\n");
        task = get_current();
        signum = SIGETX;
    }
	return 0;
}

ssize_t hb100_read(struct file *filp, char __user *buf, size_t len,loff_t * off)
{
	//copy_to_user(buf, state, 1);
	return 0;
}

//--------------------------------------------------------------------------------------

static int __init ModuleInit(void)
{
  /*Allocating Major number*/
  if((alloc_chrdev_region(&dev, 0, 1, "hb100_Dev")) <0){
    pr_err("Cannot allocate major number\n");
    goto r_unreg;
  }
  pr_info("Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));
  /*Creating cdev structure*/
  cdev_init(&hb100_cdev,&hb100_fops);
  /*Adding character device to the system*/
  if((cdev_add(&hb100_cdev,dev,1)) < 0){
    pr_err("Cannot add the device to the system\n");
    goto r_del;
  }
  /*Creating struct class*/
  if(IS_ERR(dev_class = class_create(THIS_MODULE,"hb100_class"))){
    pr_err("Cannot create the struct class\n");
    goto r_class;
  }
  /*Creating device*/
  if(IS_ERR(device_create(dev_class,NULL,dev,NULL,"hb100_device"))){
    pr_err( "Cannot create the Device \n");
    goto r_device;
  }
  
  //Input GPIO configuratioin
  //Checking the GPIO is valid or not
  if(gpio_is_valid(GPIO_25_IN) == false){
    pr_err("GPIO %d is not valid\n", GPIO_25_IN);
    goto r_gpio_in;
  }
  
  //Requesting the GPIO
  if(gpio_request(GPIO_25_IN,"GPIO_25_IN") < 0){
    pr_err("ERROR: GPIO %d request\n", GPIO_25_IN);
    goto r_gpio_in;
  }
  
  //configure the GPIO as input
  gpio_direction_input(GPIO_25_IN);
  
  /*
  ** I have commented the below few lines, as gpio_set_debounce is not supported 
  ** in the Raspberry pi. So we are using EN_DEBOUNCE to handle this in this driver.
  */ 
//#ifndef EN_DEBOUNCE
  //Debounce the button with a delay of 200ms
  if(gpio_set_debounce(GPIO_25_IN, 200) < 0){
    pr_err("ERROR: gpio_set_debounce - %d\n", GPIO_25_IN);
    //goto r_gpio_in;
  }
//#endif
  
  //Get the IRQ number for our GPIO
  GPIO_irqNumber = gpio_to_irq(GPIO_25_IN);
  pr_info("GPIO_irqNumber = %d\n", GPIO_irqNumber);
  
  if (request_irq(GPIO_irqNumber,             //IRQ number
                  (void *)gpio_irq_handler,   //IRQ handler
                  IRQF_TRIGGER_RISING,        //Handler will be called in raising edge
                  "hb100_device",               //used to identify the device name using this IRQ
                  NULL)) {                    //device id for shared IRQ
    pr_err("my_device: cannot register IRQ ");
    goto r_gpio_in;
  }
  
  pr_info("Device Driver Insert...Done!!!\n");
  return 0;
r_gpio_in:
  gpio_free(GPIO_25_IN);
r_device:
  device_destroy(dev_class,dev);
r_class:
  class_destroy(dev_class);
r_del:
  cdev_del(&hb100_cdev);
r_unreg:
  unregister_chrdev_region(dev,1);
  
  return -1;
}
//--------------------------------------------------------------------------------------

static void __exit ModuleExit(void)
{
  free_irq(GPIO_irqNumber,NULL);
  gpio_free(GPIO_25_IN);
  device_destroy(dev_class,dev);
  class_destroy(dev_class);
  cdev_del(&hb100_cdev);
  unregister_chrdev_region(dev, 1);
  pr_info("Device Driver Remove...Done!!\n");
}
//--------------------------------------------------------------------------------------
module_init(ModuleInit);
module_exit(ModuleExit);
