#include "./../../myformat.cpp"

#include "./../Onegin/strfuncs.h"
#include "./../Onegin/io.h"

#include <stdio.h>


int main(int argc, char** argv) {

    FILE* outfl = {};
    FILE* inpfl = {};
    io_data ioflnms = {.inp = argv[1], .out = "cmd.asm"};
    opnfls(&inpfl, &outfl, ioflnms);

    filedata fldata = {.fl = inpfl};

    rdfrmfl(&fldata);


    //$s(fldata.rdbffr);
    for (size_t i = 0; i < fldata.nlns; i++) {

        $s(fldata.prsdbffr[i].str);
    }


    for (size_t i = 0; i < fldata.nlns; i++) {
//сделать перевод в циферки
        fwrite(fldata.prsdbffr[i].str, 1, fldata.prsdbffr[i].len, outfl);
        fwrite("\n", 1, 1, outfl);
    }


    clsfls(inpfl, outfl);
    filedatastrdestr(&fldata);

    return 0;
}
