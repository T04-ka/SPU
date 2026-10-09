#include "./../../myformat.cpp"

#include "./../Onegin/strfuncs.h"
#include "./../Onegin/io.h"

#include <stdio.h>
#include <string.h>


str_t const cmd_PSH = "PSH";
str_t const cmd_ADD = "ADD";
str_t const cmd_SUB = "SUB";
str_t const cmd_DIV = "DIV";
str_t const cmd_OUT = "OUT";
str_t const cmd_HLT = "HLT";
str_t const cmd_DMP = "DMP";
str_t const cmd_MUL = "MUL";

enum cmd_n {
    cmd_n_PSH = 1,
    cmd_n_ADD = 2,
    cmd_n_SUB = 3,
    cmd_n_DIV = 4,
    cmd_n_MUL = 5,
    cmd_n_OUT = 6,
    cmd_n_DMP = 7,
    cmd_n_HLT = 0
};


void errlog(str_t flnm, int nln, str_t str, str_t err_msg);


#undef CHECK_WRITE_CMD
#define CHECK_WRITE_CMD(CMD)                            \
    if (!strcmp(cmd_##CMD, fldata.prsdbffr[i].str)) {   \
                                                        \
        fprintf(outfl, "%d ", (int) cmd_n_##CMD);       \
        continue;                                       \
    }

int main(int argc, char** argv) {

    FILE* outfl = {};
    FILE* inpfl = {};
    io_data ioflnms = {.inp = argv[1], .out = "byte.txt"};
    opnfls(&inpfl, &outfl, ioflnms);

    filedata fldata = {.fl = inpfl};

    rdfrmfl(&fldata);


    for (int i = 0; i < fldata.nlns; i++) {

        if (fldata.prsdbffr[i].len > 4) {

            char cmd[4] = {};
            strncpy(cmd, fldata.prsdbffr[i].str, 3);
            cmd[3] = '\0';

            if (!strcmp(cmd_PSH, cmd)) {


                const char* endptr = fldata.prsdbffr[i].str + 4;

                double var = strtod(fldata.prsdbffr[i].str + 3, (char**) &endptr);

                if (endptr != fldata.prsdbffr[i].str + fldata.prsdbffr[i].len - 1) {

                    errlog(ioflnms.inp, i, fldata.prsdbffr[i].str, "Wrong PUSH parametr given");
                    break;
                }

                fprintf(outfl, "1 %d ", (int) (1000 * var));
            }
            else {

                errlog(ioflnms.inp, i, fldata.prsdbffr[i].str, "Wrong command given");
                break;
            }

            continue;
        }

        CHECK_WRITE_CMD(ADD);

        CHECK_WRITE_CMD(SUB);

        CHECK_WRITE_CMD(DIV);

        CHECK_WRITE_CMD(MUL);

        CHECK_WRITE_CMD(OUT);

        CHECK_WRITE_CMD(DMP);

        CHECK_WRITE_CMD(HLT);

        errlog(ioflnms.inp, i, fldata.prsdbffr[i].str, "Wrong command given");

        return 1;
    }

    clsfls(inpfl, outfl);
    filedatastrdestr(&fldata);

    return 0;
}

#undef ERRLOG
#undef CHECK_WRITE_CMD




void errlog(str_t flnm, int nln, str_t str, str_t err_msg) {

    fprintf(stderr, FAT"%s:%d: " RED"Syntax error: " DEF"%s.\n", flnm, nln, err_msg);
    fprintf(stderr, "%5d | %s\n%5s |\n", nln, str, "");
}
