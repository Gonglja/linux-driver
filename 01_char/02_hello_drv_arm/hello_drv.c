#include <linux/fs.h>
#include <linux/module.h>
#include <linux/uaccess.h>

static int major = 0;
static char buff[1024];

static int hello_drv_open(struct inode *node, struct file *file) {
    // printk("[%s %s line %d]:", __FILE__, __FUNCTION__, __LINE__);
    printk("hello_drv_open.\r\n");
	return 0;
}


static int hello_drv_release(struct inode *node, struct file *file) {
    // printk("[%s %s line %d]:", __FILE__, __FUNCTION__, __LINE__);
    printk("hello_drv_release.\r\n");
    return 0;
}


static ssize_t hello_drv_read(struct file *file, char __user *buf, size_t size, loff_t *offset) {
    // printk("[%s %s line %d]:", __FILE__, __FUNCTION__, __LINE__);
    int ret = copy_to_user(buf, buff, size);
    if( ret == 0 ) {
        printk("kernel senddata ok!\r\n");
    } else {
        printk("kernel senddata failed!\r\n");
    }
    printk("ret:%d\r\n",ret);
    return 0;
}


static ssize_t hello_drv_write(struct file *file, const char __user *buf, size_t size, loff_t *offset) {
    // printk("[%s %s line %d]:", __FILE__, __FUNCTION__, __LINE__);
    int ret = copy_from_user(buff, buf, size);
    if( ret == 0) {
        printk("kernel recvdata size:%zu %s\r\n", size, buff);
    } else {
        printk("kernel recvdata failed!\r\n");
    }
    return 0;
}



static struct file_operations hello_drv_ops = {
    .owner      = THIS_MODULE,
    .open       = hello_drv_open,
    .release    = hello_drv_release,
    .read       = hello_drv_read,
    .write      = hello_drv_write
};



/* 模块入口 */
static int __init hello_drv_init(void) {
    printk("-----------------------------------------------\r\n");
    printk("hello_drv_init.\r\n");
    major = register_chrdev(0, "hello_drv", &hello_drv_ops);
    // 可以通过 cat /proc/devices 查看
    printk("major:%d\r\n", major);
    return 0;
}

/* 模块出口 */
static void __exit hello_drv_exit(void) {
    printk("hello_drv_exit.\r\n");
    unregister_chrdev(major, "hello_drv");
}

MODULE_LICENSE("GPL");
module_init(hello_drv_init);
module_exit(hello_drv_exit);
