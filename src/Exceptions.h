/**
 * @file Exceptions.h
 * 
 * @brief This file implement the unit tests.
 *
 * @author Warshall
 * Name             github
 * Gulam Nabi       edsml-gmn22
 * Yihang Zhou      acse-yz3522
 * Xingjian Qi      acse-xq322
 * Luisina Canto    edsml-lbc19
 * Hongyuan Liu     edsml-hl1222
 * Kexin Yan        acse-ky322
 * 
 * @version 0.1
 * @date 2023-03-23
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include <exception>
#include <string>
class QuitProgram : public std::exception
{
public:
    /**
     * @brief Return exception
     * 
     * @return const char* 
     */
    const char *what() const throw();
};

class InvalidPath : public std::exception
{
private:
    /**
     * @brief data
     * 
     */
    std::string msg;

public:
    /**
     * @brief InvalidPath(std::string);
     * 
     */
    const char *what() const throw();
};

class LoadFail : public std::exception
{
public:
    /**
     * @brief Return exception
     * 
     * @return const char* 
     */
    const char *what() const throw();
};

class BadBrightnessValue : public std::exception
{
public:
    /**
     * @brief Return exception
     * 
     * @return const char* 
     */
    const char *what() const throw();
};

class BadOption : public std::exception
{
public:
    /**
     * @brief Return exception
     * 
     * @return const char* 
     */
    const char *what() const throw();
};
