#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/string.h>
#include <linux/module.h>
#include <linux/platform_device.h>

static struct platform_device *my_device;

static int __init platform_dev_init(void) {
    int ret;

    //分配结构
    my_device = platform_device_alloc("my_dev", -1);
    //注册设备
    ret = platform_device_add(my_device);

    if(ret)
        printk("platform_device_add failed!\n");

    return ret;
}

static void __exit platform_dev_exit(void) {
    platform_device_unregister(my_device);//卸载设备
}

module_init(platform_dev_init);
module_exit(platform_dev_exit);
MODULE_LICENSE("GPL");
