/**
 * @file Slice.h
 *
 * @brief This file implement the interface of Slice class.
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
class Slice
{
private:
    /* sliced image, memory managed by this class*/
    Image *img;
    /* input volume, memory not managed by this class */
    Volume *vol;
    /* slicing direction */
    char dir;

public:
    /**
     * @brief Construct a new Slice object
     * @param Volume pointer
     * @param (char) direction of slice 'x' or 'y'
     * @param (int) position to slice at
     */
    Slice(Volume *, char, int);

    /**
     * @brief Function will fill an Image object with values from a 2D slice in the xz plane at a
        specified position.
     * @param (int) position of slice
     */
    void slice_xz(int);

    /**
     * @brief Function will fill an Image object with values from a 2D slice in the yz plane at a
        specified position.
     * @param (int) position of slice
     */
    void slice_yz(int);

    /**
     * @brief Get the img object
     * 
     * @return const Image* 
     */
    const Image *get_img() const;

    ~Slice();
};
