/**************************************************************
* Class:  CSC-415-01 Spring 2023
* Name:Esau Bojorquez Medina
* GitHub UserID: Esau 4119
* Project: Assignment 6 – Device Driver
*
* File: helloworld.c
*
* Description:The assignment is to write a Linux Device Driver
*             with io control.
*
**************************************************************/
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>
#include "../Test/ioctrl.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("EsauBM");
// Global data

//Device number for Major
static dev_t myDevNum;

// class for saluter
static struct class *myClass;

//Auto Create file
static struct cdev myDevice;

//Initializing our driver info
#define DRIVER_NAME  "Driver"
#define DRIVER_Class "ModuleClass"


//Buffer Info
static char buffer[255];
static int buffPointer; 

static ssize_t device_Read (struct file *File, char *userBuff, size_t count, loff_t *offs){

    int to_copy, not_copied, delta;

    //amount of data to copy
    to_copy = min(count, buffPointer);

    //Copying data to the user
    not_copied = copy_to_user(userBuff, buffer, to_copy);
    delta = to_copy - not_copied;
    return delta;
};


static ssize_t device_Write (struct file *File, const char *userBuff, size_t count, loff_t *offs){

    int to_copy, not_copied, delta;
    //amount of data to copy
    to_copy = min(count, sizeof(buffer));

    //Copying data to the user
    not_copied = copy_from_user(buffer, userBuff, to_copy);
    buffPointer = to_copy;
    delta = to_copy - not_copied;
    return delta;
};


//Variable for reading and writing
unsigned char * startString = '1';
static long int my_ioctl(struct file *file, unsigned cmd, unsigned long arg){
    struct mystruct drStruct;
    //printk("hi");
    switch(cmd){
        case WR_VALUE:
            
            // coping the string coming from user 
            if(copy_from_user(&startString,(unsigned char *) arg, sizeof(startString))){
                // if we fail 
                printk("IOCTL: ERROR COPYING DATA FROM\n");
            
            }else{
                //if we pass
                printk("IOCTL: Update answer to %c\n", startString);
            }
            break;
        case RD_VALUE:
            // device_Read(file,(unsigned char *)buffer,sizeof(file));
            if(copy_to_user((unsigned char *) arg,&startString, sizeof(startString))){
                // if we fail 
                printk("IOCTL: ERROR COPYING DATA TO USER\n");
            }else{
                //if we pass
                printk("IOCTL: Input WAS COPIED\n");
            }
            break;
        case SALUTE:
            if(copy_from_user(&drStruct,(struct mystruct *) arg, sizeof(drStruct))){
                //if we fail
                printk("IOCTL: ERROR COPYING DATA FROM\n");
            }else{
                //if we pass
                printk("IOCTL: %d greets to %s\n", drStruct.repeat,drStruct.name);
            }
            break;
    }
    return 0;
}


// Indicate our Device has been open.
static int device_open(struct inode *device_file, struct file *inst) {
	printk("devNum: Open was called!\n");
	return 0;
}

//Called to before destroying evidence. 
static int device_close(struct inode *device_file, struct file *inst) {
	printk("devNum: Close was called!\n");
	return 0;
}

// initializing our file operation struct
static struct file_operations fops = {
	.open = device_open,
	.release = device_close,
    .read = device_Read,
    .write = device_Write,
    .unlocked_ioctl = my_ioctl
};



static int __init ModuleInit(void) {
	int retVal;
	printk("KERNAL ENTRY\n");

    //Allocate Device num
    if(alloc_chrdev_region(&myDevNum, 0 , 1, DRIVER_NAME) < 0){
        printk("Device could not be allocated");
        return -1; 
    }
    printk("RW: devNum: registered Device number Major: %d, Minor: %d\n", myDevNum>>20, myDevNum&0xfffff);
    
    //Device Class
    if((myClass = class_create(THIS_MODULE,DRIVER_Class)) ==NULL){
        printk("Device Class could not be allocated");
        goto ClassError;
    }
    // Creating Device file
    if(device_create(myClass,NULL, myDevNum, NULL,DRIVER_NAME) == NULL){
        printk("Device File could not be allocated");
        goto FileError; 
    }


    // Initializing Device file
    cdev_init(&myDevice, &fops);

    //Registering to kernel
    if(cdev_add(&myDevice,myDevNum,1) == -1){
        printk("Registering of device to kernel Failed");
        goto AddError;
    }

    return 0;

    // classes to find out what errors we have
    // as well as clean up. 
    AddError:
        device_destroy(myClass,myDevNum);
    FileError:
        class_destroy(myClass);
    ClassError:
        unregister_chrdev(myDevNum, DRIVER_NAME);
        return -1;

};

static void __exit ModuleExit(void) {
    // remove device class and unregister our device. 
    cdev_del(&myDevice);
    device_destroy(myClass,myDevNum);
    class_destroy(myClass);
    unregister_chrdev(myDevNum, DRIVER_NAME);
	printk("Goodbye, Kernel\n");
};

module_init(ModuleInit);
module_exit(ModuleExit);



 