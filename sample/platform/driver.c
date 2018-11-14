#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/string.h>
#include <linux/module.h>
#include <linux/platform_device.h>

static int my_probe(struct device *dev) {
    printk("Driver found device!\n");
    return 0;
}

static int my_remove(struct device *dev) {
    printk("Driver found device unpluged!\n");
    return 0;
}
//定义platform_driver结构体
static struct platform_driver my_driver = {
    .probe	= my_probe,
    .remove = my_remove,	
    .driver = {
        .owner = THIS_MODULE,
        .name = "my_dev",
    },
};

static int __init my_driver_init(void) {
    return platform_driver_register(&my_driver);
}

static void __exit my_driver_exit(void) {
    platform_driver_unregister(&my_driver);
}

module_init(my_driver_init);
module_exit(my_driver_exit);
MODULE_LICENSE("GPL");
