#include <linux/module.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/device.h>

char *author = "LiWanPeng";
extern struct bus_type virtual_bus;
extern struct device my_bus;

static ssize_t show_device_author(struct device *dev, struct device_attribute *attr, char *buf){
	return snprintf(buf, PAGE_SIZE, "%s\n", author);
}

void virtual_device_release(struct device *dev){
	printk("virtual_device is released\n");
}

struct device virtual_device ={
	//.init_name = "my_dev",
	.bus = &virtual_bus,
	.parent = &my_bus,
	.release = virtual_device_release,
};

static DEVICE_ATTR(author, S_IRUGO, show_device_author, NULL); 

static int __init device_init(void){
	int ret;

        dev_set_name(&virtual_device, "my_dev");
	ret = device_register(&virtual_device);
	if(ret)
		return ret;
	if(device_create_file(&virtual_device, &dev_attr_author))
		printk(KERN_NOTICE "Unable to create author attribute\n");
	printk("device register success\n");
	return ret;
}

static void __exit device_exit(void){
	device_unregister(&virtual_device);
}

module_init(device_init);
module_exit(device_exit);
MODULE_AUTHOR("liwanpeng");
MODULE_LICENSE("GPL");

