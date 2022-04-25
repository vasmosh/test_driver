#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>

#include <linux/version.h>

#include <linux/uaccess.h>
#include <linux/ioctl.h>
#include <asm/errno.h>



#include <linux/proc_fs.h>

#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#define MY_MAJOR_NUM (115)
#define DRIVER_NAME "hello"

static struct proc_dir_entry *proc_dir_root = NULL;
static struct proc_dir_entry *proc_dir_devices1 = NULL;
static struct proc_dir_entry *proc_dir_devices2 = NULL;
static struct proc_dir_entry *proc_dir_devices3 = NULL;



long fops_ioctl(struct file *p_file, unsigned int cmd, unsigned long arg){

    char tmp[100];
    char tmp1[100];

    printk("cmd: %d \n", cmd);

    switch (cmd){
        case 1:
            if(copy_from_user(&tmp, (void *)arg, sizeof(tmp))) return -EINVAL;
            strcpy(tmp1, "ioctl rw test 3");

            if(copy_to_user((void *)arg, &tmp1, sizeof(tmp1))) return -EINVAL;
            return strlen(tmp1);
            break;
        case 3:
            if(copy_from_user(&tmp, (void *)arg, sizeof(tmp))) return -EINVAL;
            printk("rx ioctl tmp %s \n", tmp);
            return 0;
            break;
        default:
            return -ENOSYS;
    }

}

int fops_open(struct inode *p_node, struct file *p_file){

    // pre init

    return 0;
}

ssize_t fops_read(struct file *p_file, char __user *p_user, size_t count, loff_t *f_pos){

    char buf[100];
    strcpy(buf, "test");
    if(copy_to_user(p_user, &buf, sizeof(buf))) return -EFAULT;

    return strlen(buf);

}

ssize_t fops_write(struct file *p_file, const char __user *p_user, size_t count, loff_t *f_pos){

//copy_to_user()
    char tmp[100];
    if(copy_from_user(&tmp, (void *)p_user, sizeof(tmp))) return -EINVAL;
    printk("rx data tmp = %s \n", tmp);

    return 0;
}

static struct file_operations fops ={
        .owner = THIS_MODULE,
        .open = fops_open,
        .read = fops_read,
        .write = fops_write,
        .unlocked_ioctl = fops_ioctl,
};



int POWER = 0;
int POWER2 = 0;
module_param(POWER, int, 0);
module_param(POWER2, int, 0);



static int open_device_info(struct inode *, struct file *);
static ssize_t read_file_all(struct file *, char __user *, size_t, loff_t *);
static int release_file_all(struct inode *, struct file *);

struct private_data {
    size_t length;
    struct mutex lock;
    char data_for_user[0];
};



static int hello_proc_show(struct seq_file *m, void *v) {
    seq_printf(m, "Hello proc!\n");
    return 0;
}

static int hello_proc_open(struct inode *inode, struct  file *file) {
    return single_open(file, hello_proc_show, NULL);
}



static const struct proc_ops myprocfops = {
        .proc_open = hello_proc_open,
        .proc_read = seq_read,
//        .proc_release = single_release,
};




int mydev_init(void){

    printk("init my drive \n");

    struct proc_dir_entry *dev_dir = NULL;
    register_chrdev(MY_MAJOR_NUM, DRIVER_NAME, &fops);

    proc_dir_root = proc_mkdir(DRIVER_NAME, NULL);

    proc_dir_devices1 = proc_mkdir("folder1", proc_dir_root);
    proc_dir_devices2 = proc_mkdir("folder2", proc_dir_root);
    proc_dir_devices3 = proc_mkdir("folder3", proc_dir_root);

    proc_create("hello_proc", 0444, proc_dir_devices1, &myprocfops);
    proc_create("hello_proc", 0444, proc_dir_devices2, &myprocfops);

    return 0;
}

void mydev_exit(void){

    remove_proc_subtree("hello", NULL);
    printk("exit my drive \n");
    unregister_chrdev(MY_MAJOR_NUM, DRIVER_NAME);

}



MODULE_LICENSE("GPL");
MODULE_AUTHOR("MVA");

module_init(mydev_init);
module_exit(mydev_exit);