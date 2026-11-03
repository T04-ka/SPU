#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdio.h>


//-------------------------------------------------------------------------------------
struct string {
    char* str;
    int len;
};


//-------------------------------------------------------------------------------------
struct filedata {
    FILE* fl;
    size_t sz;
    int nlns;
    char *rdbffr;
    string *prsdbffr;
};


//-------------------------------------------------------------------------------------
struct io_data {
    const char* inp;
    const char* out;
};


#endif
