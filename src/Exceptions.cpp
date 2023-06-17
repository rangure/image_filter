/**
 * @file Exception.cpp
 *
 * @brief This file implement the functionalties of all exception classes
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
 #include"Exceptions.h"
 #include<string>
const char *QuitProgram::what() const throw()
{
        return "Quiting!\n";
}
const char *InvalidPath::what() const throw()
{
        // return ("Path "+msg+" doesn't exist, input again.").c_str();
        return nullptr;
}
const char *LoadFail::what() const throw()
{
        return "Load fail, quitting";
}

const char *BadBrightnessValue::what() const throw()
{
        return "invalid brigntness value, please enter value between 255 and -255";
}

const char* BadOption::what() const throw()
{
    return "invalid option value, please enter again.";
}