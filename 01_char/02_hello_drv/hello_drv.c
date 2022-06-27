#include <linux/fs.h>
#include <linux/module.h>

static int major = 0;

static int hello_drv_open(struct inode *, struct file *) {
    printk("[%s %s line %d]:", __FILE__, __FUNCTION__, __LINE__);
    printk("hello_open\r\n");
	return 0;
}


static int hello_drv_release(struct inode *, struct file *) {
    printk("[%s %s line %d]:", __FILE__, __FUNCTION__, __LINE__);
    printk("hello_release\r\n");
    return 0;
}


static ssize_t hello_drv_read(struct file *file, char __user *buff, size_t size, loff_t *offset) {
    printk("[%s %s line %d]:", __FILE__, __FUNCTION__, __LINE__);
    printk("size:%ld\r\n",size);
    return 0;
}


static ssize_t hello_drv_write(struct file *, const char __user *, size_t, loff_t *) {
    printk("[%s %s line %d]:", __FILE__, __FUNCTION__, __LINE__);
    // printk("size:%d\r\n");
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
    printk("hello_drv_init\r\n");
    major = register_chrdev(0, "hello_drv", &hello_drv_ops);
    // 可以通过 cat /proc/devices 查看
    printk("major:%d\r\n", major);
    return 0;
}

/* 模块出口 */
static void __exit hello_drv_exit(void) {
    printk("hello_drv_exit\r\n");
    unregister_chrdev(major, "hello_drv");
}

MODULE_LICENSE("GPL");
module_init(hello_drv_init);
module_exit(hello_drv_exit);