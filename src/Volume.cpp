/**
 * @file Volume.cpp
 *
 * @brief This file implement functionalites of Volume class
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
#include "Volume.h"
#include "stb_image.h"
#include <iostream>
#include "Image.h"
#include "Filter.h"
#include "stb_image_write.h"
#include "Exceptions.h"
#include "MemManager.h"
using namespace std;
extern MemManager my_garbage_collector;
Volume::Volume(int n, std::vector<std::string> &filenames, bool ifgray)
{
    int w0, h0, c0;
    int w1, h1, c1;
    std::cout << "Volume loading start ..." << std::endl;
    unsigned char *data0 = stbi_load(filenames[0].c_str(), &w0, &h0, &c0, 0);
    if (data0 == NULL)
    {
        cout << "Image " << filenames[0] << " Load Failed, exiting." << endl;
        throw QuitProgram{};
    }
    Image *img = new Image(data0, w0, h0, c0);
    stbi_image_free(data0);
    if (ifgray)
    {
        Filter::filter_grayscale(img);
        w0 = img->get_width();
        h0 = img->get_height();
        c0 = img->get_channel();
    }
    num = n;
    width = w0;
    height = h0;
    channel = c0;
    delete img;
    data = new unsigned char[n * w0 * h0 * c0];
    my_garbage_collector.allocate(data, 1, "unsigned char");
    int idx, idx_img;
    for (int i = 0; i < num; i++)
    {
        mapping.push_back(filenames[i]);
        data0 = stbi_load(filenames[i].c_str(), &w1, &h1, &c1, 0);
        if (data0 == NULL)
        {
            cout << "Image " << filenames[i] << " Load Failed, exiting." << endl;
            stbi_image_free(data0);
            throw QuitProgram{};
        }
        img = new Image(data0, w1, h1, c1);
        my_garbage_collector.allocate(img, 0, "img");

        stbi_image_free(data0);
        if (ifgray)
        {
            Filter::filter_grayscale(img);
            w1 = img->get_width();
            h1 = img->get_height();
            c1 = img->get_channel();
        }
        if (w1 != w0 || h1 != h0 || c1 != c0)
        {
            cout << "Image " << filenames[i] << " have a different dimension, exiting." << endl;
            throw QuitProgram{};
;
        }
        for (int y = 0; y < h1; y++)
        {
            for (int x = 0; x < w1; x++)
            {
                for (int chan = 0; chan < channel; chan++)
                {
                    idx = pixel_index(y, x, chan, i);
                    idx_img = img->pixel_index(y, x, chan);
                    data[idx] = img->get_data_by_idx(idx_img);
                }
            }
        }
        delete img;
        my_garbage_collector.deallocate(img);

    }
    my_garbage_collector.deallocate(data);

}
int Volume::pixel_index(int h, int w, int c, int n) const
{
    if (!Volume::valid_idx(w, h, c, n))
    {
        return -1;
    }
    int idx = h * width * channel * num + w * channel * num + c * num + n;
    return idx;
}
int Volume::get_width() const
{
    return width;
}

int Volume::get_height() const
{
    return height;
}

int Volume::get_channel() const
{
    return channel;
}
int Volume::get_num() const
{
    return num;
}
const unsigned char *Volume::get_data() const
{
    return data;
}
bool Volume::valid_idx(int x, int y, int c, int n) const
{
    if (x < width && x >= 0)
    {
        if (y < height && y >= 0)
            if (n < num && n >= 0)
                if (c < channel && c >= 0)
                    return true;
    }
    return false;
}
unsigned char Volume::get_data_idx(int h, int w, int c, int n)
{
    int idx = pixel_index(h, w, c, n);
    if (idx == -1)
        return 0;
    else
        return data[idx];
}
void Volume::set_data_idx(int idx, int val)
{
    if (idx == -1)
    {
        cout << "invalid index for vol" << endl;
        exit(1);
    }
    else
        data[idx] = val;
}
Volume::~Volume()
{
    delete[] data;
}