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
#define DEV_MAJOR		241

#define DEVICE_DESC		"PI no-DMA"

#define PERROR(ERRSTR, ERRNO)	printk(KERN_ERR "pind: %s %d\n", ERRSTR, ERRNO);

// file operations prototypes
static ssize_t pind_read(struct file *filp, char *buffer, size_t length,
	loff_t * offset);
static ssize_t pind_write(struct file *filp, const char *buffer, size_t length,
	loff_t *ppos);

#if DEV_MAJOR == 0
static u8 dev_major;
#endif

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.read  = pind_read,
	.write = pind_write
};

static struct gpio pins[] = {
	{6, GPIOF_OUT_INIT_LOW, "LED_B"},
	{19, GPIOF_OUT_INIT_LOW, "LED_G"},
	{26, GPIOF_OUT_INIT_LOW, "LED_R"},
};

static int __init pind_init(void) {
	int err;

	if ((err = gpio_request_array(pins, ARRAY_SIZE(pins)))) {
		PERROR("gpio_request_array", err);
		return -1;
	}

	if ((err = register_chrdev(DEV_MAJOR, DEVICE_DESC, &fops)) < 0) {
		PERROR("register_chrdev", err);
		return -1;
	}

#if DEV_MAJOR == 0
	dev_major = err;
	printk(KERN_INFO "pind: Device major: %hhu\n", dev_major);
#endif

	printk(KERN_INFO "pind: Driver initialized\n");
	return 0;
}

static void __exit pind_exit(void) {
#if DEV_MAJOR == 0
	unregister_chrdev(dev_major, DEVICE_DESC);
#else
	unregister_chrdev(DEV_MAJOR, DEVICE_DESC);
#endif
	gpio_free_array(pins, ARRAY_SIZE(pins));
	printk(KERN_INFO "pind: Driver unloaded\n");
}

static ssize_t pind_read(struct file *filp, char *buffer, size_t length, loff_t *ppos) {
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

static ssize_t pind_write(struct file *filp, const char *buffer,
	size_t length, loff_t *ppos) {
	const pin_data *data = (pin_data *) buffer;
	if (length != sizeof(pin_data))
		return -EINVAL;
	gpio_set_value(data->pin, data->value);
	return sizeof(pin_data);
}

module_init(pind_init);
module_exit(pind_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tiago Koji Castro Shibata <tiago.shibata@thunderatz.org> <tishi@linux.com>");
MODULE_DESCRIPTION("PI no-DMA");
