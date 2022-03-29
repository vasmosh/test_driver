#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>

#include <linux/version.h>

#include <linux/uaccess.h>
#include <linux/ioctl.h>
#include <asm/errno.h>



#include <linux/proc_fs.h>


#define MY_MAJOR_NUM (115)
#define DRIVER_NAME "hello"


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



struct hrtimer hrtimer_test;
unsigned int count;
ktime_t m_kt;
int value = 2000;

static enum hrtimer_restart  hrtimer_test_poll(struct hrtimer *timer){
    printk("============ count = %d ========== \n", count++);
    hrtimer_forward(timer, timer->base->get_time(), m_kt);
    return HRTIMER_NORESTART;
}


int mydev_init(void){

    printk("init my drive \n");

    struct proc_dir_entry *dev_dir = NULL;
    register_chrdev(MY_MAJOR_NUM, DRIVER_NAME, &fops);

    hrtimer_init(&hrtimer_test, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
    hrtimer_test.function = hrtimer_test_poll;
    m_kt = ktime_set(value/1000, (value % 1000)*1000000);
    hrtimer_start(&hrtimer_test, m_kt, HRTIMER_MODE_REL);


    return 0;
}

void mydev_exit(void){
    printk("exit my drive \n");
    unregister_chrdev(MY_MAJOR_NUM, DRIVER_NAME);
    hrtimer_cancel(&hrtimer_test);

}



MODULE_LICENSE("GPL");
MODULE_AUTHOR("MVA");

module_init(mydev_init);
module_exit(mydev_exit);