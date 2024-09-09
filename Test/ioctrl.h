/**************************************************************
* Class:  CSC-415-01 Spring 2023
* Name:Esau Bojorquez Medina
* GitHub UserID: Esau 4119
* Project: Assignment 6 – Device Driver
*
* File: ioctrl.h
*
* Description:Io control header
*
**************************************************************/
#ifndef IOCTRL_H
#define IOCTRL_H
// structure for salute
struct mystruct{
    int repeat;
    char name[64];
};
//initializing our function
# define WR_VALUE _IOW('a','a', int32_t *)
# define RD_VALUE _IOR('a','b', int32_t *)
# define SALUTE  _IOW('a','c', struct mystruct  *)

#endif 