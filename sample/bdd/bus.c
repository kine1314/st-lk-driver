#include <linux/module.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/device.h>
#include <linux/kernel.h>

static char *author = "LiWanPeng";

static ssize_t show_bus_author(struct bus_type *bus, char *buf){
	return snprintf(buf, PAGE_SIZE, "%s\n", author);
}

void my_bus_release(struct device *dev){
	printk(KERN_DEBUG "my bus release\n");	
}	

static int virtual_bus_match(struct device *dev, struct device_driver *drv){
	return !strncmp(dev_name(dev), drv->name, strlen(drv->name));
}

struct bus_type virtual_bus = {
	.name = "my_bus",
	.match = virtual_bus_match,
};

struct device my_bus = {
	.init_name = "my_bus0",
	.release = my_bus_release,
};

EXPORT_SYMBOL(my_bus);
EXPORT_SYMBOL(virtual_bus);

static BUS_ATTR(author, S_IRUGO, show_bus_author, NULL); 

static int __init bus_init(void){
	int ret;
	ret = bus_register(&virtual_bus);
	if(ret)
		return ret;
	if(bus_create_file(&virtual_bus, &bus_attr_author))
		printk(KERN_NOTICE "Unable to create author attribute\n");
	ret = device_register(&my_bus);
	if(ret)
		printk(KERN_NOTICE "Fail to register device\n");
	printk("bus regiter success\n");
	return ret;
}

static void __exit bus_exit(void){
	bus_unregister(&virtual_bus);
	device_unregister(&my_bus);
}

module_init(bus_init);
module_exit(bus_exit);
MODULE_LICENSE("GPL");   

