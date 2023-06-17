/**
 * @file Image.cpp
 *
 * @brief This file implement all the functionalities in Image class
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
#include "Image.h"
#include <iostream>
#include <algorithm>
// #include <vector>
#include <math.h>
#include <cstring>
#include "stb_image_write.h"
#define PI 3.14159265358979323846
using namespace std;
Image::Image(unsigned char *d, int w, int h, int c)
{
    // input data using pointer to location input by user
    //set data of size width*height*number of channels
    data = new unsigned char[w * h * c];
//copy data memory block
    memcpy(data, d, w * h * c);
    width = w;
    height = h;
    channel = c;
    // cout << "Image loaded with size " << w << " x " << h << " with " << c << " channel(s)." << endl;
}
Image::Image(int w, int h, int c)
{
    data = new unsigned char[w * h * c];
    memset(data, 0, w * h * c);
    width = w;
    height = h;
    channel = c;
    // cout << "Image loaded with size " << w << " x " << h << " with " << c << " channel(s)." << endl;
}
int Image::get_width() const
{
    return width;
}

int Image::get_height() const
{
    return height;
}

int Image::get_channel() const
{
    //number of channels
    return channel;
}
//checks if index is a valid index in the image (if it exist)
bool Image::valid_idx(int x, int y) const
{
    
    if (x < width && x >= 0)
    {
        if (y < height && y >= 0)
            return true;
    }
    return false;
}
//fetches image data
const unsigned char *Image::get_data() const
{
    return data;
}
//gets the index of a speficif height, weidth and channels position
int Image::pixel_index(int h, int w, int c) const
{
    //if index is invalid
    if (!Image::valid_idx(w, h))
    {
        return -1;
    }
    int idx = h * width * channel + w * channel + c;
    return idx;
}
//gets the average of 2 points (x1,y1) and (x2,y2)
double Image::average_by_channel(int x1, int x2, int y1, int y2, int c) const
{
    double sum_value = 0;
    int idx;
    for (int i = y1; i < y2; i++)
    {
        for (int j = x1; j < x2; j++)
        {
             //get pixel index
            idx = Image::pixel_index(i, j, c);
            //check if valid
            if (idx != -1)
            {
                sum_value += data[idx];
            }
        }
    }
    return sum_value / (x2 - x1) / (y2 - y1);
}
//replace data with new (filtered) data
void Image::replace_data(unsigned char *newdata)
{
    delete[] data;
    data = newdata;
}
//set particular index in data to a value
void Image::set_data_idx(int idx, int val)
{
    data[idx] = val;
}
//pull data from a specific index
unsigned char Image::get_data_by_idx(int idx) const
{
    return data[idx];
}
// change the number of channels
void Image::set_channel(int newchannel)
{
    channel = newchannel;
}
int Image::save_img(const char *filename) const
{
    return stbi_write_png(filename, width, height, channel, data, 0);
}
Image::~Image()
{
    delete[] data;
}

