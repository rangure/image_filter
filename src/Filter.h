/**
 * @file Filter.h
 *
 * @brief This file specify the interface and functionalities of Filter class
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
#pragma once
#include "Image.h"
#include "Volume.h"
class Filter
{
private:
    /* data */
    /**
     * Converts RGB pixel to HSV colour space.
     * @param r red pixel value
     * @param g green pixel value
     * @param b blue pixel value
     * @param h hue pixel value
     * @param s saturation pixel value
     * @param v value pixel value
     */
    static void rgb_to_hsv(float &r, float &g, float &b, float &h, float &s, float &v);
    /**
     * Converts HSV space pixel to RGB pixels.
     * @param h hue pixel value
     * @param s saturation pixel value
     * @param v value pixel value
     * @param r red pixel value
     * @param g green pixel value
     * @param b blue pixel value
     */
    static void hsv_to_rgb(float &h, float &s, float &v, float &r, float &g, float &b);

public:
    Filter(/* args */);
    /**
     * Filters the given image to convert it to a greyscale (checking how many channel image has).
     * @param img Image pointer
     */
    static void filter_grayscale(Image *img);
    /**
     *  Filter that applies automatic colour balance taking the (weighted) average of all channels.
     * @param img Image pointer
     */
    static void filter_ACB(Image *img);
    /**
     * Filters the brightness adjustment by calculating the brightness value automatically from the image
     * @param img Image pointer
     */
    static void filter_brightness(Image *img);
    /**
     *  Filters the brightness of an image object by applying a specific brightness value (bv) to
     * each pixels intensity.
     * @param img Image pointer
     * @param bv  Brightness value
    */
    static void filter_brightness(Image *img, int bv);
    /**
     * Histogram equalisation filter to streatching out the intensity range of the image 
     * @param img Image pointer
    */
    static void filter_histogram_equalization(Image *);
    /**
     * Filter to automatically enhance the contrast of the RGB image
     * @param img Image pointer
    */
    static void filter_contrast_enhancement(Image *);
    /**
     * Filter to convert RGB image to an HSV colour scale
     * @param img Image pointer
    */
    static void filter_hsv(Image*);
    /**
     * Filter applies median blur filtering to an image, replacing each pixel value as the median
     * pixel value of a specified neighbourhood (defined by the kernel size input).
     * @param img Image pointer,
     * @param ksize Kernel size,
     * @param psize Padding size
    */
    static void filter_median_blur(Image *img, int ksize, int psize);
    /**
     * Filter applies a box blur to the image by replacing the pixel value with the average value
     * of a specified neighbourhood (defined by the kernel size input).
     * @param img Image pointer,
     * @param ksize Kernel size,
     * @param psize Padding size
     */
    static void filter_box_blur(Image *img, int ksize, int psize);
    /**
     * Filter applies a gaussian blur
     * @param img Image pointer,
     * @param ksize Kernel size,
     * @param psize Padding size
     * @param sig standard deviation
    */
    static void filter_gaussian_blur(Image *img, int kszie, int psize, double sig);
    /**
     * @brief Filter applies the Sobel operator on the image, which is used for edge detection by
    convolving the image with 2 set 3x3 kernels by calling the function apply_kernel_det to
    calculate the determinant.
     * 
     * @param Image pointer 
     */
    static void filter_sobel(Image *img);

    /**
     * @brief Filter implements Prewitt edge detection filter for the image that uses a set 3x3 kernel
to compute the gradient of the image intensity and applies to the the image using
the function apply_kernel_det.
     * 
     * @param img pointer
     */
    static void filter_prewitt(Image *img);

    /**
     * @brief Filter implements a Scharr edge detection for the image. It applies a 3x3 matrix
    (based on the Scharr coefficients) convolution on the image using a double precision buffer .
     * 
     * @param img pointer
     */
    static void filter_scharr(Image *img);

    /**
     * @brief Filter implements Roberts cross edge detection for the image. It uses two 3x3 matrix
    (one for the x direction and another for y) and applies them on the image.
     * 
     * @param img pointer
     */
    static void filter_robertscross(Image *img);

    /**
     * @brief Function computes and returns a gaussian kernel(2D matrix of values which is used to weigh)
   neighbour pixels
     * 
     * @param kernel size
     * @param standard deviation of gaussian curve
     */  
    static double *gaussian_kernel(int ksize, double sig);
    
    /**
     * @brief Generates a 1D gaussian kernel of input kernel size with input standard deviation.
   It returns this normalized kernel as a pointer.
     * 
     * @param ksize 
     * @param sig 
     * @return double* standard deviation
     */
    static double *gaussian_vec(int ksize, double sig);

    /**
     * @brief Function that convolves image with given kernels in the x and y direction separately. It
    combines the gradients to find the magnitude of the gradient vector, in which the maximum is
    stored in a temporary buffer which is normalized and applied to the pixel values.
     * 
     * @param img 
     * @param kx 
     * @param ky 
     * @param l length of one side of square kernel
     */
    static void apply_kernel_det(Image *img, int *kx, int *ky, int l);

    /**
     * @brief Function applies a 3D gaussian filter to the 3D volume of the image.
     * 
     * @param vol Image volume-input from user
     * @param ksize gaussian kernel size
     * @param psize padding size
     * @param sig standard deviation of gaussian
     * @param which which version to run, 0 for fast version, 1 for small memory version
     */
    static void gaussian_3d(Volume *vol, int ksize, int psize, double sig, bool which);
    
    /**
     * @brief Function applies a 3D median filter to the 3D volume of the image.
     * 
     * @param Image volume-input from user 
     * @param median kernel size 
     * @param padding size 
     */
    static void median_3d(Volume *vol, int ksize, int psize);
    ~Filter();
};
