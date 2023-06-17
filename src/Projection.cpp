/**
 * @file Project.cpp
 *
 * @brief This file implement the functionalties of Projection class
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
#include "Projection.h"
#include <iostream>
#include "MemManager.h"
#include <cstring>
extern MemManager my_garbage_collector;
Image *Projection::project(Volume *vol, int start, int end, double (*func)(unsigned char *, int))
{
    //load data
    int width, height, channel, idx, res, imgidx;
    channel = vol->get_channel();
    height = vol->get_height();
    width = vol->get_width();
    //calculate array size to act on based on user input of start/end
    int arr_size = end + 1 - start;
     //set a temporary buffer to store filtered image (acted on by func)
    unsigned char *temp_array = new unsigned char[arr_size];
    Image *img = new Image(width, height, channel);
    //automatic memory management
    my_garbage_collector.allocate(img, 0, "img");
     //loop through all pixel values and apply a function/filter to all values
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            for (int k = 0; k < channel; k++)
            {
                idx = vol->pixel_index(i, j, k, start);
                memcpy(temp_array, vol->get_data() + idx, arr_size);
                res = func(temp_array, arr_size);
                //update values with new projection
                imgidx = img->pixel_index(i, j, k);
                img->set_data_idx(imgidx, res);

            }
        }
    }
    //clear memory
    delete[] temp_array;
    return img;
}
