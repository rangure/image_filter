/**
 * @file MemManager.h
 *
 * @brief This file implement the interfate of MemManager class
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
#include <map>
#include <tuple>
#include <string>

/**
 * @brief this class offers functionalities to manage memory.
     The map stores the address of the allocated memroy and the type of the allocated memory.
     allocate should be called when new is called, and deallocate should be called when delete
     is called. however, this class is meant to be used when a there's exception and you need to quit
     the program with allocated memory. if there's no exception between new and delete this shouldn't 
     be used.
 * 
 */
class MemManager
{
private:

    /**
     * @brief map for the allocated memory. key is the address of the allocated memory,
     value is a tuple with first value indicating if the poninter is pointed to an array,
     the second value of the tuple is a string indicating the type of the pointer pointed to.
     * 
     */
    std::map<void *, std::tuple<bool, std::string>> addr;

public:

    /**
     * @brief Construct a new Mem Manager object
     * 
     */
    MemManager();
    /**
     * @brief Record the allocated memory to addr. key is the address of the allocated memory,
     * value is a tuple with first value indicating if the poninter is pointed to an array,
     * the second value of the tuple is a string indicating the type of the pointer pointed to.
     * This class only support int, double, char, unsigned char, Image, Volume, Slice and only support
     * array for
     * @param ptr void pointer whcih pointed to some allocated memory.
     * @param isarr true if the pointer is pointed to an array, false otherwise.
     * @param type type stored in the allocated memory by string.
     */
    void allocate(void *ptr, bool isarr, std::string type);
    /**
     * @brief remove the ptr from the map. this won't deallocate the memory pointed by the ptr
     * just remove it from the map.
     * @param ptr void pointer whcih pointed to some allocated memory.
     */
    void deallocate(void *ptr);
    /**
     * @brief deallocate all the memeory left in the map.
     */
    ~MemManager();
};