#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/moduleparam.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include<linux/slab.h>                 //kmalloc()
#include<linux/uaccess.h>              //copy_to/from_user()
#include <linux/ioctl.h>
#include<linux/proc_fs.h>
#include <linux/kthread.h>
#include <linux/wait.h>
#include<linux/sysfs.h> 
#include<linux/kobject.h> 
#include <linux/interrupt.h>
#include <asm/io.h>
#include <asm/hw_irq.h>
#include <linux/input.h>
//ioctl
#define WR_VALUE_X _IOW('a','a',int)
#define WR_VALUE_Y _IOW('a','b',int)
#define WR_VALUE_BTN _IOW('a','c',int)
//(open function mem alloc)
#define mem_size        1024 
int32_t value_io = 0;//ioctl
dev_t dev = 0;//major/minor nums
//dev_t dev = MKDEV(2,0);
//proc
char array[20]="try_proc_array\n";
static int len = 1;
//waiting queue
/*uint32_t read_count = 0;
static struct task_struct *wait_thread;
wait_queue_head_t wait_queue;
int wait_queue_flag = 0;//*/
//Sysfs
volatile int sys_value = 0;
struct kobject *kobj_ref;
//input
static struct input_dev *virtual_dev;

/*---------------------------------------------------------------------------------------Declarations------------------*/
//device file
static struct class *dev_class;
 static struct cdev cdev;
 uint8_t *kernel_buffer;
//module
static int __init my_init(void);
static void __exit my_exit(void);
//driver
static int open(struct inode *inode, struct file *file);
static int release(struct inode *inode, struct file *file);
static ssize_t read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t write(struct file *filp, const char *buf, size_t len, loff_t * off);
static long ioctl(struct file *file, unsigned int cmd, unsigned long arg);
static struct file_operations fops =
{
.owner          = THIS_MODULE,
.read           = read,
.write          = write,
.open           = open,
.unlocked_ioctl = ioctl,
.release        = release,
};
//Procfs
static int open_proc(struct inode *inode, struct file *file);
static int release_proc(struct inode *inode, struct file *file);
static ssize_t read_proc(struct file *filp, char __user *buffer, size_t length,loff_t * offset);
static ssize_t write_proc(struct file *filp, const char *buff, size_t len, loff_t * off);
static struct /*file_operations*/proc_ops fops_proc =
{
.proc_read           = read_proc,
.proc_write          = write_proc,
.proc_open           = open_proc,
.proc_release        = release_proc,
};
//Sysfs
static ssize_t sysfs_show(struct kobject *kobj,  struct kobj_attribute *attr, char *buf);
static ssize_t sysfs_store(struct kobject *kobj,  struct kobj_attribute *attr,const char *buf, size_t count);
 
struct kobj_attribute attr = __ATTR(sys_value, 0660, sysfs_show, sysfs_store);
//----------------------------------------------------------Wait Function
/*static int wait_function(void *unused)
{
        
        while(1) {
                printk(KERN_INFO "[VirtualJoystick]:Waiting For Event...\n");
                wait_event_interruptible(wait_queue, wait_queue_flag != 0 );
                if(wait_queue_flag == 2) {
                        printk(KERN_INFO "[VirtualJoystick]:Event Came From Exit Function\n");
                        return 0;
                }
                printk(KERN_INFO "[VirtualJoystick]:Event Came From Read Function - %d\n", ++read_count);
                wait_queue_flag = 0;
        }
        do_exit(0);
        return 0;
}//*/
 

//----------------------------------------------------------------------------------------------device file functions
static int open(struct inode *inode, struct file *file)
{
        /*Creating Physical memory*/
        if((kernel_buffer = kmalloc(mem_size , GFP_KERNEL)) == 0){
            printk(KERN_WARNING "[VirtualJoystick]:Cannot allocate memory in kernel\n");
            return -1;
        }
      //  kernel_buffer = "bonjour!";//*/
       // printk(KERN_INFO "[VirtualJoystick]:Device File Opened...!!!\n");
        return 0;
}
 
static int release(struct inode *inode, struct file *file)
{
        kfree(kernel_buffer);
       // printk(KERN_INFO "[VirtualJoystick]:Device File Closed...!!!\n");
        return 0;
}
 
static ssize_t read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{       
        //wait queue
        /*wait_queue_flag = 1;
        wake_up_interruptible(&wait_queue);//*/

        //data transfer
        copy_to_user(buf, kernel_buffer, mem_size);

        //(Interrupt request 11)
       /* struct irq_desc *desc;
        desc = irq_to_desc(11);
        if (!desc) return -EINVAL;
        __this_cpu_write(vector_irq[59], desc);
      asm("int $0x3B"); //*/
        printk(KERN_INFO "[VirtualJoystick]:Data Read : Done!\n"); // (value = %s),kernel_buffer);
        return 0;
}
static ssize_t write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
      /*  kernel_buffer = "";
        printk("[VirtualJoystick]:CPout=[%ld]",copy_from_user(kernel_buffer, buf, len));//*/
        printk(KERN_INFO "[VirtualJoystick]:Data Write : Done!\n"); //(value =%s),kernel_buffer);
        return len;
}
static long ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
         switch(cmd) {
                case WR_VALUE_X:
                        //copy_from_user(&value_io ,(int32_t*) arg, sizeof(value_io));
                        input_report_abs(virtual_dev, ABS_X, (int)arg);
                       // printk(KERN_INFO "[VirtualJoystick]: X value changed !!!!!!!!!!(arg = %ld)", arg);
                        break;
                case WR_VALUE_Y:
                        input_report_abs(virtual_dev, ABS_Z, (int)arg);
                        //printk(KERN_INFO "[VirtualJoystick]: Y value changed !!!!!!!!!!(arg = %ld)", arg);
                        break;
                case WR_VALUE_BTN:
                        input_report_key(virtual_dev, BTN_A, (int)arg);
                       // printk(KERN_INFO "[VirtualJoystick]: BTN 0 pressed !!!!!!!!!!(arg = %ld)", arg);
        }
        input_sync(virtual_dev);
        return 0;
}
//-------------------------------------------------------------------------------------Procfs functions
static int open_proc(struct inode *inode, struct file *file)
{
         
    printk(KERN_INFO "[VirtualJoystick]:proc file opend.....\t");
    return 0;
}
 
static int release_proc(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "[VirtualJoystick]:proc file released.....\n");
    return 0;
}
 
static ssize_t read_proc(struct file *filp, char __user *buffer, size_t length,loff_t * offset)
{
    if(len)
        len=0;
    else{
        len=1;
        return 0;
    }//*/
    copy_to_user(buffer,array,20);
     printk(KERN_INFO "[VirtualJoystick]:proc file read.....\n");

    return length;
}
 
static ssize_t write_proc(struct file *filp, const char *buff, size_t len, loff_t * off)
{
    printk(KERN_INFO "[VirtualJoystick]:proc file wrote.....\n");
        /*kfree(array);
        if((array = kmalloc(len , GFP_KERNEL)) == 0){
            printk(KERN_WARNING "[mymodulue]:Cannot allocate memory in kernel(proc)\n");
            return -1;
        }//*/
    copy_from_user(array,buff,20);
    return len;
}
//------------------------------------------------------------------------------------------------Sysfs Fuctions
static ssize_t sysfs_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
        printk(KERN_INFO "Sysfs - Read!!!\n");
        return sprintf(buf, "%d", sys_value);
}
 
static ssize_t sysfs_store(struct kobject *kobj,struct kobj_attribute *attr,const char *buf, size_t count)
{
        printk(KERN_INFO "Sysfs - Write!!!\n");
        sscanf(buf,"%d",&sys_value);
        return count;
}
//-----------------------------------------------------------------------------------------------Init function
static int __init my_init(void)
{
        int error;
		/*Allocating Major number Dynamically*/
        if(alloc_chrdev_region(&dev, 0, 1, "Virtual_Joystick") <0){//register_chrdev_region(dev, 1, "Dev_Name")
                printk(KERN_WARNING "[VirtualJoystick]:Cannot allocate major number for device 1\n");
                return -1;
        }
        printk(KERN_INFO "[VirtualJoystick]:Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));
                //-------------------------------------//
                /*Creating cdev structure*/
        cdev_init(&cdev,&fops);
 
        /*Adding character device to the system*/
        if((cdev_add(&cdev,dev,1)) < 0){
            printk(KERN_INFO "[VirtualJoystick]:Cannot add the device to the system\n");
            goto r_class;
        }
                /*Creating struct class*/
        if((dev_class = class_create(THIS_MODULE,"Virtual_Joystick_class")) == NULL){
            printk(KERN_WARNING "[VirtualJoystick]:Cannot create the struct class for device\n");
            goto r_class;
        }
 
                /*Creating device*/
        if((device_create(dev_class,NULL,dev,NULL,"js1")) == NULL){
            printk(KERN_WARNING "[VirtualJoystick]:Cannot create the Device\n");
            goto r_device;
        }
        /*Creating Proc entry*/
        proc_create("Virtual_Joystick_proc",0666,NULL,&fops_proc);
        
        //Initialize wait queue
        /*init_waitqueue_head(&wait_queue);
 
        //Create the kernel thread with name 'mythread'
        wait_thread = kthread_create(wait_function, NULL, "WaitThread");
        if (wait_thread) {
                printk("[mymodulue]:Thread Created successfully\n");
                wake_up_process(wait_thread);
        } else
                printk(KERN_INFO "[mymodulue]:Thread creation failed\n");//*/
                //-------------------------------------//
        /*Creating a directory in /sys/kernel/ */
        kobj_ref = kobject_create_and_add("Virtual_Joystick",kernel_kobj);
 
        /*Creating sysfs file for etx_value*/
        if(sysfs_create_file(kobj_ref,&attr.attr)){
                printk(KERN_INFO"[VirtualJoystick]:Cannot create sysfs file......\n");
                goto r_sysfs;
    }
    virtual_dev = input_allocate_device();
        if (!virtual_dev) {
                printk(KERN_ERR "[VirtualJoystick]: Not enough memory\n");
                error = -ENOMEM;
                goto r_sysfs;
        }
        virtual_dev->name = "Virtual Joystic";
virtual_dev->phys = "JoysticSimulModule/input0"; // "vmd" is the driver's name
virtual_dev->id.bustype = BUS_VIRTUAL;
virtual_dev->id.vendor  = 0x0000;
virtual_dev->id.product = 0x0000;
virtual_dev->id.version = 0x0000;
/*virtual_dev->evbit[0] = BIT_MASK(EV_KEY) | BIT_MASK(EV_REL);
virtual_dev->keybit[BIT_WORD(BTN_MOUSE)] = BIT_MASK(BTN_LEFT) | BIT_MASK(BTN_RIGHT) | BIT_MASK(BTN_MIDDLE);
virtual_dev->relbit[0] = BIT_MASK(REL_X) | BIT_MASK(REL_Y);
virtual_dev->keybit[BIT_WORD(BTN_MOUSE)] |= BIT_MASK(BTN_SIDE) | BIT_MASK(BTN_EXTRA);
virtual_dev->relbit[0] |= BIT_MASK(REL_WHEEL);//*/
         virtual_dev->evbit[0] = BIT_MASK(EV_ABS) | BIT_MASK(EV_KEY); 
        //virtual_dev->absbit[BIT_WORD(ABS_X)]= BIT_MASK(ABS_X);//|BIT_MASK(ABS_Z);
        //virtual_dev->absbit[BIT_WORD(ABS_X|ABS_Z)]= BIT_MASK(ABS_Z);//|BIT_MASK(ABS_Z);
        //virtual_dev->absbit[0] = BIT_MASK(ABS_X)|BIT_MASK(ABS_Z);
        virtual_dev->keybit[BIT_WORD(BTN_GAMEPAD)] = BIT_MASK(BTN_A);

        input_set_abs_params(virtual_dev, ABS_X, -255, 255, 0, 0);
        input_set_abs_params(virtual_dev, ABS_Z, -255, 255, 0, 0);
        //input_set_abs_params(virtual_dev, ABS_Z, 0, 255, 0, 0);
        /* printk(KERN_INFO "[VirtualJoystick]: BIT_MASK(EV_ABS) = %ld",BIT_MASK(EV_ABS));
         printk(KERN_INFO "[VirtualJoystick]: BIT_WORD(ABS_X) = %d",BIT_WORD(ABS_X));
         printk(KERN_INFO "[VirtualJoystick]: BIT_WORD(ABS_Z) = %d",BIT_WORD(ABS_Z));
         printk(KERN_INFO "[VirtualJoystick]: BIT_WORD(ABS_Z) = %d",BIT_WORD(ABS_Z));
         printk(KERN_INFO "[VirtualJoystick]: BIT_MASK(ABS_Z)|BIT_MASK(ABS_Z)|BIT_MASK(ABS_X) = %ld",BIT_MASK(ABS_Z)|BIT_MASK(ABS_Z)|BIT_MASK(ABS_X));
         printk(KERN_INFO "[VirtualJoystick]: BIT_MASK(ABS_Z) = %ld",BIT_MASK(ABS_Z));
         printk(KERN_INFO "[VirtualJoystick]: BIT_MASK(ABS_X|ABS_Z) = %ld",BIT_MASK(ABS_X|ABS_Z));
         printk(KERN_INFO "[VirtualJoystick]: BIT_MASK(ABS_Z|ABS_X) = %ld",BIT_MASK(ABS_Z|ABS_X));
         printk(KERN_INFO "[VirtualJoystick]: BIT_MASK(ABS_X|ABS_Z) = %ld",BIT_MASK(ABS_X|ABS_Z));
         printk(KERN_INFO "[VirtualJoystick]: BIT_MASK(ABS_Z|ABS_X) = %ld",BIT_MASK(ABS_Z|ABS_X));
         printk(KERN_INFO "[VirtualJoystick]: BIT_MASK(ABS_Z|ABS_Z) = %ld",BIT_MASK(ABS_Z|ABS_Z));
         printk(KERN_INFO "[VirtualJoystick]: BIT_MASK(ABS_Z|ABS_Z) = %ld",BIT_MASK(ABS_Z|ABS_Z));
         printk(KERN_INFO "[VirtualJoystick]: BIT_MASK(ABS_X|ABS_Z|ABS_Z) = %ld",BIT_MASK(ABS_X|ABS_Z|ABS_Z));
         printk(KERN_INFO "[VirtualJoystick]: BIT_MASK(ABS_X|ABS_Z|ABS_Z) = %ld",BIT_MASK(ABS_X|ABS_Z|ABS_Z));//*/

        error = input_register_device(virtual_dev);
        if (error) {
                printk(KERN_ERR "[VirtualJoystick]: Failed to register device\n");
                goto dev;
        }
    //////-----------------------------------
        printk(KERN_INFO "[VirtualJoystick]:Kernel Module Inserted Successfully...\n");

        return 0;

dev:
        //input_unregister_device(virtual_dev);
        input_free_device(virtual_dev);
r_sysfs:
        kobject_put(kobj_ref); 
        sysfs_remove_file(kernel_kobj, &attr.attr);
r_device:
        class_destroy(dev_class);
r_class:
        unregister_chrdev_region(dev,1);
        cdev_del(&cdev);
        return -1;

}
//----------------------------------------------------------------------------------------------------------Exit function
void __exit my_exit(void)
{
        input_unregister_device(virtual_dev);
        input_free_device(virtual_dev);
        kobject_put(kobj_ref); 
        sysfs_remove_file(kernel_kobj, &attr.attr);
        /*wait_queue_flag = 2;
        wake_up_interruptible(&wait_queue);//*/
        remove_proc_entry("Virtual_Joystick_proc",NULL);
        device_destroy(dev_class,dev);
        class_destroy(dev_class);
         cdev_del(&cdev);
        unregister_chrdev_region(dev, 1);
    printk(KERN_INFO "[VirtualJoystick]: Kernel Module Removed Successfully...\n");
}
//--------------------------------------------------------------------------------Module data
module_init(my_init);
module_exit(my_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Armand <armand.perrin.emeraud@gmail.com>");
MODULE_DESCRIPTION("Desc : a virtual joystic");
MODULE_VERSION("0:1.0-v0");
/*modProg:
	rmmod JoysticSimulModule.ko
	make -C $(KDIR) M=$(shell pwd) modules
	insmod JoysticSimulModule.ko
	gcc -o JoysticSimulUserProg JoysticSimulUserProg.c
	./JoysticSimulUserProg//*/