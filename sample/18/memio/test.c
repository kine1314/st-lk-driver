#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fcntl.h>

#include <linux/vmalloc.h>
#include <asm/uaccess.h>
#include <asm/io.h>

#include <asm/page.h>
#include <linux/mm.h>
#include <linux/slab.h>

#define MMAPIOMEM_DEV_NAME "mmapiomem"
#define MMAPIOMEM_DEV_MAJOR 240

#define VIDEO_TEXT_ADDR 0xB8000
#define VIDEO_TEXT_SIZE 0x8000

int mmapiomem_open (struct inode *inode, struct file *filp)
{
    return 0;
}

int mmapiomem_release (struct inode *inode, struct file *filp)
{
    return 0;
}

int mmapiomem_mmap(struct file *filp , struct vm_area_struct *vma)
{
    vma->vm_flags |= VM_RESERVED;
    vma->vm_flags |= VM_IO;

    if(remap_page_range(vma, vma->vm_start, VIDEO_TEXT_ADDR, VIDEO_TEXT_SIZE, vma->vm_page_prot))
        return -EAGAIN;

    return 0;
}

struct file_operations mmapiomem_fops =
{
    .owner = THIS_MODULE,
    .open = mmapiomem_open,
    .release = mmapiomem_release,
    .mmap = mmapiomem_mmap,
};

int mmapiomem_init(void)
{
    int result;

    result = register_chrdev(MMAPIOMEM_DEV_MAJOR, MMAPIOMEM_DEV_NAME,&mmapiomem_fops);

    if (result < 0) return result;

    return 0;
}

void mmapiomem_exit(void)
{
    unregister_chrdev(MMAPIOMEM_DEV_MAJOR, MMAPIOMEM_DEV_NAME);
}

module_init(mmapiomem_init);
module_exit(mmapiomem_exit);

MODULE_LICENSE("Kine BSD/GPL");
