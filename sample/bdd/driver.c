#include <linux/module.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/device.h>
#include <linux/kernel.h>

extern struct bus_type virtual_bus;
char *author = "LiWanPeng";

static ssize_t show_driver_author(struct device_driver *driver, char *buf){
	return snprintf(buf, PAGE_SIZE, "%s/n", author);
}

int my_driver_remove(struct device *dev){
	printk("driver is removed/n");
	return 0;
}	

int my_driver_probe(struct device *dev){
	printk("driver can handle the device/n");	
	return 0;
}

struct device_driver virtual_driver = {
	.name = "my_dev",
	.bus = &virtual_bus,
	.probe = my_driver_probe,
	.remove = my_driver_remove,
};	

static DRIVER_ATTR(author, S_IRUGO, show_driver_author, NULL); 

static int __init my_driver_init(void){
	int ret;
	ret = driver_register(&virtual_driver);
	if(ret)
		return ret;
	if(driver_create_file(&virtual_driver, &driver_attr_author))
		printk(KERN_NOTICE "Unable to create author attribute/n");
	printk("driver register success/n");
	return ret;
}

static void __exit my_driver_exit(void){
	driver_unregister(&virtual_driver);
}

module_init(my_driver_init);
module_exit(my_driver_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("liwanpeng");

