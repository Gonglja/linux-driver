#include <linux/fs.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <asm/io.h>
#include <linux/cdev.h>
#include <linux/device.h>       /* struct class */
#include <linux/of.h>           /* of_xx */
#include <linux/of_address.h>   /* of_iomap */
#include <linux/of_gpio.h>      /* of_gpio gpio */
#include <linux/platform_device.h>

static char buzzer_status[1];

struct buzzer_dev{
    dev_t               devid;      /* 设备号 */
    struct cdev         cdev;       /* cdev  */
    struct class        *class;     /* class */
    struct device       *device;    /* device */
    char                *name;      /* name */
    int                 major;      /* 主设备号 */
    int                 minor;      /* 次设备号 */
    struct device_node  *nd;        /* 设备节点 */
    int                 gpionum;    /* buzzer的gpio编号 */
};

static struct buzzer_dev bzdev;


static int buzzer_open(struct inode *node, struct file *file) 
{
    file->private_data = &bzdev;   /* 设置私有数据 */
	return 0;
}


static int buzzer_release(struct inode *node, struct file *file) 
{
    return 0;
}


static ssize_t buzzer_read(struct file *file, char __user *buf, size_t size, loff_t *offset) 
{
    int ret = copy_to_user(buf, buzzer_status, 1);
    if( ret == 0 ) {
        printk("kernel senddata ok!\r\n");
    } else {
        printk("kernel senddata failed!\r\n");
    }
    return 0;
}


static ssize_t buzzer_write(struct file *file, const char __user *buf, size_t size, loff_t *offset) 
{
    struct buzzer_dev *dev = file->private_data;
    printk("kernel recvdata size:%u \r\n", size);
    if(size == 1) {
        int ret = copy_from_user(buzzer_status, buf, size);
        if( ret == 0) {
            printk("kernel recvdata size:%u %x\r\n", size, buzzer_status[0]);
            gpio_set_value(dev->gpionum, buzzer_status[0] ? 1 : 0);
        } else {
            printk("kernel recvdata failed!\r\n");
        }
    }
    return 0;
}



static struct file_operations buzzer_ops = {
    .owner      = THIS_MODULE,
    .open       = buzzer_open,
    .release    = buzzer_release,
    .read       = buzzer_read,
    .write      = buzzer_write
};



static int buzzer_probe(struct platform_device *pdev) 
{
    int ret;

    /* 获取设备树中的属性数据 */
    /* 1.获取设备节点：buzzer */
    bzdev.nd = of_find_node_by_path("/buzzer");
    if( bzdev.nd == NULL ) {
        printk("buzzer node can not found!\r\n");
        return -EINVAL;
    } else {
        printk("buzzer node has been found!\r\n");
    }

    /* 2.获取led-pin 属性内容 */
    bzdev.gpionum = of_get_named_gpio(bzdev.nd, "gpio-pin", 0);
    if(bzdev.gpionum < 0) {
        printk("can't get led-pin.\r\n");
        return -EINVAL;
    } else {
        printk("gpio-pin num = %d \r\n", bzdev.gpionum);
    }
    
    
    /* 3. 设置pin脚为输出模式，并且输出低电平，默认关闭buzzer */
    ret = gpio_direction_output(bzdev.gpionum, 0);
    if(ret<0) {
        printk("can't set gpio%d!\r\n", bzdev.gpionum);
    }
    
    
    bzdev.name = "buzzer";
    /* 如果主设备号不为空，则注册设备*/
    if(bzdev.major) {
        bzdev.devid = MKDEV(bzdev.major, 0);
        register_chrdev_region(bzdev.devid, 1, bzdev.name);
    } else {
        /* 否则申请设备号 */
        alloc_chrdev_region(&bzdev.devid, 0, 1, bzdev.name);
        bzdev.major = MAJOR(bzdev.devid); /* 申请的主设备号  */
        bzdev.minor = MINOR(bzdev.devid); /* 申请的次设备号 */
    }
    /* cdev结构初始化，cdev 表示一个字符设备 */
    cdev_init(&bzdev.cdev, &buzzer_ops);

    /* cdev 添加设备*/
    cdev_add(&bzdev.cdev, bzdev.devid, 1);

    /* 可以通过 cat /proc/devices 查看 */
    printk("major:%d\r\n", bzdev.major);

    /* class 创建类 */
    bzdev.class = class_create(THIS_MODULE, bzdev.name);

    /* device 创建设备 */
    bzdev.device = device_create(bzdev.class, NULL, bzdev.devid, NULL, bzdev.name);
    return 0;
}

static int buzzer_remove(struct platform_device *pdev) 
{
    cdev_del(&bzdev.cdev);
    unregister_chrdev_region(bzdev.major, 1);
    device_destroy(bzdev.class, bzdev.devid);
    class_destroy(bzdev.class);
    return 0;
}

static const struct of_device_id of_buzzer_match[] = {
    { .compatible = "smart210,buzzer", },
    {},
};
MODULE_DEVICE_TABLE(of, of_buzzer_match);

static struct platform_driver buzzer_driver = {
    .probe      = buzzer_probe,
    .remove     = buzzer_remove,
    .driver     = {
        .name   = "buzzer",
        .of_match_table = of_buzzer_match,
    },
};


static int __init buzzer_init(void) 
{
    printk("buzzer_init\r\n");
    return platform_driver_register(&buzzer_driver);
}


static void __exit buzzer_exit(void) 
{
    printk("buzzer_exit\r\n");
    platform_driver_unregister(&buzzer_driver);
}

MODULE_LICENSE("GPL");
module_init(buzzer_init);
module_exit(buzzer_exit);