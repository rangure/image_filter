/**
 * @file Slice.cpp
 *
 * @brief This file implement the functionalties of Slice class
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
#include "Slice.h"
#include <iostream>
#include "Exceptions.h"
using namespace std;
Slice::Slice(Volume* volp, char dirction, int pos)
{
    //get data dimensions and channel information
    int dim_x, dim_y, dim_z, channel;
    dim_x = volp->get_width();
    dim_y = volp->get_height();
    dim_z = volp->get_num();
    channel = volp->get_channel();
     //set volume pointer and direciton from input
    vol = volp;
    dir = dirction;
    if(dir=='x')
    {
        //create new image consisting of x and z plane
        //check if out of range
        if (pos<0 || pos >= dim_y)
        {
            cout<<"invalid slicing position"<<endl;
            throw QuitProgram{};
        }
        img = new Image(dim_x, dim_z, channel);
        //slice in the xz plane using function
        slice_xz(pos);
    }
    else if (dir=='y')
    {
        if (pos<0 || pos >= dim_x)
        {
             //check for valid position of slice
            cout<<"invalid slicing position"<<endl;
            throw QuitProgram{};
            
        }
        //creat a new image +splice to get yz plane
        img = new Image(dim_y, dim_z, channel);
        slice_yz(pos);
    }
    else
    {
        //invalid input
        cout<<"Undefined slice behavior"<<endl;
        throw QuitProgram{};
        
    }
    
}
void Slice::slice_xz(int pos)
{
    // get image data
    int dim_x, dim_y, dim_z, channel;
    dim_x = vol->get_width();
    dim_y = vol->get_height();
    dim_z = vol->get_num();
    channel = vol->get_channel();
    int idx, val;
     //loop through every pixel in xz plane for the specified position
    for(int i=0; i<dim_z;i++)
    {
        for(int j =0; j<dim_x; j++)
        {
             //loop thorugh channels
            for(int k=0; k<channel; k++)
            {
                //set index and value of (slice) pixel to 2D Image
                val = vol->get_data_idx(pos,j,k,i);
                idx = img->pixel_index(i,j,k);
                img->set_data_idx(idx,val);
            }
        }
    }
}
void Slice::slice_yz(int pos)
{
    //get data
    int dim_x, dim_y, dim_z, channel;
    dim_x = vol->get_width();
    dim_y = vol->get_height();
    dim_z = vol->get_num();
    channel = vol->get_channel();
    int idx, val;
    //loop through z and y plane for all channels for the specified position
    for(int i=0; i<dim_z;i++)
    {
        for(int j =0; j<dim_y; j++)
        {
            for(int k=0; k<channel; k++)
            {
                //set index and value of (slice) pixel to 2D Image
                val = vol->get_data_idx(j,pos,k,i);
                idx = img->pixel_index(i,j,k);
                img->set_data_idx(idx,val);
            }
        }
    }
}
const Image* Slice::get_img() const
{
    return img;
}
Slice::~Slice()
{
    delete img;
}