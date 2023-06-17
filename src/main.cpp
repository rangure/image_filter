/**
 * @file main.cpp
 *
 * @brief This file implement the main control flow of the user interface
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
#include "MemManager.h"
#include "Exceptions.h"
#include <limits>
using namespace std;
// This is a garbage collector which will deallocate the allocated memory. detailed documentation can be found in
//  class defination. This is defined as a global variable beacuse otherwise the code is way more complicated if i
//  want to use it in all the defined classes. An alternative is to define the private variable that's managing the
//  memory as a static variable, and deallocate all the memory using a static member function instead in the destructor.
//  this will requires to call the member function before we're returning. If we define it as global variable, the variable
//  is destroyed when the program exit, which will call it's destructor and deacllocate all the memory.
MemManager my_garbage_collector;
int main()
{
    cout << "Welcome to the Image processing software developed by group Warshall!" << endl;
    cout << "You could enter q to exit at any time." << endl;
    cout << "Enter the letter before \":\" to choose from the provide options." << endl;
    char datatype;
    bool exflag = true;
    cout << "What are you tring to load?" << endl;
    while (exflag)
    {
        try
        {
            cout << "a: Single image" << endl;
            cout << "b: 3D Volume" << endl;
            cin >> datatype;
            if (datatype == 'q')
                throw QuitProgram{};
            if (datatype != 'a' && datatype != 'b')
                throw "invalid argument";
            exflag = 0;
        }
        catch (char const *s)
        {
            cout << "there aren't any options matching " << datatype << " please choose again" << endl;
        }
        catch (QuitProgram &q)
        {
            cout << q.what() << endl;
            return 0;
        }
    }
    if (datatype == 'a')
    {
        string imgpath;
        bool exflag = true;

        while (exflag)
        {
            try
            {
                cout << "input the path of the image you want to load:" << endl;
                cin >> imgpath;
                ifstream ifile;
                if (imgpath[0] == 'q' && imgpath.size() == 1)
                    throw QuitProgram{};
                ifile.open(imgpath);
                if (!ifile)
                {
                    throw InvalidPath{};
                }
                else
                {
                    ifile.close();
                    exflag = false;
                }
            }
            catch (QuitProgram &q)
            {
                cout << q.what() << endl;
                return 0;
            }
            catch (InvalidPath &ip)
            {
                cout << "Path " + imgpath + " doesn't exist, input again." << endl;
            }
        }
        int w, h, c;
        unsigned char *data;
        cout << "loading " << imgpath << endl;
        try
        {
            data = stbi_load(imgpath.c_str(), &w, &h, &c, 0);

            if (data == NULL)
            {
                throw LoadFail{};
            }
        }
        catch (LoadFail &lf)
        {
            cout << lf.what() << endl;
            return 0;
        }
        cout << "Image loaded with size " << w << " x " << h << " with " << c << " channel(s)." << endl;
        Image *img = new Image(data, w, h, c);
        my_garbage_collector.allocate(img, 0, "img");
        stbi_image_free(data);
        bool breakflag = true;
        cout << "Choose a filter to apply, the filters will be applied on the previous result, no on the original image" << endl;
        while (breakflag)
        {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "\nWhat operation do you want to apply?" << endl;
            cout << "a: Grayscale filter" << endl;
            cout << "b: Automatic colour balance filter" << endl;
            cout << "c: Brightness filter" << endl;
            cout << "d: Contrast Enhancement" << endl;
            cout << "e: Median blur filter" << endl;
            cout << "f: Box blur filter" << endl;
            cout << "g: Gaussian blur filter" << endl;
            cout << "h: Sobel edge detection filter" << endl;
            cout << "i: Prewitt edge detection filter" << endl;
            cout << "j: Scharr edge detection filter" << endl;
            cout << "k: Roberts\' Cross edge detection filter" << endl;
            cout << "n: go to next step." << endl;

            char filtertype;
            try
            {

                if (!(cin >> filtertype))
                {
                    continue;
                }
                switch (filtertype)
                {
                case 'a':
                {
                    cout << "Applying Grayscal filter" << endl;
                    Filter::filter_grayscale(img);
                    break;
                }
                case 'b':
                {
                    cout << "Applying Automatic colour balance filter" << endl;
                    Filter::filter_ACB(img);
                    break;
                }
                case 'c':
                {
                    cout << "What type of brightness filter do you want to apply?" << endl;
                    cout << "a: Automatic brightness filter." << endl;
                    cout << "b: Shift brightness of all pixels." << endl;
                    char brightnessflag;
                    cin >> brightnessflag;
                    switch (brightnessflag)
                    {
                    case 'a':
                    {
                        cout << "Applying Brightness filter" << endl;
                        Filter::filter_brightness(img);
                        break;
                    }
                    case 'b':
                    {
                        int bv = get_int(255, -255, "Input the brightness value ");
                        cout << "Applying Brightness filter" << endl;
                        Filter::filter_brightness(img, bv);
                        break;
                    }

                    case 'q':
                    {
                        throw QuitProgram{};
                    }
                    default:
                        cout << "there aren't any options matching " << brightnessflag << " please choose again." << endl;
                        break;
                    }
                    break;
                }

                case 'd':
                {
                    cout << "What type of Contrast Enhancement filter do you want to apply?" << endl;
                    cout << "a: RGB contrast enhancement." << endl;
                    cout << "b: Histogram equalisation for greyscale." << endl;
                    cout << "c: HSV colour space" << endl;
                    char contrast_enhancement;
                    cin >> contrast_enhancement;
                    switch (contrast_enhancement)
                    {
                    case 'a':
                    {
                        cout << "Applying RGB contrast enhancement" << endl;
                        Filter::filter_contrast_enhancement(img);
                        break;
                    }
                    case 'b':
                    {

                        cout << "Applying Histogram equalisation for greyscale." << endl;
                        Filter::filter_histogram_equalization(img);
                        break;
                    }
                    case 'c':
                    {
                        cout << "Converting image to HSV colour space." << endl;
                        Filter::filter_hsv(img);
                        break;
                    }
                    case 'q':
                    {
                        throw QuitProgram{};
                    }
                    default:
                        cout << "there aren't any options matching " << contrast_enhancement << " please choose again." << endl;
                        break;
                    }
                    break;
                }
                case 'e':
                {
                    int kernel_size = get_kernel_size((h > w) ? w : h);
                    int pad = 0;
                    char ifpad;
                    get_padding_size(ifpad, pad);
                    if (ifpad == 'q')
                    {
                        throw QuitProgram{};
                    }

                    cout << "Applying Median blur filter" << endl;
                    Filter::filter_median_blur(img, kernel_size, pad);
                    break;
                }
                case 'f':
                {
                    int kernel_size = get_kernel_size((h > w) ? w : h);
                    int pad = 0;
                    char ifpad;
                    get_padding_size(ifpad, pad);
                    if (ifpad == 'q')
                    {
                        return 0;
                    }
                    cout << "Applying Box blur filter" << endl;
                    Filter::filter_box_blur(img, kernel_size, pad);
                    break;
                }
                case 'g':
                {
                    int kernel_size = get_kernel_size((h > w) ? w : h);
                    int pad = 0;
                    char ifpad;
                    get_padding_size(ifpad, pad);
                    if (ifpad == 'q')
                    {
                        return 0;
                    }
                    double sig = get_sig();
                    cout << "Applying Gaussian blur filter" << endl;
                    Filter::filter_gaussian_blur(img, kernel_size, pad, sig);
                    break;
                }
                case 'h':
                {
                    cout << "Applying grayscale filter before edge detection filter" << endl;
                    Filter::filter_grayscale(img);
                    cout << "Applying Sobel edge detection filter" << endl;
                    Filter::filter_sobel(img);
                    break;
                }
                case 'i':
                {
                    cout << "Applying grayscale filter before edge detection filter" << endl;
                    Filter::filter_grayscale(img);
                    cout << "Applying Prewitt edge detection filter" << endl;
                    Filter::filter_prewitt(img);
                    break;
                }
                case 'j':
                {
                    cout << "Applying grayscale filter before edge detection filter" << endl;
                    Filter::filter_grayscale(img);
                    cout << "Applying Scharr edge detection filter" << endl;
                    Filter::filter_scharr(img);
                    break;
                }
                case 'k':
                {
                    cout << "Applying grayscale filter before edge detection filter" << endl;
                    Filter::filter_grayscale(img);
                    cout << "Applying Roberts\' Cross edge detection filter" << endl;
                    Filter::filter_robertscross(img);
                    break;
                }
                case 'n':
                {
                    breakflag = false;
                    break;
                }
                case 'q':
                {
                    throw QuitProgram{};
                }
                default:
                    cout << "there aren't any options matching " << filtertype << " please choose again." << endl;
                }
            }
            catch (QuitProgram &q)
            {
                cout << q.what() << endl;
                return 0;
            }
        }
        char ifsave;
        cout << "Do you want to save the image?(y for yes, q for quit, others for no)" << std::endl;
        cin >> ifsave;
        switch (ifsave)
        {
        case 'y':
        {
            int ifquit = save_img_warpper(img);
            if (ifquit == -1)
            {
                return 0;
            }
            break;
        }
        case 'q':
        {
            return 0;
        }
        default:;
        }
    }
    else
    {
        cout << "To load a 3D volume, we need 5 inputs." << endl;
        cout << "For example, if we want to load Scans/confuciusornis/confuYZ0010.png to confuYZ0070.png, the five parts are as follows." << endl;
        cout << "prefix: the prefix of the volume (Scans/confuciusornis/confuYZ)" << endl;
        cout << "suffix: the suffix of individual file (.png)" << endl;
        cout << "start idx: the start idx of the volume (10)" << endl;
        cout << "end idx: the end idx of the volume (70)" << endl;
        cout << "how many digits: how many digits is required for the indexing (because 0070 have 4 digits, this will be 4)" << endl
             << endl;
        std::string prefix;
        std::string suffix;
        int start_idx, end_idx;
        int length;
        cout << "Please input the prefix of the Volume path." << endl;
        cin >> prefix;
        if (prefix[0] == 'q' && prefix.size() == 1)
        {
            return 0;
        }
        cout << "Please input the suffix of the Volume path." << endl;
        cin >> suffix;
        if (suffix[0] == 'q' && suffix.size() == 1)
        {
            return 0;
        }
        while (true)
        {
            cout << "Please input the start index of the Volume path.(int)" << endl;
            if (cin >> start_idx && start_idx >= 0)
            {
                break;
            }
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << start_idx << " is invalid start index, please enter again." << endl;
        }
        while (true)
        {
            cout << "Please input the end index of the Volume path.(int)" << endl;
            if (cin >> end_idx && end_idx >= start_idx)
            {
                break;
            }
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << end_idx << " is invalid end index, please enter again." << endl;
        }
        while (true)
        {
            cout << "Please input the number of digits of the Volume path.(int)" << endl;
            if (cin >> length && length >= 0)
            {
                break;
            }
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << length << " is invalid number of digits, please enter again." << endl;
        }
        char ifgray;
        cout << "Do you want to apply grayscale filter on the input images?(y for yes, q for quit, others for no)" << endl;
        cin >> ifgray;
        if (ifgray == 'q')
        {
            return 0;
        }
        std::vector<std::string> filenames;
        std::ifstream ifile;
        int *simplemap = new int[end_idx - start_idx + 1];
        my_garbage_collector.allocate(simplemap, 1, "int");

        for (int i = start_idx; i <= end_idx; i++)
        {
            std::string s = std::to_string(i);
            std::string res = prefix + std::string(length - s.size(), '0').append(s) + suffix;
            ifile.open(res);
            if (ifile)
            {
                ifile.close();
                simplemap[i - start_idx] = filenames.size();
                filenames.push_back(res);
            }
            else
            {
                std::cout << res << " doesn't exists, skipping." << std::endl;
                simplemap[i - start_idx] = -1;
            }
        }
        if (filenames.size() == 0)
        {
            std::cout << "No file found, exiting." << std::endl;
            return 0;
        }
        cout << endl;
        cout << "Successfully load " << filenames.size() << " files into 3d Volume." << endl;
        cout << endl;
        Volume *vol = nullptr;
        try
        {
            vol = new Volume(filenames.size(), filenames, ifgray == 'y');
            my_garbage_collector.allocate(vol, 0, "volume");
        }
        catch (QuitProgram &q)
        {
            cout << q.what() << endl;
            return 0;
        }
        int w, h, n, c;
        w = vol->get_width();
        h = vol->get_height();
        n = vol->get_num();
        c = vol->get_channel();
        while (true)
        {
            bool breakflag = true;
            while (breakflag)
            {
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                cout << "\nWhat operation do you want to apply?" << endl;
                cout << "a: 3d guassian filter(fast version)" << endl;
                cout << "b: 3d guassian filter(memory efficient version)" << endl;
                cout << "c: 3d median filter" << endl;
                cout << "d: Maximum intensity projection" << endl;
                cout << "e: Minimum intensity projection" << endl;
                cout << "f: Mean intensity projection" << endl;
                cout << "g: Median intensity projection" << endl;
                cout << "h: Slice in the x-z plane" << endl;
                cout << "i: Slice in the y-z plane" << endl;
                char volume_op;
                cin >> volume_op;
                switch (volume_op)
                {

                case 'a':
                {
                    int min_dim = (h > w) ? w : h;
                    min_dim = (min_dim > n) ? n : min_dim;
                    int kernel_size = get_kernel_size(min_dim);
                    int pad = 0;
                    char ifpad;
                    get_padding_size(ifpad, pad);
                    if (ifpad == 'q')
                    {
                        return 0;
                    }
                    double sig = get_sig();
                    cout << "Applying fast 3d guassian filter" << endl;
                    Filter::gaussian_3d(vol, kernel_size, pad, sig, 0);
                    break;
                }
                case 'b':
                {
                    int min_dim = (h > w) ? w : h;
                    min_dim = (min_dim > n) ? n : min_dim;
                    int kernel_size = get_kernel_size(min_dim);
                    int pad = 0;
                    char ifpad;
                    get_padding_size(ifpad, pad);
                    if (ifpad == 'q')
                    {
                        return 0;
                    }
                    double sig = get_sig();
                    cout << "Applying small memory usage 3d guassian filter" << endl;
                    Filter::gaussian_3d(vol, kernel_size, pad, sig, 1);
                    break;
                }
                case 'c':
                {
                    int min_dim = (h > w) ? w : h;
                    min_dim = (min_dim > n) ? n : min_dim;
                    int kernel_size = get_kernel_size(min_dim);
                    int pad = 0;
                    char ifpad;
                    get_padding_size(ifpad, pad);
                    if (ifpad == 'q')
                    {
                        return 0;
                    }
                    cout << "Applying 3d median filter" << endl;
                    Filter::median_3d(vol, kernel_size, pad);
                    break;
                }
                case 'd':
                {
                    try
                    {
                        apply_projection("Applying maximum intensity projection", start_idx, end_idx, simplemap, vol, &max_func);
                    }
                    catch (QuitProgram &q)
                    {
                        return 0;
                    }
                    break;
                }
                case 'e':
                {
                    try
                    {
                        apply_projection("Applying Minimum intensity projection", start_idx, end_idx, simplemap, vol, &min_func);
                    }
                    catch (QuitProgram &q)
                    {
                        return 0;
                    }
                    break;
                }
                case 'f':
                {
                    try
                    {
                        apply_projection("Applying Mean intensity projection", start_idx, end_idx, simplemap, vol, &average_func);
                    }
                    catch (QuitProgram &q)
                    {
                        return 0;
                    }
                    break;
                }
                case 'g':
                {
                    try
                    {
                        apply_projection("Applying Median intensity projection", start_idx, end_idx, simplemap, vol, &median_func);
                    }
                    catch (QuitProgram &q)
                    {
                        return 0;
                    }
                    break;
                }
                case 'h':
                {
                    try
                    {
                        apply_slice('x', vol, h);
                    }
                    catch (QuitProgram &q)
                    {
                        return 0;
                    }
                    break;
                }
                case 'i':
                {
                    try
                    {
                        apply_slice('y', vol, w);
                    }
                    catch (QuitProgram &q)
                    {
                        return 0;
                    }
                    break;
                }
                case 'q':
                {
                    return 0;
                }
                default:
                    cout << "there aren't any options matching " << volume_op << " please choose again." << endl;
                }
            }
        }
    }
    return 0;
}
