// uno_project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//Yoana Mladenova fn4MI0600631

#include <iostream>
#include <algorithm> 
#include <random>    
#include <ctime> 

int streq(const char* a, const char* b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i])
            return 0;
        i++;
    }
    return a[i] == b[i];
}

void strcp(char* dest, const char* src) {
    int i = 0;
    while (src[i] != '\0')
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

void strconcat(char* dest, const char* src) {
    int i = 0;
    while (dest[i] != '\0') {
        i++;
    }
    int j = 0;
    while (src[j] != '\0') {
        dest[i] = src[j];
        i++;
        j++;
    }

    dest[i] = '\0';
}

int main()
{
   
}


