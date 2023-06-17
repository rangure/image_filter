/**
 * @file Image.h
 *
 * @brief This file implement the interface of Image class
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

class Image
{
private:
    unsigned char *data; /**< Double for the hue of the pixel, in degrees [0, 360]. */
    int width;
    int height;
    int channel; /**< Double for the hue of the pixel, in degrees [0, 360]. */

public:
    /**
     * Constructs a Image with the given data, the result image is a deep copy of the data.
     */
    Image(unsigned char *, int, int, int);
    /**
     * Constructs a blank Image with the given dimensions, the result image is a deep copy of the data.
     */
    Image(int, int, int);

    /**
     * @brief Function will (if valid) return the (int) index of the pixel in the position chosen.
    If inputs are not valid, will return -1.
     * 
     * @param h hieght
     * @param w width
     * @param c channel number
     * @return int 
     */
    int pixel_index(int h, int w, int c) const;

    /**
     * @brief Function takes 2 position inputs (x1,y1) and (x2,y2) in a certain channel, checks if they
    are valid positions (if not will return 0), and if valid will calculate and return the average
    pixel value of these two positions.
     * 
     * @param x1 
     * @param x2 
     * @param y1 
     * @param y2 
     * @param c 
     * @return double 
     */
    double average_by_channel(int x1, int x2, int y1, int y2, int c) const;

    /**
     * @brief Function returns the true if the index is valid false otherwise
     * 
     * @return true 
     * @return false 
     */
    bool valid_idx(int, int) const;

    /**
     * @brief Get the width object
     * 
     * @return int 
     */
    int get_width() const;

    /**
     * @brief Get the height object
     * 
     * @return int 
     */
    int get_height() const;

    /**
     * @brief Get the channel object
     * 
     * @return int 
     */
    int get_channel() const;

    /**
     * @brief Function returns a pointer to the data of size [width*height*channels] containing
    pixel values of the chosen image
     * 
     * @return const unsigned char* 
     */
    const unsigned char *get_data() const;

    /**
     * @brief Function deletes the current memory of the image and replaces it with the new data
     * @param (unsigned char *) array of new data
     */
    void replace_data(unsigned char *);

    /*Function that sets a value to a specific index in the data.
        Args: (int) index, (int) value */
    /**
     * @brief Set the data idx object
     * @param (int) index
     * @param (int) value
     */
    void set_data_idx(int, int);

    /**
     * @brief Get the data by idx object
     * @param (int) index
     * @return unsigned char 
     */
    unsigned char get_data_by_idx(int) const;

    /**
     * @brief Set the channel object
     * @param (int) new number of channels*
     */
    void set_channel(int);

    /**
     * @brief Function that will save the image to a specific filename within the directory chosen.
     * @param (char) filename
     * @return int 
     */
    int save_img(const char *) const;

    /**
     * @brief Destroy the Image object
     * 
     */
    ~Image();
};
