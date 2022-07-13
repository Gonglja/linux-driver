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


static char key_status[1];

struct key_dev{
    dev_t               devid;      /* 设备号 */
    struct cdev         cdev;       /* cdev  */
    struct class        *class;     /* class */
    struct device       *device;    /* device */
    char                *name;      /* name */
    int                 major;      /* 主设备号 */
    int                 minor;      /* 次设备号 */
    struct device_node  *nd;        /* 设备节点 */
    int                 keys[8];    /* 8个key的gpio编号 */
    struct mutex        mutex;      /* 互斥量 */
};

static struct key_dev keydev;

static int key_open(struct inode *node, struct file *file) 
{
    file->private_data = &keydev;   /* 设置私有数据 */

    /* 获取互斥锁，进入休眠状态的进程可以被信号打断 */
	if(mutex_lock_interruptible(&keydev.mutex)) {
        return -ERESTARTSYS;
    }
#if 0
    mutex_lock(&keydev.mutex); /* 不能被信号打断 */
#endif
    return 0;
}


static int key_release(struct inode *node, struct file *file) 
{
    /* 释放互斥锁 */
    mutex_unlock(&keydev.mutex);
    return 0;
}


static ssize_t key_read(struct file *file, char __user *buf, size_t size, loff_t *offset) 
{
    int ret = copy_to_user(buf, key_status, 1);
    if( ret == 0 ) {
        printk("kernel senddata ok!\r\n");
    } else {
        printk("kernel senddata failed!\r\n");
    }
    return 0;
}


static ssize_t key_write(struct file *file, const char __user *buf, size_t size, loff_t *offset) 
{
    struct key_dev *dev = file->private_data;
    printk("kernel recvdata size:%u \r\n", size);
    if(size == 1) {
        int ret = copy_from_user(key_status, buf, size);
        if( ret == 0) {
            printk("kernel recvdata size:%u %x\r\n", size, key_status[0]);
            for(int i=0; i<4; ++i) {
                // if(key_status[0] & (0x1<<i)) {  /* 关闭LED */
                //     gpio_set_value(dev->leds[i], 1);
                // } else {                        /* 打开LED */
                //     gpio_set_value(dev->leds[i], 0);
                // }
            }
        } else {
            printk("kernel recvdata failed!\r\n");
        }
    }
    return 0;
}



static struct file_operations key_ops = {
    .owner      = THIS_MODULE,
    .open       = key_open,
    .release    = key_release,
    .read       = key_read,
    .write      = key_write
};



static int keys_probe(struct platform_device *pdev) 
{
    int ret;

    /* 获取设备树中的属性数据 */
    /* 1.获取设备节点：led */
    keydev.nd = of_find_node_by_path("/keys");
    if( keydev.nd == NULL ) {
        printk("keys node can not found!\r\n");
        return -EINVAL;
    } else {
        printk("keys node has been found!\r\n");
    }

    /* 2.获取led-pin 属性内容 */
    struct device_node *first_child_node=NULL;
    for(int i=0; i<8; ++i) {
        /* 获取下一个子节点，传入两个参数，第一个为父节点，第二个如果为NULL，则返回第一个子节点，如果为上一个子节点，则返回下一个子节点*/
        first_child_node = of_get_next_child(keydev.nd, first_child_node);
        keydev.keys[i] = of_get_named_gpio(first_child_node, "gpio-pin", 0);
        if(keydev.keys[i] < 0) {
            printk("can't get gpio-pin.\r\n");
            return -EINVAL;
        } else {
            printk("gpio-pin num[%d] = %d \r\n", i, keydev.keys[i]);
        }
    }
    
    /* 3. 设置pin脚为输入模式 */
    for(int i=0; i<8; ++i) {
        ret = gpio_direction_input(keydev.keys[i]);
        if(ret<0) {
            printk("can't set gpio%d!\r\n", keydev.keys[i]);
        }
    }
    
    keydev.name = "key";
    /* 如果主设备号不为空，则注册设备*/
    if(keydev.major) {
        keydev.devid = MKDEV(keydev.major, 0);
        register_chrdev_region(keydev.devid, 1, keydev.name);
    } else {
        /* 否则申请设备号 */
        alloc_chrdev_region(&keydev.devid, 0, 1, keydev.name);
        keydev.major = MAJOR(keydev.devid); /* 申请的主设备号  */
        keydev.minor = MINOR(keydev.devid); /* 申请的次设备号 */
    }
    /* cdev结构初始化，cdev 表示一个字符设备 */
    cdev_init(&keydev.cdev, &key_ops);

    /* cdev 添加设备*/
    cdev_add(&keydev.cdev, keydev.devid, 1);

    /* 可以通过 cat /proc/devices 查看 */
    printk("major:%d\r\n", keydev.major);

    /* class 创建类 */
    keydev.class = class_create(THIS_MODULE, keydev.name);

    /* device 创建设备 */
    keydev.device = device_create(keydev.class, NULL, keydev.devid, NULL, keydev.name);
    return 0;
}

static int keys_remove(struct platform_device *pdev) 
{
    cdev_del(&keydev.cdev);
    unregister_chrdev_region(keydev.major, 1);
    device_destroy(keydev.class, keydev.devid);
    class_destroy(keydev.class);
    return 0;
}

static const struct of_device_id of_keys_match[] = {
    { .compatible = "s5pv210,keys", },
    {},
};
MODULE_DEVICE_TABLE(of, of_keys_match);

static struct platform_driver keys_driver = {
    .probe      = keys_probe,
    .remove     = keys_remove,
    .driver     = {
        .name   = "keys",
        .of_match_table = of_keys_match,
    },
};


static int __init keys_init(void) 
{
    printk("key_init\r\n");
    /* 初始化互斥锁 */
    mutex_init(&keydev.mutex);
    return platform_driver_register(&keys_driver);
}


static void __exit keys_exit(void) 
{
    printk("key_exit\r\n");
    platform_driver_unregister(&keys_driver);
}

MODULE_LICENSE("GPL");
module_init(keys_init);
module_exit(keys_exit);