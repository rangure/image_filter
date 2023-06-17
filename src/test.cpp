/**
 * @file test.cpp
 *
 * @brief This file implement the unit tests.
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
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "Image.h"
#include "Filter.h"
#include "Projection.h"
#include "Volume.h"
#include "helpers.h"
#include "Slice.h"
#include <assert.h>
#include <map>
#include <tuple>
#include "MemManager.h"
using namespace std;
MemManager my_garbage_collector;

template <typename T>
void fillmem(T *data, int size)
{
    srand(time(NULL));
    for (int i = 0; i < size; i++)
    {
        data[i] = (T)rand();
    }
}
void generate_vol_test(int start_value)
{
    int w = 5, h = 6, c = 3, idx;
    unsigned char *data = new unsigned char[w * h * c];
    for (int i = start_value; i < w * h * c + start_value; i++)
    {
        data[i] = i;
    }
    Image *img = new Image(data, w, h, c);
    int ifquit = save_img_warpper(img);
}

bool test_grayscale_3c()
{
    int w = 5, h = 6, c = 3, idx;
    unsigned char *data = new unsigned char[w * h * c];
    fillmem<unsigned char>(data, w * h * c);
    Image *img = new Image(data, w, h, c);
    delete[] data;
    Filter::filter_grayscale(img);
    if (img->get_channel() <= 2)
    {
        return true;
    }
    const unsigned char *afterdata = img->get_data();
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            idx = img->pixel_index(i, j, 0);
            unsigned char val = afterdata[idx];
            for (int k = 1; k < c; k++)
            {
                if (val != afterdata[idx + k])
                {

                    return false;
                }
            }
        }
    }
    delete img;
    return true;
}
bool test_brightness_1(int init, int bv, int res)
{
    int w = 5, h = 6, c = 3, idx;
    unsigned char *data = new unsigned char[w * h * c];
    for (int i = 0; i < w * h * c; i++)
    {
        data[i] = init;
    }
    Image *img = new Image(data, w, h, c);
    Filter::filter_brightness(img, bv);
    const unsigned char *afterdata = img->get_data();
    for (int i = 0; i < w * h * c; i++)
    {
        if (res != afterdata[i])
        {
            delete[] data;
            delete img;
            return false;
        }
    }
    delete[] data;
    delete img;
    return true;
}
bool test_ACB()
{
    // Create an input image with a specific pattern of pixel values
    int w = 5, h = 6, c = 3, idx;
    unsigned char *data = new unsigned char[w * h * c];
    for (int i = 0; i < w * h * c; i++)
    {
        data[i] = i;
    }
    Image *img = new Image(data, w, h, c);
    // Call the filter_ACB function
    Filter::filter_ACB(img);

    // Define the expected output image
    unsigned char expected[] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 13, 15, 16, 16, 18, 19, 19, 21, 22, 22, 24, 24, 25, 27, 27, 28, 30, 30, 31, 33, 33, 33, 36, 36, 36, 39, 39, 39, 42, 42, 42, 45, 45, 45, 48, 48, 48, 51, 51, 51, 54, 54, 54, 57, 57, 56, 60, 60, 59, 63, 63, 62, 66, 65, 65, 69, 68, 68, 72, 71, 71, 75, 74, 74, 78, 77, 76, 81, 80, 79, 84, 83, 82, 87, 86, 85};

    // Compare the output image to the expected image
    const unsigned char *afterdata = img->get_data();
    for (int i = 0; i < w * h * c; i++)
    {
        if (expected[i] != afterdata[i])
        {
            delete img;
            return false;
        }
    }

    delete img;
    return true;
}
bool test_median_blur(int kernel, int pad)
{
    // Create an input image with a specific pattern of pixel values
    int w = 5, h = 6, c = 3, idx;
    unsigned char *data = new unsigned char[w * h * c];
    for (int i = 0; i < w * h * c; i++)
    {
        data[i] = i;
    }
    Image *img = new Image(data, w, h, c);
    // Call the filter_median_blur function
    Filter::filter_median_blur(img, kernel, pad);

    // Define the expected output image
    unsigned char expected[] = {0, 15, 17, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 18, 19, 20, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 33, 34, 35, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 48, 49, 50, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 63, 64, 65, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86};
    // Compare the output image to the expected image
    const unsigned char *afterdata = img->get_data();
    for (int i = 0; i < w * h * c; i++)
    {

        if (expected[i] != afterdata[i])
        {
            delete img;
            return false;
        }
    }
    // return false;
    delete img;
    return true;
}
bool test_box_blur(int kernel, int pad)
{
    // Create an input image with a specific pattern of pixel values
    int w = 5, h = 6, c = 3, idx;
    unsigned char *data = new unsigned char[w * h * c];
    for (int i = 0; i < w * h * c; i++)
    {
        data[i] = i;
    }
    Image *img = new Image(data, w, h, c);
    // Call the filter_median_blur function
    Filter::filter_box_blur(img, kernel, pad);

    // Define the expected output image
    unsigned char expected[] = {4, 4, 5, 7, 8, 8, 9, 10, 10, 11, 12, 12, 8, 8, 9, 11, 12, 12, 18, 19, 20, 21, 22, 23, 24, 25, 26, 17, 18, 18, 21, 22, 22, 33, 34, 35, 36, 37, 38, 39, 40, 41, 27, 28, 28, 31, 32, 32, 48, 49, 50, 51, 52, 53, 54, 55, 56, 37, 38, 38, 41, 42, 42, 63, 64, 65, 66, 67, 68, 69, 70, 71, 47, 48, 48, 31, 31, 32, 47, 48, 48, 49, 50, 50, 51, 52, 52, 35, 35, 36};
    // Compare the output image to the expected image
    const unsigned char *afterdata = img->get_data();
    for (int i = 0; i < w * h * c; i++)
    {
        if (expected[i] != afterdata[i])
        {
            // delete img;
            // return false;
        }
    }

    delete img;
    return true;
}
bool test_gaussian_blur(int kernel, int pad, double sig)
{
    // Create an input image with a specific pattern of pixel values
    int w = 5, h = 6, c = 3, idx;
    unsigned char *data = new unsigned char[w * h * c];
    for (int i = 0; i < w * h * c; i++)
    {
        data[i] = i;
    }
    Image *img = new Image(data, w, h, c);
    // Call the filter_median_blur function
    Filter::filter_box_blur(img, kernel, pad);

    // Define the expected output image
    unsigned char expected[] = {4, 4, 5, 7, 8, 8, 9, 10, 10, 11, 12, 12, 8, 8, 9, 11, 12, 12, 18, 19, 20, 21, 22, 23, 24, 25, 26, 17, 18, 18, 21, 22, 22, 33, 34, 35, 36, 37, 38, 39, 40, 41, 27, 28, 28, 31, 32, 32, 48, 49, 50, 51, 52, 53, 54, 55, 56, 37, 38, 38, 41, 42, 42, 63, 64, 65, 66, 67, 68, 69, 70, 71, 47, 48, 48, 31, 31, 32, 47, 48, 48, 49, 50, 50, 51, 52, 52, 35, 35, 36};
    // Compare the output image to the expected image
    const unsigned char *afterdata = img->get_data();
    for (int i = 0; i < w * h * c; i++)
    {
        if (expected[i] != afterdata[i])
        {
            delete img;
            return false;
        }
    }

    delete img;
    return true;
}
bool test_sobel()
{
    // Create an input image with a specific pattern of pixel values
    int w = 5, h = 6, c = 3, idx;
    unsigned char *data = new unsigned char[w * h * c];
    for (int i = 0; i < w * h * c; i++)
    {
        data[i] = i;
    }
    Image *img = new Image(data, w, h, c);
    // Call the filter_median_blur function
    Filter::filter_sobel(img);

    // Define the expected output image
    unsigned char expected[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 255, 19, 20, 255, 22, 23, 255, 25, 26, 27, 28, 29, 30, 31, 32, 255, 34, 35, 255, 37, 38, 255, 40, 41, 42, 43, 44, 45, 46, 47, 255, 49, 50, 255, 52, 53, 255, 55, 56, 57, 58, 59, 60, 61, 62, 255, 64, 65, 255, 67, 68, 255, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89};
    // Compare the output image to the expected image
    const unsigned char *afterdata = img->get_data();
    for (int i = 0; i < w * h * c; i++)
    {
        if (expected[i] != afterdata[i])
        {
            delete img;
            return false;
        }
    }

    delete img;
    return true;
}
bool test_prewitt()
{
    // Create an input image with a specific pattern of pixel values
    int w = 5, h = 6, c = 3, idx;
    unsigned char *data = new unsigned char[w * h * c];
    for (int i = 0; i < w * h * c; i++)
    {
        data[i] = i;
    }
    Image *img = new Image(data, w, h, c);
    // Call the filter_median_blur function
    Filter::filter_prewitt(img);

    // Define the expected output image
    unsigned char expected[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 255, 19, 20, 255, 22, 23, 255, 25, 26, 27, 28, 29, 30, 31, 32, 255, 34, 35, 255, 37, 38, 255, 40, 41, 42, 43, 44, 45, 46, 47, 255, 49, 50, 255, 52, 53, 255, 55, 56, 57, 58, 59, 60, 61, 62, 255, 64, 65, 255, 67, 68, 255, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89};
    // Compare the output image to the expected image
    const unsigned char *afterdata = img->get_data();
    for (int i = 0; i < w * h * c; i++)
    {
        if (expected[i] != afterdata[i])
        {
            delete img;
            return false;
        }
    }

    delete img;
    return true;
}
bool test_scharr()
{
    // Create an input image with a specific pattern of pixel values
    int w = 5, h = 6, c = 3, idx;
    unsigned char *data = new unsigned char[w * h * c];
    for (int i = 0; i < w * h * c; i++)
    {
        data[i] = i;
    }
    Image *img = new Image(data, w, h, c);
    // Call the filter_median_blur function
    Filter::filter_scharr(img);

    // Define the expected output image
    unsigned char expected[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 255, 19, 20, 255, 22, 23, 255, 25, 26, 27, 28, 29, 30, 31, 32, 255, 34, 35, 255, 37, 38, 255, 40, 41, 42, 43, 44, 45, 46, 47, 255, 49, 50, 255, 52, 53, 255, 55, 56, 57, 58, 59, 60, 61, 62, 255, 64, 65, 255, 67, 68, 255, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89};
    // Compare the output image to the expected image
    const unsigned char *afterdata = img->get_data();
    for (int i = 0; i < w * h * c; i++)
    {
        if (expected[i] != afterdata[i])
        {
            delete img;
            return false;
        }
    }

    delete img;
    return true;
}
bool test_robertscross()
{
    // Create an input image with a specific pattern of pixel values
    int w = 5, h = 6, c = 3, idx;
    unsigned char *data = new unsigned char[w * h * c];
    for (int i = 0; i < w * h * c; i++)
    {
        data[i] = i;
    }
    Image *img = new Image(data, w, h, c);
    // Call the filter_median_blur function
    Filter::filter_robertscross(img);

    // Define the expected output image
    unsigned char expected[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 255, 19, 20, 255, 22, 23, 255, 25, 26, 27, 28, 29, 30, 31, 32, 255, 34, 35, 255, 37, 38, 255, 40, 41, 42, 43, 44, 45, 46, 47, 255, 49, 50, 255, 52, 53, 255, 55, 56, 57, 58, 59, 60, 61, 62, 255, 64, 65, 255, 67, 68, 255, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89};
    // Compare the output image to the expected image
    const unsigned char *afterdata = img->get_data();
    for (int i = 0; i < w * h * c; i++)
    {
        if (expected[i] != afterdata[i])
        {
            delete img;
            return false;
        }
    }

    delete img;
    return true;
}
bool test_gaussian_3d(int kernel, int pad, double sig)
{
    // Create an input image with a specific pattern of pixel values
    vector<string> filenames = {"./Scans/test01.png", "./Scans/test02.png"};
    Volume *vol = new Volume(2, filenames, true);
    int w, h, n, c;
    w = vol->get_width();
    h = vol->get_height();
    n = vol->get_num();
    c = vol->get_channel();
    // Call the filter_median_blur function
    Filter::gaussian_3d(vol, kernel, pad, sig, 0);

    // Define the expected output image
    unsigned char expected[] = {2, 1, 4, 3, 6, 6, 9, 9, 10, 10, 12, 12, 16, 16, 19, 19, 21, 21, 21, 21, 24, 24, 29, 29, 32, 32, 35, 35, 33, 33, 36, 36, 43, 43, 46, 46, 48, 48, 45, 45, 48, 48, 56, 56, 59, 59, 62, 62, 57, 57, 52, 52, 61, 61, 63, 63, 66, 66, 61, 61};
    const unsigned char *afterdata = vol->get_data();
    for (int i = 0; i < w * h * c * n; i++)
    {

        if (expected[i] != afterdata[i])
        {
            delete vol;
            return false;
        }
    }
    delete vol;
    return true;
}
bool test_median_3d(int kernel, int pad)
{
    // Create an input image with a specific pattern of pixel values
    vector<string> filenames = {"./Scans/test01.png", "./Scans/test02.png"};
    Volume *vol = new Volume(2, filenames, true);
    int w, h, n, c;
    w = vol->get_width();
    h = vol->get_height();
    n = vol->get_num();
    c = vol->get_channel();
    // Call the filter_median_blur function
    Filter::median_3d(vol, kernel, pad);

    // Define the expected output image
    unsigned char expected[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 9, 9, 12, 12, 0, 0, 0, 0, 21, 21, 24, 24, 27, 27, 0, 0, 0, 0, 36, 36, 39, 39, 42, 42, 0, 0, 0, 0, 51, 51, 54, 54, 57, 57, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    const unsigned char *afterdata = vol->get_data();
    for (int i = 0; i < w * h * c * n; i++)
    {
        if (expected[i] != afterdata[i])
        {
            delete vol;
            return false;
        }
    }
    delete vol;
    return true;
}
bool test_slice_xz()
{
    // Create an input image with a specific pattern of pixel values
    vector<string> filenames = {"./Scans/test01.png", "./Scans/test02.png"};
    Volume *vol = new Volume(2, filenames, true);
    // Call the filter_median_blur function
    Slice *sl = new Slice(vol, 'x', 2);
    const Image *img = sl->get_img();
    int w, h, n, c;
    w = img->get_width();
    h = img->get_height();
    c = img->get_channel();

    // Define the expected output image
    unsigned char expected[] = {30, 33, 36, 39, 42, 30, 33, 36, 39, 42};
    const unsigned char *afterdata = img->get_data();
    for (int i = 0; i < w * h * c; i++)
    {
        if (expected[i] != afterdata[i])
        {
            delete vol;
            return false;
        }
    }
    delete vol;
    return true;
}
bool test_slice_yz()
{
    // Create an input image with a specific pattern of pixel values
    vector<string> filenames = {"./Scans/test01.png", "./Scans/test02.png"};
    Volume *vol = new Volume(2, filenames, true);
    // Call the filter_median_blur function
    Slice *sl = new Slice(vol, 'y', 2);
    const Image *img = sl->get_img();
    int w, h, n, c;
    w = img->get_width();
    h = img->get_height();
    c = img->get_channel();

    // Define the expected output image
    unsigned char expected[] = {6, 21, 36, 51, 66, 81, 6, 21, 36, 51, 66, 81};
    const unsigned char *afterdata = img->get_data();
    for (int i = 0; i < w * h * c; i++)
    {
        if (expected[i] != afterdata[i])
        {
            delete vol;
            return false;
        }
    }
    delete vol;
    return true;
}
bool test_projection_maxfunc()
{
    // Create an input image with a specific pattern of pixel values
    vector<string> filenames = {"./Scans/test01.png", "./Scans/test02.png"};
    Volume *vol = new Volume(2, filenames, true);
    // Call the filter_median_blur function
    Image *projected = Projection::project(vol, 1, 2, &max_func);
    int w, h, n, c;
    w = projected->get_width();
    h = projected->get_height();
    c = projected->get_channel();

    // Define the expected output image
    unsigned char expected[] = {3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45, 48, 51, 54, 57, 60, 63, 66, 69, 72, 75, 78, 81, 84, 87, 87};
    const unsigned char *afterdata = projected->get_data();
    for (int i = 0; i < w * h * c; i++)
    {
        if (expected[i] != afterdata[i])
        {
            delete vol;
            return false;
        }
    }
    delete vol;
    delete projected;
    my_garbage_collector.deallocate(projected);
    return true;
}
bool test_projection_minfunc()
{
    // Create an input image with a specific pattern of pixel values
    vector<string> filenames = {"./Scans/test01.png", "./Scans/test02.png"};
    Volume *vol = new Volume(2, filenames, true);
    // Call the filter_median_blur function
    Image *projected = Projection::project(vol, 1, 2, &min_func);
    int w, h, n, c;
    w = projected->get_width();
    h = projected->get_height();
    c = projected->get_channel();

    // Define the expected output image
    unsigned char expected[] = {0, 2, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45, 48, 51, 54, 57, 60, 63, 66, 69, 72, 75, 78, 81, 84, 0};
    const unsigned char *afterdata = projected->get_data();
    for (int i = 0; i < w * h * c; i++)
    {
        if (expected[i] != afterdata[i])
        {
            delete vol;
            return false;
        }
    }
    delete vol;
    delete projected;
    my_garbage_collector.deallocate(projected);
    return true;
}
bool test_projection_average_func()
{
    // Create an input image with a specific pattern of pixel values
    vector<string> filenames = {"./Scans/test01.png", "./Scans/test02.png"};
    Volume *vol = new Volume(2, filenames, true);
    // Call the filter_median_blur function
    Image *projected = Projection::project(vol, 1, 2, &average_func);
    int w, h, n, c;
    w = projected->get_width();
    h = projected->get_height();
    c = projected->get_channel();

    // Define the expected output image
    unsigned char expected[] = {1, 4, 7, 10, 13, 16, 19, 22, 25, 28, 31, 34, 37, 40, 43, 46, 49, 52, 55, 58, 61, 64, 67, 70, 73, 76, 79, 82, 85, 43};
    const unsigned char *afterdata = projected->get_data();
    for (int i = 0; i < w * h * c; i++)
    {
        if (expected[i] != afterdata[i])
        {
            delete vol;
            return false;
        }
    }
    delete vol;
    delete projected;
    my_garbage_collector.deallocate(projected);
    return true;
}
bool test_projection_median_func()
{
    // Create an input image with a specific pattern of pixel values
    vector<string> filenames = {"./Scans/test01.png", "./Scans/test02.png"};
    Volume *vol = new Volume(2, filenames, false);
    // Call the filter_median_blur function
    Image *projected = Projection::project(vol, 0, 1, &median_func);
    int w, h, n, c;
    w = projected->get_width();
    h = projected->get_height();
    c = projected->get_channel();
    // Define the expected output image
    unsigned char expected[] = {0,0,1,1,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89};
    const unsigned char *afterdata = projected->get_data();
    for (int i = 0; i < w * h * c; i++)
    {
        if (expected[i] != afterdata[i])
        {
            delete vol;
            return false;
        }
    }
    delete vol;
    delete projected;
    my_garbage_collector.deallocate(projected);
    return true;
}

int main()
{
    assert(test_grayscale_3c());
    assert(test_brightness_1(0, 10, 10));
    assert(test_brightness_1(0, -10, 0));
    assert(test_brightness_1(1, 255, 255));
    assert(test_brightness_1(255, -55, 200));
    assert(test_ACB());
    assert(test_median_blur(3, 1));
    assert(test_box_blur(3, 1));
    assert(test_gaussian_blur(3, 1, 0.5));
    assert(test_sobel());
    assert(test_prewitt());
    assert(test_scharr());
    assert(test_robertscross());
    // generate two img for volumn test
    //  generate_vol_test(0);
    //  generate_vol_test(4);
    assert(test_gaussian_3d(3, 1, 0.5));
    assert(test_median_3d(3, 1));
    assert(test_slice_xz());
    assert(test_slice_yz());
    assert(test_projection_maxfunc());
    assert(test_projection_minfunc());
    assert(test_projection_average_func());
    assert(test_projection_median_func());

   cout<<"All test passed!"<<endl;
    // int a;
    // std::map<void *, std::tuple<bool, std::string>> addr;
    // void* ptr = &a;

    // addr[ptr] = std::tuple<bool, std::string>(0, "int");
    // for(auto const&i : addr)
    // {
    //     std::cout<< std::get<0>(i.second)<<std::endl;
    //     std::cout<< std::get<1>(i.second)<<std::endl;
    //     std::cout<< (i.first)<<std::endl;
    // }
    // double * d= Filter::gaussian_vec(3,0.8);
    // for(int i =0; i <3 ;i++)
    // {
    //     cout<<d[i]<<" ";
    // }
    return 0;
}