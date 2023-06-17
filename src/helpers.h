/**
 * @file helpers.h
 *
 * @brief This file defines the functions in helpers
 *
 * @author Warshall
 * Name             github
 * Gulam Nabi       edsml-gmn22
 * Yihang Zhou      acse-yz3522
 * Xingjian Qi      acse-xq322
 * Luisina Canto    edsml-lbc19
 * Hongyuan Liu     edsml-hl1222
 * Kexin Yan        acse-ky322
 * @version 0.1
 * @date 2023-03-23
 * @copyright Copyright (c) 2023
 */
#ifndef HELPERS_H
#define HELPERS_H
#include "Image.h"
#include "Volume.h"
/**
 * given the x coodinate and the
 * @param x position of interested
 * @param sig standard deviation of guassian function
 * @return the gaussian value of the given position with given standard deviation
 */
double gaussian_function(double x, double sig);
/**
 * given the start pointer and end pointer, sort the items between start and end inplace.
 * @param start_ptr start position of the array
 * @param end_ptr end position of the array
 */
void my_sort(unsigned char *start_ptr, unsigned char *end_ptr);
/**
 * given the start pointer and length, return the average of the array from start to
 * start + lenght
 * @param start_ptr start position of the array
 * @param n length of the arry
 * @return the average value
 */
double average_func(unsigned char *start_ptr, int n);
/**
 * given the start pointer and length, return the median of the array from start to
 * start + lenght
 * @param start_ptr start position of the array
 * @param n length of the arry
 * @return the median value
 */
double median_func(unsigned char *start_ptr, int n);
/**
 * given the start pointer and length, return the max of the array from start to
 * start + lenght
 * @param start_ptr start position of the array
 * @param n length of the arry
 * @return the max value
 */
double max_func(unsigned char *start_ptr, int n);
/**
 * given the start pointer and length, return the min of the array from start to
 * start + lenght
 * @param start_ptr start position of the array
 * @param n length of the arry
 * @return the min value
 */
double min_func(unsigned char *start_ptr, int n);
/**
 * given the start pointer and left position and right position,
 * return the sorted array by using merge sort
 * @param start_ptr start position of the array
 * @param left the left index of the array
 * @param right the right index of the array
 */
void mergesort(unsigned char *start_ptr, int left, int right);
/**
 * read in an integer that is not greater than m
 * @param m upper bound of the input value
 * @return the inputed integer
 */  
int get_kernel_size(int m);
/**
 * read in the padding size.
 * @param ifpad true if there's padding, false if there's not padding
 * @param pad padding size if there's padding
 */  
void get_padding_size(char &ifpad, int &pad);
/**
 * read in the standard deviation
 * @return inputed standard deviation 
 */  
double get_sig();
/**
 * read in an integer between upper bound and lower bound and print the message
 * @param ub upper bound of the integer
 * @param lb lower bound of the integer
 * @param msg message to output on screen
 * @return inputed integer
 */  
int get_int(int ub, int lb, const char * msg);
/**
 * wrapper for reading the starting(ending) index for the thin slab. 
 * this function is for the user interface
 * @param s message to output on screen
 * @param start_idx lower bound for the integer
 * @param end_idx upper bound for the integer
 * @param arr array where the index indicating whether the input is valid
 * @param minval minimum accepted value
 * @return inputed integer
 */  
int get_slab(const char *s, int start_idx, int end_idx, int *arr, int minval);
/**
 * wrapper for saving the image
 * this function is for user interface
 * @param img the image to save
 * @return 0 if save success -1 if it fails
 */  
int save_img_warpper(const Image *img);
/**
 * wrapper for applying the given projcetion 
 * this function is for the user interface
 * @param msg message to output on screen
 * @param start_idx start index of the projection position
 * @param end_idx end index of the projection position
 * @param simplemap array where the ith value indicate the position of the k-start_idx image where k is the index in the name of the image
 * @param vol volume projection should act on
 * @param func projection function applied(max, min, median, mean)
 */  
void apply_projection(char const *msg, int start_idx, int end_idx, int *simplemap, Volume *vol, double (*func)(unsigned char *, int));
/**
 * wrapper for applying slice function 
 * this function is for the user interface
 * @param dir slicing direction
 * @param ub slicing position upper bound
 * @param vol volume projection should act on
 */  
void apply_slice(char dir, Volume *vol, int ub);
/**
 * return the kth smallest element using quick select
 * @param arr array of interest
 * @param l left index of interest area
 * @param r right index of interest area
 * @param k target ranking
 */  
unsigned char kthSmallest(unsigned char *arr, int l, int r, int k);
#endif