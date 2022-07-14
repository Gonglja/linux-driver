#include <linux/fs.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <asm/io.h>
#include <linux/cdev.h>
#include <linux/device.h>       /* struct class */
#include <linux/of.h>           /* of_xx */
#include <linux/of_address.h>   /* of_iomap */
#include <linux/of_gpio.h>      /* of_gpio gpio */
#include <linux/platform_device.h>/* platform */
#include <linux/mutex.h>
#include <linux/timer.h>



struct led_dev{
    dev_t               devid;      /* 设备号 */
    struct cdev         cdev;       /* cdev  */
    struct class        *class;     /* class */
    struct device       *device;    /* device */
    char                *name;      /* name */
    int                 major;      /* 主设备号 */
    int                 minor;      /* 次设备号 */
    struct device_node  *nd;        /* 设备节点 */
    int                 leds[4];    /* 4个led灯的gpio编号 */
    struct mutex        mutex;      /* 互斥量 */

    struct timer_list   timer;      /* timer */
    int                 timerperiod;/* period: ms */
    char                timer_data; /* timer data */
};



static struct led_dev leddev;

static int led_open(struct inode *node, struct file *file) 
{
    file->private_data = &leddev;   /* 设置私有数据 */

    // leddev.timerperiod = 1000;      /* default:1000ms */
    /* 获取互斥锁，进入休眠状态的进程可以被信号打断 */
	if(mutex_lock_interruptible(&leddev.mutex)) {
        return -ERESTARTSYS;
    }
#if 0
    mutex_lock(&leddev.mutex); /* 不能被信号打断 */
#endif
    return 0;
}


static int led_release(struct inode *node, struct file *file) 
{
    /* 释放互斥锁 */
    mutex_unlock(&leddev.mutex);
    return 0;
}


static struct file_operations led_ops = {
    .owner      = THIS_MODULE,
    .open       = led_open,
    .release    = led_release
};

void timer_callback(struct timer_list *tl) 
{
    struct led_dev *dev = from_timer(dev, tl, timer);
    static int sta = 0;
    unsigned long flags;

    /* bussiness: set led */
    sta = sta ? 0 : 1;
    gpio_set_value(dev->leds[0], sta);
    printk("timer_callback\r\n");
    /* restart timer */
    mod_timer(&dev->timer, jiffies + msecs_to_jiffies(dev->timerperiod));
}

static int led_probe(struct platform_device *pdev) 
{
    int ret;

    /* 获取设备树中的属性数据 */
    /* 1.获取设备节点：led */
    leddev.nd = of_find_node_by_path("/led");
    if( leddev.nd == NULL ) {
        printk("led node can not found!\r\n");
        return -EINVAL;
    } else {
        printk("led node has been found!\r\n");
    }

    /* 2.获取led-pin 属性内容 */
    for(int i=0; i<4; ++i) {
        leddev.leds[i] = of_get_named_gpio(leddev.nd, "led-pin", i);
        if(leddev.leds[i] < 0) {
            printk("can't get led-pin.\r\n");
            return -EINVAL;
        } else {
            printk("led-pin num = %d \r\n", leddev.leds[i]);
        }
    }
    
    /* 3. 设置pin脚为输出模式，并且输出高电平，默认关闭led */
    for(int i=0; i<4; ++i) {
        ret = gpio_direction_output(leddev.leds[i], 1);
        if(ret<0) {
            printk("can't set gpio%d!\r\n", leddev.leds[i]);
        }
    }
    
    leddev.name = "led";
    /* 如果主设备号不为空，则注册设备*/
    if(leddev.major) {
        leddev.devid = MKDEV(leddev.major, 0);
        register_chrdev_region(leddev.devid, 1, leddev.name);
    } else {
        /* 否则申请设备号 */
        alloc_chrdev_region(&leddev.devid, 0, 1, leddev.name);
        leddev.major = MAJOR(leddev.devid); /* 申请的主设备号  */
        leddev.minor = MINOR(leddev.devid); /* 申请的次设备号 */
    }
    /* cdev结构初始化，cdev 表示一个字符设备 */
    cdev_init(&leddev.cdev, &led_ops);

    /* cdev 添加设备*/
    cdev_add(&leddev.cdev, leddev.devid, 1);

    /* 可以通过 cat /proc/devices 查看 */
    printk("major:%d\r\n", leddev.major);

    /* class 创建类 */
    leddev.class = class_create(THIS_MODULE, leddev.name);

    /* device 创建设备 */
    leddev.device = device_create(leddev.class, NULL, leddev.devid, NULL, leddev.name);

    /* init timer */
    leddev.timerperiod = 1000;
    timer_setup(&leddev.timer, timer_callback, 0U);

    /* add timer */
    add_timer(&leddev.timer);

    mod_timer(&leddev.timer, jiffies + msecs_to_jiffies(leddev.timerperiod));

    return 0;
}

static int led_remove(struct platform_device *pdev) 
{
    /* del timer */
    del_timer(&leddev.timer);
    cdev_del(&leddev.cdev);
    unregister_chrdev_region(leddev.major, 1);
    device_destroy(leddev.class, leddev.devid);
    class_destroy(leddev.class);
    return 0;
}

static const struct of_device_id of_leds_match[] = {
    { .compatible = "s5pv210,led", },
    {},
};
MODULE_DEVICE_TABLE(of, of_leds_match);

static struct platform_driver leds_driver = {
    .probe      = led_probe,
    .remove     = led_remove,
    .driver     = {
        .name   = "leds",
        .of_match_table = of_leds_match,
    },
};


static int __init led_init(void) 
{
    printk("led_init\r\n");
    /* 初始化互斥锁 */
    mutex_init(&leddev.mutex);
    return platform_driver_register(&leds_driver);
}


static void __exit led_exit(void) 
{
    printk("led_exit\r\n");
    platform_driver_unregister(&leds_driver);
}

MODULE_LICENSE("GPL");
module_init(led_init);
module_exit(led_exit);