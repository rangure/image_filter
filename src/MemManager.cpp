/**
 * @file MemManager.cpp
 *
 * @brief This file implement the functionalites in MemManager class.
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
#include "MemManager.h"
#include <iostream>
#include "Image.h"
#include "Volume.h"
#include "Slice.h"
MemManager::MemManager(/* args */)
{
}

MemManager::~MemManager()
{
    for(auto const&i : addr)
    {
        std::string s = std::get<1>(i.second);
        void * ptr = i.first;
        std::cout<<"Deallocating "<<s<<" stored at "<<ptr<<std::endl;  
        if(std::get<0>(i.second))
        {
            if(s == "int")
            {
                delete[] (int*)ptr;
            }
            else if (s == "double")
            {
                delete[] (double*)ptr;
            }
            else if( s== "char")
            {
                delete[] (char*)ptr;
            }
            else if( s== "unsigned char")
            {
                delete[] (unsigned char*)ptr;
            }
            else
            {
                std::cout<<"there is no array type "<<s<<" supported by MemManager"<<std::endl;
            }
        }
        else
        {
            if(s == "int")
            {
                delete (int*)ptr;
            }
            else if (s == "double")
            {
                delete (double*)ptr;
            }
            else if( s== "char")
            {
                delete (char*)ptr;
            }
            else if( s== "unsigned char")
            {
                delete (unsigned char*)ptr;
            }
            else if( s== "img")
            {
                delete (Image*)ptr;
            }
            else if( s== "volume")
            {
                delete (Volume*)ptr;
            }
            else if(s=="slice")
            {
                delete (Slice* )ptr;
            }
            else
            {
                std::cout<<"there is no type "<<s<<" supported by MemManager"<<std::endl;
            }
        }
    }   
}

void MemManager::allocate(void *ptr, bool isarr, std::string type)
{

    addr[ptr] = std::tuple<bool, std::string>(isarr, type);
}

void MemManager::deallocate(void *ptr)
{
    
    std::map<void *, std::tuple<bool, std::string>>::iterator it=addr.find(ptr);
    if(it == addr.end())
    {
        std::cout<<"memory location doesn't exist!"<<std::endl;
        exit(1);
    } 
    addr.erase(ptr);
}