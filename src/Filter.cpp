
/**
 * @file Filter.cpp
 *
 * @brief This file implementes the functionalites of Filter class
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

#include "Filter.h"
#include <iostream>
#include <vector>
#include "helpers.h"
#include <math.h>
using namespace std;

/*Filter class: containing the following filters
    - filter_greyscale
    -filter_brightness
    -filter_ACB
    -filter_median_blur
    -filter_box_blur
    filter_gaussian_blur
    -gaussian_kernel
    -gaussian_vec
    -filter_prewitt
    -filter_robertscross
    -apply_kernel_det
    -gaussian_3d
    -median_3d
*/
Filter::Filter(/* args */)
{
}

Filter::~Filter()
{
}

void Filter::filter_grayscale(Image *img)
{
    int channel = img->get_channel();
    int width = img->get_width();
    int height = img->get_height();
    const unsigned char *data = img->get_data();
    // Check if image already has 2 or less colour channels (if so dont need to apply the filter.)
    if (channel <= 2)
    {
        return;
    }
    else
    {
        // Change to grayscale by taking and average of the RGB channels to make it 1 channel
        int channel_new = channel - 2;
        // create new container for grayscale image
        unsigned char *newimg = new unsigned char[width * height * channel_new];
        double avg;
        int idx;
        int cur_idx;
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                // for each pixel, take the average of of the RGB channels with the formula below
                idx = img->pixel_index(i, j, 0);
                // Average = 0.299 * R + 0.587 * G + 0.114 * B
                avg = 0.299 * data[idx] + 0.587 * data[idx + 1] + 0.114 * data[idx + 2];
                cur_idx = i * width * channel_new + j * channel_new;
                newimg[cur_idx] = avg;
                // has channels representing transparency
                if (channel_new == 2)
                {
                    // copies original image to the second channel of the new image
                    newimg[cur_idx + 1] = data[idx + 3];
                }
            }
        }
        // update image
        img->replace_data(newimg);
        img->set_channel(channel_new);
    }
}
// Filter applies specified brightness value (bv) to each pixels intensity
void Filter::filter_brightness(Image *img, int bv)
{
    // get image data
    int channel = img->get_channel();
    int width = img->get_width();
    int height = img->get_height();
    const unsigned char *data = img->get_data();
    int idx;
    // loop through pixels (and appropriate channels)
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            for (int k = 0; k < (channel - 1) / 2 * 2 + 1; k++)
            {
                // adds the bv to each pixels intensity value
                idx = img->pixel_index(i, j, k);
                img->set_data_idx(idx, max(min(data[idx] + bv, 255), 0));
            }
        }
    }
}

void Filter::filter_brightness(Image *img)
{
    // get image data
    int sum_value = 0;
    int channel = img->get_channel();
    int width = img->get_width();
    int height = img->get_height();
    const unsigned char *data = img->get_data();
    // loop through relevant channels - 1 for grey, 3 for RGB
    for (int k = 0; k < (channel - 1) / 2 * 2 + 1; k++)
    {
        // add average of to sum
        sum_value += img->average_by_channel(0, width, 0, height, k);
    }
    // calculate scaling factor - target average pix value / actual average pixel value
    double rate = 128.0 / (sum_value / ((channel - 1) / 2 * 2 + 1));
    int idx;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            for (int k = 0; k < (channel - 1) / 2 * 2 + 1; k++)
            {
                // set brightness adjustment
                idx = img->pixel_index(i, j, k);
                img->set_data_idx(idx, max(min((int)(data[idx] * rate + 0.5), 255), 0));
                // value is clamped to [0,255] and stored in image
            }
        }
    }
}
// Function applies automatic colour balance
void Filter::filter_ACB(Image *img)
{
    // get all values from Image
    int channel = img->get_channel();
    int width = img->get_width();
    int height = img->get_height();
    const unsigned char *data = img->get_data();
    // array to hold the sum of pixel value for each channel
    double sum_value[] = {0, 0, 0, 0};

    double minval = 256;
    // compute sum of each colour channel
    for (int k = 0; k < (channel - 1) / 2 * 2 + 1; k++)
    {
        // find average value foe current channel- avg = avg / ((channel - 1) / 2 * 2 + 1)
        sum_value[k] += img->average_by_channel(0, width, 0, height, k);
        // avg += sum_value[k];
        // update value across all channels
        if (minval > sum_value[k])
            minval = sum_value[k];
    }
    // avg = avg / ((channel - 1) / 2 * 2 + 1);
    // compute scaling factor for each channel - min pix value / avg pix value for each channel
    // container to hold colour balance rate
    double rate[] = {0, 0, 0, 0};
    for (int k = 0; k < (channel - 1) / 2 * 2 + 1; k++)
    {
        // cout << sum_value[k] << endl;
        rate[k] = minval / sum_value[k];
    }
    // Apply scaling factor
    int idx;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            for (int k = 0; k < (channel - 1) / 2 * 2 + 1; k++)
            {
                // index of current pixel (to set data)
                idx = img->pixel_index(i, j, k);
                img->set_data_idx(idx, max(min((int)(data[idx] * rate[k] + 0.5), 255), 0));
            }
        }
    }
    cout << "average value after ACB" << endl;
    for (int k = 0; k < (channel - 1) / 2 * 2 + 1; k++)
    {
        // calculates average pixel value for each channel
        cout << img->average_by_channel(0, width, 0, height, k) << endl;
    }
}

void Filter::filter_histogram_equalization(Image *img)
{

    int channel = img->get_channel();
    int width = img->get_width();
    int height = img->get_height();
    // Creating a copy of pointer to image data(to reduce function call)
    const unsigned char *data = img->get_data();
    // Checking if the image is greyscale/single color.
    if (channel <= 2)
    {
        std::vector<int> intensity_dist(256, 0);

        for (int j = 0; j < width * height * channel; ++j)
        {
            uint8_t pixel = static_cast<uint8_t>(data[j]);
            // Pixel value distribution
            intensity_dist[pixel]++;
        }
        // Cummulative distribution function
        std::vector<int> cdf(256, 0);
        int sum = 0;
        for (int i = 0; i < 256; ++i)
        {
            sum += intensity_dist[i];
            cdf[i] = sum;
        }
        // Normalize CDF to [0, 255]
        int min_cdf = cdf[0];
        int max_cdf = cdf[255];

        for (int i = 0; i < 256; ++i)
        {
            cdf[i] = static_cast<int>((cdf[i] - min_cdf) * 255.0 / (max_cdf - min_cdf));
        }

        unsigned char *equalised = new unsigned char[width * height * channel];
        // Mapping pixel intensities to new values using CDF
        for (int i = 0; i < width * height * channel; ++i)
        {
            uint8_t pixel = data[i];
            equalised[i] = static_cast<unsigned char>(cdf[pixel]);
        }
        img->replace_data(equalised);
    }
    else
    {
        std::cout << "This implementation is for grayscale only. Use contrast enhancement or convert this image into grey scale first" << std::endl;
    }
}

void Filter::filter_median_blur(Image *img, int kernel, int pad)
{
    // input image data
    int channel = img->get_channel();
    int width = img->get_width();
    int height = img->get_height();
    const unsigned char *data = img->get_data();

    // calculate index for padding and kernel start and stop positions
    int loopx_start, loopx_end, loopy_start, loopy_end;
    loopx_start = kernel / 2 - pad;
    loopx_end = width + pad - kernel / 2;
    loopy_start = kernel / 2 - pad;
    loopy_end = height + pad - kernel / 2;

    // dynamic array to hold pixel value in the kernel neighbourhood
    unsigned char *myarray = new unsigned char[kernel * kernel];
    // dynamic array to hold filtered image data
    unsigned char *temp_img = new unsigned char[width * height * channel];
    int base_x, base_y;
    int idx, val, center_idx;
    double median;
    int validval;
    // loop iterating though each channel of the image for kernel neighbourhood
    for (int i = loopy_start; i < loopy_end; i++)
    {
        for (int j = loopx_start; j < loopx_end; j++)
        {
            for (int c = 0; c < (channel - 1) / 2 * 2 + 1; c++)
            {
                // gets the edge values of the kernel (index)
                base_y = i - kernel / 2;
                base_x = j - kernel / 2;
                // keeps track of the number of valid pixel values (within kernel boundary)
                validval = 0;
                // fill in temp array with kernel neighbourhood pixel values
                for (int k1 = 0; k1 < kernel; k1++)
                {
                    for (int k2 = 0; k2 < kernel; k2++)
                    {
                        idx = img->pixel_index(base_y + k1, base_x + k2, c);
                        val = 0;
                        if (idx != -1)
                        {
                            validval++;
                            val = data[idx];
                        }
                        myarray[k1 * kernel + k2] = val;
                    }
                }
                // sort the temp array + calulate the median and set it in the temp_img
                center_idx = img->pixel_index(i, j, c);
                my_sort(myarray, myarray + kernel * kernel);
                median = (myarray[(kernel * kernel * 2 - validval) / 2]);
                temp_img[center_idx] = (int)(median + 0.5);
            }
        }
    }
    // set the median values calculated above to the image
    for (int i = loopy_start; i < loopy_end; i++)
    {
        for (int j = loopx_start; j < loopx_end; j++)
        {
            for (int c = 0; c < (channel - 1) / 2 * 2 + 1; c++)
            {
                idx = img->pixel_index(i, j, c);
                img->set_data_idx(idx, temp_img[idx]);
            }
        }
    }
    // clear memory
    delete[] myarray;
    delete[] temp_img;
}
void Filter::filter_box_blur(Image *img, int kernel, int pad)
{
    // get image data
    int channel = img->get_channel();
    int width = img->get_width();
    int height = img->get_height();
    const unsigned char *data = img->get_data();

    // calculate loop start and end point based on kernel and padding
    int loopx_start, loopx_end, loopy_start, loopy_end;
    loopx_start = kernel / 2 - pad;
    loopx_end = width + pad - kernel / 2;
    loopy_start = kernel / 2 - pad;
    loopy_end = height + pad - kernel / 2;

    // create temporary buffer to store the filtered image data
    unsigned char *temp_img = new unsigned char[width * height * channel];
    int base_x, base_y;
    double avg;
    int center_idx, idx;

    // loop through each pixel in the image and apply the filter
    for (int i = loopy_start; i < loopy_end; i++)
    {
        for (int j = loopx_start; j < loopx_end; j++)
        {
            for (int c = 0; c < (channel - 1) / 2 * 2 + 1; c++)
            {
                // calculate the base point index for the current kernel
                base_y = i - kernel / 2;
                base_x = j - kernel / 2;
                // get the average pixel value for current kernel neighbourhood
                avg = img->average_by_channel(base_x, base_x + kernel, base_y, base_y + kernel, c);
                // calculate index
                center_idx = img->pixel_index(i, j, c);
                // store and update temporary buffer with current average value
                temp_img[center_idx] = (int)(avg + 0.5);
            }
        }
    }
    // Translate data from the temporary buffer to the image array
    for (int i = loopy_start; i < loopy_end; i++)
    {
        for (int j = loopx_start; j < loopx_end; j++)
        {
            for (int c = 0; c < (channel - 1) / 2 * 2 + 1; c++)
            {
                // calculate the index for the current pixel in the image
                idx = img->pixel_index(i, j, c);
                img->set_data_idx(idx, temp_img[idx]);
            }
        }
    }
    // clear memory
    delete[] temp_img;
}
void Filter::filter_gaussian_blur(Image *img, int kernel, int pad, double sig)
{
    // load in data
    int channel = img->get_channel();
    int width = img->get_width();
    int height = img->get_height();
    const unsigned char *data = img->get_data();
    // Calculate the gaussian kernel using function in class
    double *gkern = Filter::gaussian_kernel(kernel, sig);
    // Set kernel boundaries
    int loopx_start, loopx_end, loopy_start, loopy_end;
    loopx_start = kernel / 2 - pad;
    loopx_end = width + pad - kernel / 2;
    loopy_start = kernel / 2 - pad;
    loopy_end = height + pad - kernel / 2;
    // create a temporary buffer to store filtered image
    unsigned char *temp_img = new unsigned char[width * height * channel];
    int base_x, base_y;
    double total_val;
    int idx, val, center_idx;
    // Apply the filter to each pixel of the image
    for (int i = loopy_start; i < loopy_end; i++)
    {
        for (int j = loopx_start; j < loopx_end; j++)
        {
            for (int c = 0; c < (channel - 1) / 2 * 2 + 1; c++)
            {
                // detemrine the neighbourhood location to apply the filter on
                base_y = i - kernel / 2;
                base_x = j - kernel / 2;
                total_val = 0;
                // apply the filter to each pixel in the neighbourhood
                for (int k1 = 0; k1 < kernel; k1++)
                {
                    for (int k2 = 0; k2 < kernel; k2++)
                    {
                        idx = img->pixel_index(base_y + k1, base_x + k2, c);
                        val = 0;
                        if (idx != -1)
                        {
                            val = data[idx];
                        }
                        total_val += val * gkern[k1 * kernel + k2];
                    }
                }
                // update the temporary image
                center_idx = img->pixel_index(i, j, c);
                temp_img[center_idx] = (int)(total_val + 0.5);
            }
        }
    }
    // update the real image (with temp filtered image)
    for (int i = loopy_start; i < loopy_end; i++)
    {
        for (int j = loopx_start; j < loopx_end; j++)
        {
            for (int c = 0; c < (channel - 1) / 2 * 2 + 1; c++)
            {
                idx = img->pixel_index(i, j, c);
                img->set_data_idx(idx, temp_img[idx]);
            }
        }
    }
    // clear memory
    // delete[] temp_img;
    delete[] gkern;
}
/*Function computes and returns a gaussian kernel(2D matrix of values which is used to weigh)
  neighbour pixels */
double *Filter::gaussian_kernel(int kernel, double sig)
{
    // create array to store gaussian kernel
    double *gkern = new double[kernel * kernel];
    // temporary vector used to store values for the gaussian kernel
    double *temp_vec = new double[kernel];
    double sum_val = 0;
    // loop over row
    for (int i = 0; i < kernel; i++)
    {
        // set value to distance between current row and center or kernel
        // sig = standard deviation = width of curve
        temp_vec[i] = gaussian_function(i - kernel / 2, sig);
        sum_val += temp_vec[i];
    }
    // find value of gaussian kernel- scaling by the square of sum of gkern
    sum_val = sum_val * sum_val;
    for (int i = 0; i < kernel; i++)
    {
        for (int j = 0; j < kernel; j++)
        {
            gkern[i * kernel + j] = temp_vec[i] * temp_vec[j] / sum_val;
        }
    }
    delete[] temp_vec;
    return gkern;
}
double *Filter::gaussian_vec(int kernel, double sig)
{
    double *temp_vec = new double[kernel];
    double sum_val = 0;
    for (int i = 0; i < kernel; i++)
    {
        temp_vec[i] = gaussian_function(i - kernel / 2, sig);
        sum_val += temp_vec[i];
    }
    for (int i = 0; i < kernel; i++)
    {
        temp_vec[i] = temp_vec[i] / sum_val;
    }
    return temp_vec;
}
void Filter::filter_sobel(Image *img)
{
    // define set 3x3 sobel kernel
    int *kernel = new int[9]();
    kernel[0] = -1;
    kernel[2] = 1;
    kernel[3] = -2;
    kernel[5] = 2;
    kernel[6] = -1;
    kernel[8] = 1;
    Filter::apply_kernel_det(img, kernel, kernel, 3);
    delete[] kernel;
}
void Filter::filter_prewitt(Image *img)
{
    // define a 3x3 set matrix - chosen by prewitt condition
    int *kernel = new int[9]();
    kernel[0] = 1;
    kernel[2] = -1;
    kernel[3] = 1;
    kernel[5] = -1;
    kernel[6] = 1;
    kernel[8] = -1;
    // apply a convolution with a double kernel to the image
    Filter::apply_kernel_det(img, kernel, kernel, 3);
    delete[] kernel;
}
void Filter::filter_scharr(Image *img)
{
    // define set 3x3 scharr kernel
    int *kernel = new int[9]();
    kernel[0] = 3;
    kernel[2] = -3;
    kernel[3] = 10;
    kernel[5] = -10;
    kernel[6] = 3;
    kernel[8] = -3;
    Filter::apply_kernel_det(img, kernel, kernel, 3);
    delete[] kernel;
}
void Filter::filter_robertscross(Image *img)
{
    // define set 2x2 robertscross kernel
    int *kernelx = new int[4]();
    kernelx[0] = 1;
    kernelx[3] = -1;
    int *kernely = new int[4]();
    kernely[1] = -1;
    kernely[3] = 1;
    // apply the filters with both kernels using apply_kernel_det function
    Filter::apply_kernel_det(img, kernelx, kernely, 2);
    // clear memory
    delete[] kernelx;
    delete[] kernely;
}
void Filter::apply_kernel_det(Image *img, int *kernelx, int *kernely, int l)
{
    // load data in
    int channel = img->get_channel();
    int width = img->get_width();
    int height = img->get_height();
    const unsigned char *data = img->get_data();
    // get start and end coordinates for x,y loops
    int loopx_start, loopx_end, loopy_start, loopy_end;
    loopx_start = l / 2;
    loopx_end = width - l / 2;
    loopy_start = l / 2;
    loopy_end = height - l / 2;
    // temp image buffer to store data
    double *temp_img = new double[width * height];
    // max of filtered value
    double mymax = 0;
    int idx, val, center_idx, base_x, base_y;
    double res, gx, gy;
    // loop over each pixel in the image
    for (int i = loopy_start; i < loopy_end; i++)
    {
        for (int j = loopx_start; j < loopx_end; j++)
        {
            base_y = i - l / 2;
            base_x = j - l / 2;
            // gradients in x and y
            gx = 0;
            gy = 0;
            // loop over pixels in the kernel
            for (int k1 = 0; k1 < l; k1++)
            {
                for (int k2 = 0; k2 < l; k2++)
                {
                    idx = img->pixel_index(base_y + k1, base_x + k2, 0);
                    val = 0;
                    // check if the pixel is within the image boundary
                    if (idx != -1)
                    {
                        val = data[idx];
                    }
                    // add the weighted pixel value to the gradient
                    gx += val * kernelx[k1 * l + k2];
                    gy += val * kernely[k2 * l + k1];
                }
            }
            // find the center pixel index on the original image
            center_idx = img->pixel_index(i, j, 0);
            center_idx = i * width + j;
            // find and store mag of gradient vector in the temp buffer
            res = sqrt(pow(gx, 2) + pow(gy, 2));
            temp_img[center_idx] = res;
            if (res > mymax)
                // update the max value if new res is larger
                mymax = res;
        }
    }
    // loop over all pixels and update the values
    for (int i = loopy_start; i < loopy_end; i++)
    {
        for (int j = loopx_start; j < loopx_end; j++)
        {
            idx = img->pixel_index(i, j, 0);
            img->set_data_idx(idx, (int)(temp_img[i * width + j] / mymax * 255 + 0.5));
        }
    }
    // clear memeory
    delete[] temp_img;
}
void gaussian_3d_memory_efficient(Volume *vol, int kernel, int pad, double sig)
{
    int width, height, channel, n;
    channel = vol->get_channel();
    height = vol->get_height();
    width = vol->get_width();
    n = vol->get_num();

    // compute 1D gaussian filter + apply to each pixel value in volume
    double *arr = Filter::gaussian_vec(kernel, sig);
    int loopx_start, loopx_end, loopy_start, loopy_end, loopz_start, loopz_end;
    // calculate start and end points for loops
    loopx_start = kernel / 2 - pad;
    loopx_end = width + pad - kernel / 2;
    loopy_start = kernel / 2 - pad;
    loopy_end = height + pad - kernel / 2;
    loopz_start = kernel / 2 - pad;
    loopz_end = n + pad - kernel / 2;
    // define variables to store index for looping
    unsigned char *temp_img = new unsigned char[width * height * channel * n];
    int base_x, base_y, base_z;
    double total_val;
    int idx, val;
    double total_val2 = 0;
    double total_val3 = 0;
    double total_val4 = 0;
    // loop over all values in volume
    for (int i = loopy_start; i < loopy_end; i++)
    {
        for (int j = loopx_start; j < loopx_end; j++)
        {
            // loop over all volumes (either 1 for greyscale or 3 for rgb)
            for (int c = 0; c < channel; c++)
            {
                for (int k = loopz_start; k < loopz_end; k++)
                {
                    // find index for looping over kernel centered on current pixel
                    base_y = i - kernel / 2;
                    base_x = j - kernel / 2;
                    base_z = k - kernel / 2;
                    total_val = 0;
                    total_val2 = 0;
                    // loop over kernel centered on pixel
                    //   taking advantages of gaussian kernel to appy it efficiently
                    for (int k1 = 0; k1 < kernel; k1++)
                    {
                        total_val3 = 0;
                        for (int k2 = 0; k2 < kernel; k2++)
                        {
                            total_val4 = 0;
                            for (int k3 = 0; k3 < kernel; k3++)
                            {
                                total_val += 1;
                                // rate = arr[k1] * arr[k2] * arr[k3];
                                val = vol->get_data_idx(base_y + k1, base_x + k2, c, base_z + k3);
                                // total_val += val * rate;
                                total_val4 += val * arr[k3];
                            }
                            total_val3 += total_val4 * arr[k2];
                        }
                        total_val2 += total_val3 * arr[k1];
                    }
                    idx = vol->pixel_index(i, j, c, k);
                    // update temporary buffer with rounded convolved value to nearest int
                    temp_img[idx] = (int)(total_val2 + 0.5);
                }
            }
        }
    }
    // update pixel values with the convolved weighted gaussian values
    for (int i = loopy_start; i < loopy_end; i++)
    {
        for (int j = loopx_start; j < loopx_end; j++)
        {
            for (int c = 0; c < channel; c++)
            {
                for (int k = loopz_start; k < loopz_end; k++)
                {

                    idx = vol->pixel_index(i, j, c, k);
                    vol->set_data_idx(idx, temp_img[idx]);
                }
            }
        }
    }
    // clear memory
    delete[] temp_img;
    delete[] arr;
}
void gaussian_3d_time_efficient(Volume *vol, int kernel, int pad, double sig)
{
    int width, height, channel, n;
    channel = vol->get_channel();
    height = vol->get_height();
    width = vol->get_width();
    n = vol->get_num();

    // compute 1D gaussian filter + apply to each pixel value in volume
    double *arr = Filter::gaussian_vec(kernel, sig);
    int loopx_start, loopx_end, loopy_start, loopy_end, loopz_start, loopz_end;
    // calculate start and end points for loops
    loopx_start = kernel / 2 - pad;
    loopx_end = width + pad - kernel / 2;
    loopy_start = kernel / 2 - pad;
    loopy_end = height + pad - kernel / 2;
    loopz_start = kernel / 2 - pad;
    loopz_end = n + pad - kernel / 2;
    // define variables to store index for looping
    float *temp_img = new float[width * height * channel * n];
    float *temp_img2 = new float[width * channel * n * height];
    int base_x, base_y, base_z;
    float total_val;
    int idx, val;
    // loop over all values in volume
    for (int i = 0; i < height * width * channel * n; i++)
    {
        temp_img[i] = vol->get_data()[i];
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // loop over all volumes (either 1 for greyscale or 3 for rgb)
            for (int c = 0; c < channel; c++)
            {
                for (int k = 0; k < n; k++)
                {
                    // find index for looping over kernel centered on current pixel
                    base_y = i - kernel / 2;
                    total_val = 0;
                    // loop over kernel centered on pixel
                    //   taking advantages of gaussian kernel to appy it efficiently
                    for (int k1 = 0; k1 < kernel; k1++)
                    {
                        val = vol->get_data_idx(base_y + k1, j, c, k);
                        total_val += val * arr[k1];
                    }
                    idx = vol->pixel_index(i, j, c, k);
                    // update temporary buffer with rounded convolved value to nearest int
                    temp_img[idx] = total_val;
                }
            }
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // loop over all volumes (either 1 for greyscale or 3 for rgb)
            for (int c = 0; c < channel; c++)
            {
                for (int k = 0; k < n; k++)
                {
                    // find index for looping over kernel centered on current pixel
                    base_x = j - kernel / 2;
                    total_val = 0;
                    // loop over kernel centered on pixel
                    //   taking advantages of gaussian kernel to appy it efficiently
                    for (int k1 = 0; k1 < kernel; k1++)
                    {

                        val = vol->pixel_index(i, base_x + k1, c, k);
                        if (val != -1)
                            total_val += temp_img[val] * arr[k1];
                    }
                    idx = vol->pixel_index(i, j, c, k);
                    // update temporary buffer with rounded convolved value to nearest int
                    temp_img2[idx] = total_val;
                }
            }
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // loop over all volumes (either 1 for greyscale or 3 for rgb)
            for (int c = 0; c < channel; c++)
            {
                for (int k = 0; k < n; k++)
                {
                    // find index for looping over kernel centered on current pixel
                    base_z = k - kernel / 2;
                    total_val = 0;
                    // loop over kernel centered on pixel
                    //   taking advantages of gaussian kernel to appy it efficiently
                    for (int k1 = 0; k1 < kernel; k1++)
                    {

                        val = vol->pixel_index(i, j, c, base_z + k1);
                        if (val != -1)
                            total_val += temp_img2[val] * arr[k1];
                    }
                    idx = vol->pixel_index(i, j, c, k);
                    // update temporary buffer with rounded convolved value to nearest int
                    temp_img[idx] = total_val;
                }
            }
        }
    }
    for (int i = loopy_start; i < loopy_end; i++)
    {
        for (int j = loopx_start; j < loopx_end; j++)
        {
            for (int c = 0; c < channel; c++)
            {
                for (int k = loopz_start; k < loopz_end; k++)
                {

                    idx = vol->pixel_index(i, j, c, k);
                    vol->set_data_idx(idx, (int)(temp_img[idx] + 0.5));
                }
            }
        }
    }
    // clear memory
    delete[] temp_img;
    delete[] temp_img2;
    delete[] arr;
}
void Filter::gaussian_3d(Volume *vol, int kernel, int pad, double sig, bool which)
{
    if (which)
        gaussian_3d_memory_efficient(vol, kernel, pad, sig);
    else
        gaussian_3d_time_efficient(vol, kernel, pad, sig);
}
void Filter::median_3d(Volume *vol, int kernel, int pad)
{
    // load image data
    int width, height, channel, n;
    channel = vol->get_channel();
    height = vol->get_height();
    width = vol->get_width();
    n = vol->get_num();
    // store index for looping in kernel
    int loopx_start, loopx_end, loopy_start, loopy_end, loopz_start, loopz_end;
    loopx_start = kernel / 2 - pad;
    loopx_end = width + pad - kernel / 2;
    loopy_start = kernel / 2 - pad;
    loopy_end = height + pad - kernel / 2;
    loopz_start = kernel / 2 - pad;
    loopz_end = n + pad - kernel / 2;
    // create temporary buffer for storing data + initialize variables for looping
    unsigned char *arr = new unsigned char[kernel * kernel * kernel];
    unsigned char *temp_img = new unsigned char[width * height * channel * n];
    int base_x, base_y, base_z;
    double total_val;
    int idx, val;
    double median;
    unsigned char unmd;
    // loop through each pixel in the volume + channel
    for (int i = loopy_start; i < loopy_end; i++)
    {
        cout << i << endl;
        for (int j = loopx_start; j < loopx_end; j++)
        {
            for (int c = 0; c < channel; c++)
            {
                for (int k = loopz_start; k < loopz_end; k++)
                {
                    // set the base coordinates for kernel
                    base_y = i - kernel / 2;
                    base_x = j - kernel / 2;
                    base_z = k - kernel / 2;
                    total_val = 0;
                    // loop through values in the kernel and add them to array
                    for (int k1 = 0; k1 < kernel; k1++)
                    {
                        for (int k2 = 0; k2 < kernel; k2++)
                        {
                            for (int k3 = 0; k3 < kernel; k3++)
                            {
                                arr[k1 * kernel * kernel + k2 * kernel + k3] = vol->get_data_idx(base_y + k1, base_x + k2, c, base_z + k3);
                            }
                        }
                    }
                    // quick selet to find the median
                    unmd = kthSmallest(arr, 0, kernel * kernel * kernel - 1, (kernel * kernel * kernel) / 2 + 1);
                    // my_sort(arr, (arr + kernel * kernel * kernel));
                    // median = arr[(kernel * kernel * kernel) / 2];
                    // if(median != unmd)
                    // {
                    //     cout<<median<<" differ with "<<(int)unmd;
                    // }
                    // update buffer
                    idx = vol->pixel_index(i, j, c, k);
                    temp_img[idx] = unmd;
                }
            }
        }
    }
    // Loop through volume topdate the image volume with the filtered value found
    for (int i = loopy_start; i < loopy_end; i++)
    {
        for (int j = loopx_start; j < loopx_end; j++)
        {
            for (int c = 0; c < channel; c++)
            {
                for (int k = loopz_start; k < loopz_end; k++)
                {

                    idx = vol->pixel_index(i, j, c, k);
                    vol->set_data_idx(idx, temp_img[idx]);
                }
            }
        }
    }
    delete[] temp_img;
    delete[] arr;
}

void Filter::filter_contrast_enhancement(Image *img)
{

    int channel = img->get_channel();
    int width = img->get_width();
    int height = img->get_height();

    const unsigned char *data = img->get_data();

    if (channel > 1)
    {
        if (channel > 3)
        {
            // Removing the alpha channel
            channel = 3;
            // Creating a new heap to store 3 channel image
            unsigned char *new_data = new unsigned char[width * height * channel];

            for (int i = 0; i < width * height; ++i)
            {
                new_data[i * 3] = data[i * 4];
                new_data[i * 3 + 1] = data[i * 4 + 1];
                new_data[i * 3 + 2] = data[i * 4 + 2];
            }
            img->replace_data(new_data);
            img->set_channel(channel);
            data = img->get_data();
        }
        // Creating pointers to hsv_image and RGB image
        unsigned char *hsv_data = new unsigned char[width * height * channel];
        unsigned char *rgb_image = new unsigned char[width * height * channel];

        // Intensity distribution
        std::vector<int> intensity_dist(256, 0);

        std::cout << "Converting RGB image to HSV" << std::endl;
        // Converting RGB image to HSV
        for (int i = 0; i < width * height * channel; i += channel)
        { // Problems with channels
            float r = data[i] / 255.0;
            float g = data[i + 1] / 255.0;
            float b = data[i + 2] / 255.0;

            float h, s, v;
            rgb_to_hsv(r, g, b, h, s, v);

            // convert hue to 0-255 range
            hsv_data[i] = static_cast<unsigned char>((h / 360) * 255);
            hsv_data[i + 1] = static_cast<unsigned char>(s * 255);
            hsv_data[i + 2] = static_cast<unsigned char>(v * 255);
            uint8_t pixel = static_cast<uint8_t>(hsv_data[i + 2]);
            // Pixel value distribution
            intensity_dist[pixel]++;
        }
        // Cummulative distribution function
        std::vector<int> cdf(256, 0);

        int sum = 0;
        for (int i = 0; i < 256; ++i)
        {
            sum += intensity_dist[i];
            cdf[i] = sum;
        }
        // Normalize CDF to [0, 255]
        int min_cdf = cdf[0];
        int max_cdf = cdf[255];
        // Normalising
        for (int i = 0; i < 256; ++i)
        {
            cdf[i] = static_cast<int>((cdf[i] - min_cdf) * 255.0 / (max_cdf - min_cdf));
        }
        // Mapping pixel intensities to new values using CDF
        std::cout << "Enhancing contrast" << std::endl;

        // Histogram equalisation for value channel only
        for (int i = 0; i < width * height * channel; i += channel)
        {
            uint8_t pixel = hsv_data[i + 2];
            hsv_data[i + 2] = static_cast<unsigned char>(cdf[pixel]);
        }
        std::cout << "Converting back to RGB" << std::endl;

        // Converting Image back to RGB
        for (int i = 0; i < width * height * channel; i += channel)
        { // Problems with channels
            float h = hsv_data[i] / 255.0;
            float s = hsv_data[i + 1] / 255.0;
            float v = hsv_data[i + 2] / 255.0;

            float r, g, b;
            hsv_to_rgb(h, s, v, r, g, b);

            rgb_image[i] = static_cast<unsigned char>(r * 255);
            rgb_image[i + 1] = static_cast<unsigned char>(g * 255);
            rgb_image[i + 2] = static_cast<unsigned char>(b * 255);
        }
        // Replacing original image buffer with enhanced rgb image
        img->replace_data(rgb_image);
        // Deallocating the hsv_data from heap.
        delete[] hsv_data;
    }
    else if (channel == 1)
    {
        filter_histogram_equalization(img);
    }
}

void Filter::filter_hsv(Image *img)
{

    int channel = img->get_channel();
    int width = img->get_width();
    int height = img->get_height();

    const unsigned char *data = img->get_data();

    if (channel > 1)
    {
        if (channel > 3)
        {
            // Removing the alpha channel
            channel = 3;
            // Creating a new heap to store 3 channel image
            unsigned char *new_data = new unsigned char[width * height * channel];

            for (int i = 0; i < width * height; ++i)
            {
                new_data[i * 3] = data[i * 4];
                new_data[i * 3 + 1] = data[i * 4 + 1];
                new_data[i * 3 + 2] = data[i * 4 + 2];
            }
            img->replace_data(new_data);
            img->set_channel(channel);
            data = img->get_data();
        }
        // Creating pointer to hsv_image
        unsigned char *hsv_data = new unsigned char[width * height * channel];

        std::cout << "Converting RGB image to HSV" << std::endl;
        // Converting RGB image to HSV
        for (int i = 0; i < width * height * channel; i += channel)
        { // Problems with channels
            float r = data[i] / 255.0;
            float g = data[i + 1] / 255.0;
            float b = data[i + 2] / 255.0;

            float h, s, v;

            rgb_to_hsv(r, g, b, h, s, v);

            // convert hue, saturation and value to 0-255 range
            hsv_data[i] = static_cast<unsigned char>((h / 360) * 255);
            hsv_data[i + 1] = static_cast<unsigned char>(s * 255);
            hsv_data[i + 2] = static_cast<unsigned char>(v * 255);
        }
        img->replace_data(hsv_data);
    }
}

void Filter::rgb_to_hsv(float &r, float &g, float &b, float &h, float &s, float &v)
{
    float min_val = std::min(std::min(r, g), b);
    float max_val = std::max(std::max(r, g), b);
    float delta = max_val - min_val;

    // Compute Hue
    if (delta == 0)
    {
        h = 0;
    }
    else if (max_val == r)
    {
        h = fmod((g - b) / delta + 6.0, 6.0);
    }
    else if (max_val == g)
    {
        h = 2 + (b - r) / delta;
    }
    else
    {
        h = 4 + (r - g) / delta;
    }

    h *= 60;
    if (h < 0)
    {
        h += 360;
    }
    // Compute the saturation
    if (max_val == 0)
    {
        s = 0;
    }
    else
    {
        s = delta / max_val;
    }
    // Compute the value
    v = max_val;
}

void Filter::hsv_to_rgb(float &h, float &s, float &v, float &r, float &g, float &b)
{

    if (s == 0)
    {
        r = g = b = v;
    }
    else
    {
        int j = (int)(h * 6);
        float f = h * 6 - j;
        float p = v * (1 - s);
        float q = v * (1 - f * s);
        float t = v * (1 - (1 - f) * s);

        switch (j % 6)
        {
        // Assigining RGB values
        case 0:
            r = v;
            g = t;
            b = p;
            break;
        case 1:
            r = q;
            g = v;
            b = p;
            break;
        case 2:
            r = p;
            g = v;
            b = t;
            break;
        case 3:
            r = p;
            g = q;
            b = v;
            break;
        case 4:
            r = t;
            g = p;
            b = v;
            break;
        case 5:
            r = v;
            g = p;
            b = q;
            break;
        }
    }
}
