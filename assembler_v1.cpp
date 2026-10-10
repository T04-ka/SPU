#include "./../../myformat.cpp"

#include "./../Onegin/strfuncs.h"
#include "./../Onegin/io.h"

#include "enum.h"

#include <stdio.h>
#include <string.h>


void errlog(str_t flnm, int nln, str_t str, str_t err_msg);

bool is_onlyspace(string str);



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

        if (is_onlyspace(fldata.prsdbffr[i])) continue;
//
//         char* cmdstr = {};
//         int cmdstrlen = 0;
//         char* argstr = {};
//         int argstrlen = 0;

        string cmdstr = {};
        string argstr = {};

        sscanf(fldata.prsdbffr[i].str, "%s%n%s%n", (char*) cmdstr.str, (int*) &cmdstr.len, (char*) argstr.str, () &argstr.len);

        for (int i = 0; i < CMD_N__TOUCH_ONLY_ON_ADDING_NEW_COMMANDS_AND_NO_EXCEPT_BLYAT; i++) {

            if (!strcmp(cmdstr.str, cmd_l[i].name)) {

                if (is_onlyspace(argstr)) {


                }
            }
        }
    }

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

                fprintf(outfl, "1 %d ", (int) (SPU_EXPANENT_COEFF * var));
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
    filedatastrdestr(&fldata); //YASHA YEBANIY PIDOR DOLBOEB, NE UMEET CHITAT` SLOVA NA ANGLIYSKOM


    #define CALC_EXPANENT_COEFF 1000




    return 0;
}

#undef ERRLOG
#undef CHECK_WRITE_CMD


void errlog(str_t flnm, int nln, str_t str, str_t err_msg) {

    fprintf(stderr, FAT"%s:%d: " RED"Syntax error: " DEF"%s.\n", flnm, nln, err_msg);
    fprintf(stderr, "%5d | %s\n%5s |\n", nln, str, "");
}


bool is_onlyspace(string str) {

    int nread = 0;
    int useless = 0;

    if (!sscanf(str.str, "%d%n", &useless, &nread) && nread == str.len) {

        return true;
    }

    return false;
}
