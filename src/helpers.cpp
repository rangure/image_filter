/**
 * @file helpers.cpp
 *
 * @brief This file implements handy helpers functions
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
#include <cmath>
#include <iostream>
// #include <algorithm>
#define PI 3.14159265358979323846
#include "helpers.h"
#include "Exceptions.h"
#include "Projection.h"
#include "MemManager.h"
#include "Slice.h"
#include "cstring"
#include <limits>

extern MemManager my_garbage_collector;

double gaussian_function(double x, double sig)
{
    return exp(-0.5 * pow(x / sig, 2)) / (sqrt(2 * PI) * sig);
}
void my_sort(unsigned char *arr_start, unsigned char *arr_end)
{
    mergesort(arr_start, 0, (arr_end - arr_start) + 1);
}
void merge(unsigned char *arr, int start, int mid, int end)
{
    int start2 = mid + 1;
    // If the direct merge is already sorted
    if (arr[mid] <= arr[start2])
    {
        return;
    }
    int len = end - start + 1;
    unsigned char *temp_arr = new unsigned char[len];
    int idx = 0;
    while (idx < len)
    {

        if ((arr[start] <= arr[start2] || start2 > end) && start <= mid)
        {
            temp_arr[idx] = arr[start];
            start++;
        }
        else
        {
            temp_arr[idx] = arr[start2];
            start2++;
        }
        idx++;
    }
    memcpy(arr + end - len + 1, temp_arr, len);
    delete[] temp_arr;
}

void mergesort(unsigned char *arr, int l, int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;
        mergesort(arr, l, m);
        mergesort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}
double average_func(unsigned char *arr, int n)
{
    double sumval = 0;
    for (int i = 0; i < n; i++)
    {
        sumval += arr[i];
    }
    return sumval / n;
}
double median_func(unsigned char *arr, int n)
{
    if(n==2)
    {
        return (arr[0]+arr[1])/2.0;
    }
    unsigned char *temp_arr = new unsigned char[n];
    memcpy(temp_arr, arr, n);
    // unsigned char a = kthSmallest(temp_arr, 0, n-1, n/2+1);
    // my_sort(temp_arr, temp_arr + n);
    double median;
    if (n % 2 == 0)
    {
        my_sort(temp_arr, temp_arr + n);
        median = (temp_arr[n / 2] + temp_arr[n / 2 - 1]) / 2.0;;
        
    }
    else
    {
        unsigned char a = kthSmallest(temp_arr, 0, n-1, n/2+1);
        median = a;

    }
    delete[] temp_arr;
    return median;
}
double max_func(unsigned char *arr, int n)
{
    double val = 0;
    for (int i = 0; i < n; i++)
    {
        if (val < arr[i])
            val = arr[i];
    }
    return val;
}
double min_func(unsigned char *arr, int n)
{
    double val = 256;
    for (int i = 0; i < n; i++)
    {
        if (val > arr[i])
            val = arr[i];
    }
    return val;
}
int get_kernel_size(int m)
{
    int kernel_size;
    while (true)
    {
        std::cout << "Please input kernel size.(quitting isn't supported at this step)" << std::endl;
        if (std::cin >> kernel_size && kernel_size <= m && kernel_size >= 0)
        {
            break;
        }
        else
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "invalid kernel size, please enter again." << std::endl;
        }
    }
    return kernel_size;
}
void get_padding_size(char &ifpad, int &pad)
{
    std::cout << "Do you want any padding?(y for yes, q for quit, others for no)" << std::endl;
    std::cin >> ifpad;
    switch (ifpad)
    {
    case 'y':
    {
        while (true)
        {
            std::cout << "Please input padding size.(quitting isn't supported at this step)" << std::endl;

            if (std::cin >> pad && pad >= 0)
            {
                break;
            }
            else
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "invalid pad size, please enter again." << std::endl;
            }
        }
        break;
    }
    case 'q':
        throw QuitProgram{};
    default:
        break;
    }
}
double get_sig()
{
    double sig;
    while (true)
    {
        std::cout << "Please input standard deviation.(quitting isn't supported at this step)" << std::endl;

        if (std::cin >> sig && sig > 0)
        {
            break;
        }
        else
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "invalid standard deviation, please enter again." << std::endl;
        }
    }
    return sig;
}

int get_slab(const char *s, int start_idx, int end_idx, int *arr, int minval)
{
    int slab;
    char rel;
    while (true)
    {
        std::cout << "Please input " << s << ", the value must be larger than " << minval << ".(quitting isn't supported at this step)" << std::endl;

        if (std::cin >> slab && slab >= minval && slab >= start_idx && slab <= end_idx)
        {
            if (arr[slab - start_idx] == -1)
            {
                std::cout << "this file doesn't exist, please enter again." << std::endl;
            }
            break;
        }
        else
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "invalid " << s << ", please enter again." << std::endl;
        }
    }
    return slab - start_idx;
}

int save_img_warpper(const Image *img)
{
    std::string savepath;
    while (true)
    {
        std::cout << "Please input path to save." << std::endl;
        std::cin >> savepath;
        if (savepath[0] == 'q' && savepath.size() == 1)
        {
            return -1;
        }
        std::cout << "Saving to " << savepath << std::endl;
        int suc = img->save_img(savepath.c_str());
        if (suc == 0)
        {
            char anothertry;
            std::cout << "save failed, do you want to try again?(y)" << std::endl;
            std::cin >> anothertry;
            if (anothertry != 'y')
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
    return 0;
}

int get_int(int ub, int lb, char const *msg)
{
    bool loop_flag_b = true;
    int ret;
    while (loop_flag_b)
    {
        try
        {
            std::cout << msg << "[" << lb << "," << ub << "].(quitting isn't supported at this step)" << std::endl;
            if (!(std::cin >> ret))
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "invalid input type." << std::endl;
                continue;
            }
            if (ret > ub || ret < lb)
            {

                throw BadBrightnessValue{};
            }
            loop_flag_b = false;
        }
        catch (BadBrightnessValue &bad)
        {
            std::cout << bad.what() << std::endl;
        }
    }
    return ret;
}

void apply_projection(char const *msg, int start_idx, int end_idx, int *simplemap, Volume *vol, double (*func)(unsigned char *, int))
{
    int slab_min, slab_max;
    char rel;
    std::cout << "Do you want to input a index to act on thin slab?(y for yes, q for quit, others for no)" << std::endl;
    std::cin >> rel;
    if (rel == 'q')
    {
        throw QuitProgram{};
    }
    if (rel == 'y')
    {
        try
        {
            slab_min = get_slab("Minimum z corrdinate for slab(indexing by the filename)", start_idx, end_idx, simplemap, start_idx);
            slab_max = get_slab("Maximum z corrdinate for slab(indexing by the filename)", start_idx, end_idx, simplemap, slab_min + start_idx);
        }
        catch (QuitProgram &q)
        {
            throw QuitProgram{};
        }
    }
    else
    {
        slab_min = 0;
        slab_max = end_idx - start_idx;
    }
    slab_min = simplemap[slab_min];
    slab_max = simplemap[slab_max];
    if (slab_max == -1 || slab_min == -1)
    {
        std::cout << "slab end file not found, quiting" << std::endl;
        throw QuitProgram{};
    }
    std::cout << msg << std::endl;
    Image *projected = Projection::project(vol, slab_min, slab_max, func);
    // my_garbage_collector.allocate(projected, 0, "img");
    char ifsave;
    std::cout << "Do you want to save the image?(y for yes, q for quit, others for no)" << std::endl;
    std::cin >> ifsave;
    switch (ifsave)
    {
    case 'y':
    {
        int ifquit = save_img_warpper(projected);
        if (ifquit == -1)
        {
            throw QuitProgram{};
        }
        my_garbage_collector.deallocate(projected);
        delete projected;
        break;
    }
    case 'q':
    {
        throw QuitProgram{};
    }

    default:
        my_garbage_collector.deallocate(projected);
        delete projected;
    }
}
void apply_slice(char dir, Volume *vol, int ub)
{
    int pos;
    char negdir;
    if (dir == 'x')
    {
        negdir = 'y';
    }
    else
    {
        negdir = 'x';
    }
    while (true)
    {
        std::cout << "Please input slicing " << negdir << " coordinate.(quitting isn't supported at this step)" << std::endl;
        if (std::cin >> pos && pos >= 0 && pos < ub)
        {
            break;
        }
        else
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "invalid coordinate, please enter again." << std::endl;
        }
    }
    Slice *sl = new Slice(vol, dir, pos);
    my_garbage_collector.allocate(sl, 0, "slice");
    char ifsave;
    std::cout << "Do you want to save the image?(y for yes, q for quit, others for no)" << std::endl;
    std::cin >> ifsave;
    switch (ifsave)
    {
    case 'y':
    {
        int ifquit = save_img_warpper(sl->get_img());
        if (ifquit == -1)
        {
            throw QuitProgram{};
        }
        break;
    }
    case 'q':
    {
        throw QuitProgram{};
        break;
    }
    default:;
    }
    my_garbage_collector.deallocate(sl);
    delete sl;
}

int partition(unsigned char *arr, int l, int r)
{
    int x = arr[r], i = l;
    int temp;
    for (int j = l; j <= r - 1; j++)
    {
        if (arr[j] <= x)
        {
            temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            i++;
        }
    }
    temp = arr[i];
    arr[i] = arr[r];
    arr[r] = temp;
    return i;
}

unsigned char kthSmallest(unsigned char *arr, int l, int r, int k)
{
    // If k is smaller than number of
    // elements in array
    if (k > 0 && k <= r - l + 1)
    {

        // Partition the array around last
        // element and get position of pivot
        // element in sorted array
        int index = partition(arr, l, r);

        // If position is same as k
        if (index - l == k - 1)
            return arr[index];

        // If position is more, recur
        // for left subarray
        if (index - l > k - 1)
            return kthSmallest(arr, l, index - 1, k);

        // Else recur for right subarray
        return kthSmallest(arr, index + 1, r,
                           k - index + l - 1);
    }
    else
    {
        throw QuitProgram{};
    }
    // If k is more than number of
    // elements in array
    return 255;
}
