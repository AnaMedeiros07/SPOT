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
#include <linux/jiffies.h>

#define REG_CURRENT_TASK _IOW('a','a',int32_t*)

#define SIGETX 44
#define IRQ_NO 11
#define GPIO_25_IN (25)

#define TIMEOUT 1000
#define MIN_COUNT 4

static struct task_struct *task = NULL;
static int signum = 0;

static struct timer_list timeout_timer;
static struct hrtimer hrtimer_timeout;

unsigned int GPIO_irqNumber;

int count = 0;

dev_t dev = 0;
static struct class *dev_class;
static struct cdev hb100_cdev;

int hb100_open(struct inode *, struct file *);
int hb100_release(struct inode *, struct file *);
long hb100_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

struct file_operations hb100_fops = {
	.open = hb100_open,
	.release = hb100_release,
	.unlocked_ioctl = hb100_ioctl,
};

MODULE_LICENSE("GPL v2");

int hb100_major = 60;

bool state = 0;

static irqreturn_t gpio_irq_handler(int irq,void *dev_id) 
{
  static unsigned long flags = 0;

  local_irq_save(flags);
  pr_info("Interrupt Occurred");
  
  count++;

  local_irq_restore(flags);
  return IRQ_HANDLED;
}

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

/*
static enum hrtimer_restart RestartTimeout(struct hrtimer * unused)
{
  hrtimer_forward_now(&hrtimer_timeout, ktime_set(0,TIMEOUT * 1000000));

  return HRTIMER_RESTART;
}
*/

void timer_callback(struct timer_list * data)
{
    struct kernel_siginfo info;
    /* do your timer stuff here */
    printk("1 Seconds");

    if(count > MIN_COUNT)
    {
      memset(&info, 0, sizeof(struct kernel_siginfo));
      info.si_signo = SIGETX;
      info.si_code = SI_QUEUE;
      info.si_int = 1;

      if (task != NULL) {
            printk(KERN_INFO "Sending signal to app\n");
            count = 0;
            if(send_sig_info(SIGETX, &info, task) < 0)
                printk(KERN_INFO "Unable to send signal\n");
    }

    }
    /*
       Re-enable timer. Because this function will be called only first time. 
       If we re-enable this will work like periodic timer. 
    */
    mod_timer(&timeout_timer, jiffies + msecs_to_jiffies(TIMEOUT));
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
  
  timer_setup(&timeout_timer, timer_callback, 0);

  mod_timer(&timeout_timer, jiffies + msecs_to_jiffies(5000));

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
  del_timer(&timeout_timer);
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
