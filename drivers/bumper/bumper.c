#warning tem include a mais aqui
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/wait.h>
#include <linux/ktime.h>
#include <linux/sched.h>
#include <linux/kdev_t.h>
#include <linux/mutex.h>

#include <asm/io.h>
#include <asm/delay.h>
#include <mach/platform.h>

// see linux/Documentation/devices.txt (240-254 char = LOCAL/EXPERIMENTAL USE)
// use 0 for dynamic
#define DEV_MAJOR		242

#define DEVICE_DESC		"bumper"

#define PERROR(ERRSTR, ERRNO)	printk(KERN_ERR "bumper: %s %d\n", ERRSTR, ERRNO);

DECLARE_WAIT_QUEUE_HEAD(bumper_event);

// file operations prototypes
static ssize_t bumper_read(struct file *filp, char *buffer, size_t length,
	loff_t * offset);
static ssize_t bumper_write(struct file *filp, const char *buffer, size_t length,
	loff_t *ppos);

#if DEV_MAJOR == 0
static u8 dev_major;
#endif

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.read  = bumper_read
};

static struct gpio pins[] = {
	{21, GPIOF_IN | GPIOF_OPEN_DRAIN, "Bumper"},
	{20, GPIOF_IN | GPIOF_OPEN_DRAIN, "Bigode L"},
	{26, GPIOF_IN | GPIOF_OPEN_DRAIN, "Bigode R"}
};

static irqreturn_t bumper_handler(int irq, void *dev_id, struct pt_regs *regs) {
	wake_up(&bumper_event);
	return IRQ_HANDLED;
}

static int __init bumper_init(void) {
	int err;

	if ((err = gpio_request_array(pins, ARRAY_SIZE(pins)))) {
		PERROR("gpio_request_array", err);
		return -1;
	}

	if ((err = register_chrdev(DEV_MAJOR, DEVICE_DESC, &fops)) < 0) {
		PERROR("register_chrdev", err);
		return -1;
	}

	printk(KERN_INFO "bumper: Driver initialized\n");
	return 0;
}

static void __exit bumper_exit(void) {
	unregister_chrdev(DEV_MAJOR, DEVICE_DESC);
	gpio_free_array(pins, ARRAY_SIZE(pins));
	printk(KERN_INFO "bumper: Driver unloaded\n");
}

static ssize_t bumper_read(struct file *filp, char *buffer, size_t length, loff_t *ppos) {
	int err;
	if ((err = gpio_get_value(length)) < 0) {
		PERROR("gpio_get_value", err);
		return -EIO;
	}
	*buffer = err;
	return 1;
}

typedef struct {
	int pin, value;
} pin_data;

module_init(bumper_init);
module_exit(bumper_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tiago Koji Castro Shibata <tiago.shibata@thunderatz.org> <tishi@linux.com>");
MODULE_DESCRIPTION("Bumpers");
