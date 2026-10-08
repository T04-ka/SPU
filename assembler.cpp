#include "./../../myformat.cpp"

#include "./../Onegin/strfuncs.h"
#include "./../Onegin/io.h"

#include <stdio.h>


int main(int argc, char** argv) {

    FILE* outfl = fopen("cmd.asm", "w");
    FILE* inpfl = fopen(argv[1], "r");

    filedata fldata = {};

    io_data = {.out = "cmd.asm", };


    fclose(inpfl);
    fclose(outfl);

    return 0;
}
