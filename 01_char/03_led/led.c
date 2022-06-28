#include <linux/fs.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <asm/io.h>
#include <linux/cdev.h>
#include <linux/device.h>

static char led_status[1];

struct led_dev{
    dev_t           devid;      /* 设备号 */
    struct cdev     cdev;       /* cdev  */
    struct class    *class;     /* class */
    struct device   *device;    /* device */
    char            *name;      /* name */
    int             major;      /* 主设备号 */
    int             minor;      /* 次设备号 */
};

static struct led_dev leddev;

/* GPJ2 寄存器组物理地址 */
#define GPJ2CON_PA      (0xE0200280)
#define GPJ2DAT_PA      (0xE0200284)
// #define GPJ2PUD_PA      (0xE0200288)
// #define GPJ2DRV_PA      (0xE020028C)
// #define GPJ2CONPDN_PA   (0xE0200290)
// #define GPJ2PUDPDN_PA   (0xE0200294)

/* 经MMU映射后的地址 */
static void __iomem *GPJ2CON_VA;
static void __iomem *GPJ2DAT_VA;
// static void __iomem *GPJ2PUD_VA;
// static void __iomem *GPJ2DRV_VA;
// static void __iomem *GPJ2CONPDN_VA;
// static void __iomem *GPJ2PUDPDN_VA;


static int led_open(struct inode *node, struct file *file) {
    file->private_data = &leddev;   /* 设置私有数据 */
	return 0;
}


static int led_release(struct inode *node, struct file *file) {
    return 0;
}


static ssize_t led_read(struct file *file, char __user *buf, size_t size, loff_t *offset) {
    int ret = copy_to_user(buf, led_status, 1);
    if( ret == 0 ) {
        printk("kernel senddata ok!\r\n");
    } else {
        printk("kernel senddata failed!\r\n");
    }
    return 0;
}


static ssize_t led_write(struct file *file, const char __user *buf, size_t size, loff_t *offset) {
    printk("kernel recvdata size:%u \r\n", size);
    if(size == 1) {
        int ret = copy_from_user(led_status, buf, size);
        if( ret == 0) {
            printk("kernel recvdata size:%u %x\r\n", size, led_status[0]);
            writel(led_status[0], GPJ2DAT_VA);
        } else {
            printk("kernel recvdata failed!\r\n");
        }
    }
    return 0;
}



static struct file_operations led_ops = {
    .owner      = THIS_MODULE,
    .open       = led_open,
    .release    = led_release,
    .read       = led_read,
    .write      = led_write
};



/* 模块入口 */
static int __init led_init(void) {

    /* 映射后的地址 */
    GPJ2CON_VA    = ioremap(GPJ2CON_PA, 4);
    GPJ2DAT_VA    = ioremap(GPJ2DAT_PA, 4);
    // GPJ2PUD_VA    = ioremap(GPJ2PUD_PA, 4);
    // GPJ2DRV_VA    = ioremap(GPJ2DRV_PA, 4);
    // GPJ2CONPDN_VA = ioremap(GPJ2CONPDN_PA, 4);
    // GPJ2PUDPDN_VA = ioremap(GPJ2PUDPDN_PA, 4);
    
    /* 使能时钟    -- 不需要 */
    /* 上下拉配置  -- 不需要 */

    /* 配置低16位为输出模式 */
    writel(0x00001111, GPJ2CON_VA);

    /* 默认置高，全部关闭  */
    led_status[0] = 0xFF;
    writel(led_status[0], GPJ2DAT_VA);

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

    return 0;
}

/* 模块出口 */
static void __exit led_exit(void) {

    /* 取消映射 */
    iounmap(GPJ2CON_VA);
    iounmap(GPJ2DAT_VA);
    // iounmap(GPJ2PUD_VA);
    // iounmap(GPJ2DRV_VA);
    // iounmap(GPJ2CONPDN_VA);
    // iounmap(GPJ2PUDPDN_VA);
    cdev_del(&leddev.cdev);
    unregister_chrdev_region(leddev.major, 1);
    device_destroy(leddev.class, leddev.devid);
    class_destroy(leddev.class);
}

MODULE_LICENSE("GPL");
module_init(led_init);
module_exit(led_exit);