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

#define MMAPNOPAGE_DEV_NAME "mmapnopage"
#define MMAPNOPAGE_DEV_MAJOR 240

#define SHARE_MEM_PAGE_COUNT 4
#define SHARE_MEM_SIZE (PAGE_SIZE*SHARE_MEM_ PAGE_COUNT)
char *share_memory = NULL;

struct page *mmapnopage_vm_nopage(struct vm_area_struct *vma, unsigned long addr, int *type)
{

    struct page *page;
    unsigned long offset;
    void *page_ptr;

    printk("CALL NOPAGE %08X\n", addr);
    page = NOPAGE_SIGBUS;
    page_ptr = NULL;

    if((NULL == vma)||(share_memory == NULL)) return NOPAGE_SIGBUS;

    offset = addr - vma->vm_start;

    if(offset >= SHARE_MEM_SIZE) return NOPAGE_SIGBUS;

    page_ptr = share_memory + offset;
    page = vmalloc_to_page(page_ptr);
    get_page(page);

    if (type) *type = VM_FAULT_MINOR;

    return page;
}

struct vm_operations_struct mmapnopage_vm_ops =
{
    .nopage = mmapnopage_vm_nopage,
};

int mmapnopage_mmap(struct file *filp , struct vm_area_ struct *vma)
{
    vma->vm_flags |= VM_RESERVED;
    vma->vm_ops = &mmapnopage_vm_ops;
    return 0;
}

struct file_operations mmapnopage_fops =
{
    .owner = THIS_MODULE,
    .mmap = mmapnopage_mmap,
};

int mmapnopage_init(void)
{
    int lp;
    int result;

    result = register_chrdev(MMAPNOPAGE_DEV_MAJOR, MMAPNOPAGE_DEV_NAME, &mmapnopage_fops);
    if (result < 0) return result;

    share_memory = vmalloc(SHARE_MEM_SIZE);
    for(lp = 0; lp < SHARE_MEM_PAGE_COUNT; lp++)
    {
        sprintf(share_memory + PAGE_SIZE * lp, "TEST %d", lp);
    }

    return 0;
}

void mmapnopage_exit(void)
{
    if(share_memory != NULL) vfree(share_memory);
    unregister_chrdev(MMAPNOPAGE_DEV_MAJOR, MMAPNOPAGE_DEV_NAME);
}

module_init(mmapnopage_init);
module_exit(mmapnopage_exit);

MODULE_LICENSE("Kine BSD/GPL");
