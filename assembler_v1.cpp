#include "./../../myformat.cpp"

#include "./../Onegin/strfuncs.h"
#include "./../Onegin/io.h"

#include "enum.h"

#include <stdio.h>
#include <string.h>
#include <math.h>

void errlog(str_t flnm, int nln, str_t str, str_t err_msg);

bool is_onlyspace(string str);

/// Skips no more then n first spaces, or until '\0' is found
/// If all n symbols are spaces, return 0, else return 1 and put in "str_ptr" the pointer on first non-space symbol.
int skipn_spaces(const char* str, char* str_ptr, int n);



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


        int now_str_len = fldata.prsdbffr[i].len;
        char* now_str_ptr = (char*) fldata.prsdbffr[i].str;

        if (!skipn_spaces(fldata.prsdbffr[i].str, now_str_ptr, now_str_len)) continue;

        now_str_len = now_str_len - (now_str_ptr - fldata.prsdbffr[i].str);

        /// comment find
        const char* cmmnt_ptr = strchr(now_str_ptr, ';');
        if (cmmnt_ptr != NULL) {

            now_str_len = cmmnt_ptr - now_str_ptr + 1;
        }

        char* now_str = strndup(now_str_ptr, now_str_len);
        now_str[now_str_len - 1] = '\0';

        now_str_ptr = strchr(now_str, ' ');

        int cmd_str_len = 0;
        int arg_str_len = 0;

        char* arg_str_str = now_str;

        if (now_str_ptr != NULL) {

            cmd_str_len = now_str_ptr - now_str + 1;
            *now_str_ptr = '\0';

            arg_str_len = now_str_len - cmd_str_len;
            arg_str_str = now_str_ptr + 1;

            if (!skipn_spaces(arg_str_str, now_str_ptr, arg_str_len)) {

                arg_str_len = arg_str_len - (now_str_ptr - arg_str_str);
                arg_str_str = now_str_ptr;
            }
            else {

                arg_str_str = now_str + now_str_len - 1;
                arg_str_len = 0;
            }


        }
        else {

            cmd_str_len = now_str_len;
            arg_str_str = now_str + now_str_len - 1;
            arg_str_len = 0;
        }


        string cmd_str = {.str = now_str, .len = (size_t) cmd_str_len};
        string arg_str = {.str = arg_str_str, .len = (size_t) arg_str_len};

        for (int i = 0; i < CMD_N__TOUCH_ONLY_ON_ADDING_NEW_COMMANDS_AND_NO_EXCEPT_BLYAT; i++) {

            if (!strncmp(now_str, cmd_l[i].name, cmd_str_len)) {

            }
        }

        free(now_str);
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
