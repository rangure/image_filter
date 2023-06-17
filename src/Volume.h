/**
 * @file Volume.h
 *
 * @brief This file implement the interface of Volume class.
 *
 * @author Warshall
 * Name             github
 * Gulam Nabi      edsml-gmn22
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
#include <vector>
#include <string>
class Volume
{
private:
    unsigned char *data;
    int width, height, channel, num;
    std::vector<std::string> mapping;

public:
    /**
     * @brief Construct a new Volume object
     * 
     * @param n 
     * @param filenames 
     * @param ifgray 
     */
    Volume(int n, std::vector<std::string> & filenames, bool ifgray);

    /**
     * @brief return the pixel index in the 1-d array given 4-d coordinates
     * 
     * @param h 
     * @param w 
     * @param c 
     * @param n 
     * @return int 
     */
    int pixel_index(int h, int w, int c, int n) const;

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
     * @brief Get the num object
     * 
     * @return int 
     */
    int get_num() const;


    /**
     * @brief return the if the given coordinate exist
     * 
     * @param x 
     * @param y 
     * @param c 
     * @param n 
     * @return true 
     * @return false 
     */
    bool valid_idx(int x,  int y, int c,  int n) const;

    /**
     * @brief return the data using  4-d coordinates
     * @param h y coordinate
     * @param w x coordinate
     * @param c channel
     * @param n which image
     * @return the data stored at the coordinate
     */
    unsigned char get_data_idx(int h, int w, int c, int n);

    /**
     * @brief set the data using 1-d coordinate
     * @param h index
     * @param val value to be set
     */
    void set_data_idx(int idx, int val);

    /**
     * @brief Get the data object
     * 
     * @return const unsigned char* 
     */
    const unsigned char *get_data() const;

    /**
     * @brief 
     * 
     */
    void replace_data(unsigned char *);
    ~Volume();
};
