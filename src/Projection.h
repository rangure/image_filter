/**
 * @file Projection.h
 *
 * @brief This file implement the interface of Projection class
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
#include "Volume.h"
#include "Image.h"

class Projection
{
private:
    /* data */
public:
    Projection(/* args */);
    ~Projection();

    /**
     * @brief Function that acts on a subset of the volume data (determined by the start and end point 
    input with a function that is input (func). It generates a 2D by projecting the resulting
    values from the function onto the xy plane
     * @param pointer to the Volume of data
     * @param (int) start of projection
     * @param (int) end of projection
     * @param (double) function to act on volume with (func)
     * @return Image* 
     */
    static Image *project(Volume *, int, int, double (*)(unsigned char*, int));
};
