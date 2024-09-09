/**************************************************************
* Class:  CSC-415-01 Spring 2023
* Name:Esau Bojorquez Medina
* GitHub UserID: Esau 4119
* Project: Assignment 6 – Device Driver
*
* File: bojorquezMedina_Esau_HW6_main.c
*
* Description:The assignment is to write a Linux Device Driver
*             with io control.
*
**************************************************************/

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>
#include "ioctrl.h"

int main(){
    // Starter string for user input
    char str[225];
    // My struct in ioctrl ready for the saluter
    struct mystruct te= {4,"EZA"};
    // Opening up my driver. 
    int dev= open("/dev/Driver",O_WRONLY);
    // 
    if (dev == -1){
        printf("ERROR WITH OPENING\n");
        return -1;
    }
    
  //Needed too loop our read and write. 
    while(1){

        //reading what our driver has stored
        ioctl(dev,RD_VALUE,&str);
        printf("Read From Dev: %s\n",str);
        //User Inupt
        printf("Enter a string:\t");
        scanf( "%s[^\n]", str);

        //EXIT CODE 
        if( strcmp(str,"exit")==0){
            break;
        }
        // writing user input to driver
        ioctl(dev,WR_VALUE,&str);

        //grabing infro from driver
        ioctl(dev,RD_VALUE,&str);
        printf("Your entered String is:\t%s\n",str);
        
        //SALUTER
        ioctl(dev,SALUTE,&te);
        //easy to separate operations when running. 
        printf("----OPENING SUCCESS----\n");
    }
    
    close(dev);
    return 0;
}   