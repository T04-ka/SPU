#include "./../../myformat.cpp"

#include "./../Onegin/strfuncs.h"
#include "./../Onegin/io.h"

#include "enum.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define CALC_EXPANENT_COEFF 1000


void errlog(str_t flnm, int nln, str_t str, str_t err_msg);

bool is_onlyspace(string str);

/// Skips no more then n first spaces, or until '\0' is found
/// If all n symbols are spaces, return 0, else return 1 and put in "str_ptr" the pointer on first non-space symbol.
int skipn_spaces(const char* str, char** str_ptr, int n);

void comlipe_lines(filedata fldata, io_data ioflnms, FILE* outfl);

int parse_asm(filedata fldata, int nln, string* cmd_str, string* arg_str);

int cmd_def(string cmd_str);

int arg_def(string arg_str, double* arg_rd);

#define BREAK_CILC  free((void*) cmd_str.str); free((void*) arg_str.str); break;


int main(int argc, char** argv) {

    FILE* outfl = {};
    FILE* inpfl = {};
    io_data ioflnms = {.inp = argv[1], .out = "byte.txt"};
    opnfls(&inpfl, &outfl, ioflnms);

    filedata fldata = {.fl = inpfl};

    rdfrmfl(&fldata);

    comlipe_lines(fldata, ioflnms, outfl);

    clsfls(inpfl, outfl);
    filedatastrdestr(&fldata); //YASHA YEBANIY PIDOR DOLBOEB, NE UMEET CHITAT` SLOVA NA ANGLIYSKOM

    return 0;
}


void errlog(str_t flnm, int nln, str_t str, str_t err_msg) {

    fprintf(stderr, FAT"%s:%d: " RED"Syntax error: " DEF"%s.\n", flnm, nln + 1, err_msg);
    fprintf(stderr, "%5d | %s\n%5s |\n", nln, str, "");
}


bool is_onlyspace(string str) {

    for (int i = 0; i < str.len - 1; i++) {

        if (!isspace(str.str[i])) {

            return false;
        }
    }

    return true;
}


/// Skips no more then n first spaces, or until '\0' is found
/// If all symbols are spaces, return 0, else return 1 and put in "str_ptr" the pointer on first non-space symbol.
int skipn_spaces(const char* str, char** str_ptr, int n) {

    //$std("-----------------------\n");
    //$s(str);
    int i = 0;
    for (i = 0; i < n && str[i] == ' ' && str[i] != '\0'; i++) {}

    //$d(i);

    if (i < n && str[i] != '\0') {

        if (str_ptr != NULL) {

            *str_ptr = (char*) (str + i);

            //$s((str_t) *str_ptr);
        }
        //$std("1-----------------------\n");
        return 1;
    }
    //$std("-----------------------\n");

    return 0;
}


void comlipe_lines(filedata fldata, io_data ioflnms, FILE* outfl) {

    for (int nln = 0; nln < fldata.nlns; nln++) {

        //$s(fldata.prsdbffr[nln].str);
        if (is_onlyspace(fldata.prsdbffr[nln])) {

            //$;
            continue;
        }



        char* cmd_str_ptr = {};
        char* arg_str_ptr = {};
        // $p(cmd_str_str);
        int  cmd_str_len = 0;
        int  arg_str_len = 0;

        string cmd_str = {}, arg_str = {};

        $s(fldata.prsdbffr[nln].str);

        sscanf(fldata.prsdbffr[nln].str, "%ms%n", &cmd_str.str, &cmd_str.len);
        sscanf(fldata.prsdbffr[nln].str + cmd_str.len, "%ms%n", &arg_str.str, &arg_str.len);


        skipn_spaces(fldata.prsdbffr[nln].str, &cmd_str_ptr , cmd_str.len);
        skipn_spaces(fldata.prsdbffr[nln].str + cmd_str.len, &arg_str_ptr , arg_str.len);

        $s(cmd_str_ptr);
        $d(cmd_str.len);
        $s(arg_str_ptr);
        $d(arg_str.len);

        $std("=======================================\n");
        //arg_str_str = fldata.prsdbffr[nln + cmd_str_len;


        arg_str.len = arg_str.len - (arg_str_ptr - fldata.prsdbffr[nln].str - cmd_str.len);
        cmd_str.len -= (cmd_str_ptr - fldata.prsdbffr[nln].str);



        $s(cmd_str.str);
        $d(cmd_str.len);
        $s(arg_str.str);
        $d(arg_str.len);

        //IF NO ARG - NULL

        free(cmd_str.str);
        free(arg_str.str);

        int cmd_n_rd = cmd_def(cmd_str);

        if (cmd_n_rd == -1) {

            errlog(ioflnms.inp, nln, fldata.prsdbffr[nln].str, "Wrong command given");
            BREAK_CILC;
        }

        double arg_rd = NAN;

        if (arg_def(arg_str, &arg_rd)) {

            errlog(ioflnms.inp, nln, fldata.prsdbffr[nln].str, "Wrong parametr given");
            BREAK_CILC;
        }

        if  (cmd_l[cmd_n_rd].argqty) {

            if (isnan(arg_rd)) {

                errlog(ioflnms.inp, nln, fldata.prsdbffr[nln].str, "Miss an argument");
                BREAK_CILC;
            }

            fprintf(outfl, "%d %d ", cmd_n_rd, (int) arg_rd * SPU_EXPANENT_COEFF);
        }
        else {

            if (!isnan(arg_rd)) {

                errlog(ioflnms.inp, nln, fldata.prsdbffr[nln].str, "An extra argument given");
                BREAK_CILC;
            }

            fprintf(outfl, "%d ", cmd_n_rd);
        }

        free((void*) cmd_str.str);
        free((void*) arg_str.str);
    }


}



int parse_asm(filedata fldata, int nln, string* cmd_str, string* arg_str) {



    return 0;
}


int cmd_def(string cmd_str) {
    char* cmd_str_ptr = cmd_str.str;
    skipn_spaces(cmd_str.str, &cmd_str.str, cmd_str.len);
    $s(cmd_str.str);
    int cmd_n_rd = 0;
    for (cmd_n_rd = 0; cmd_n_rd < CMD_N__TOUCH_ONLY_ON_ADDING_NEW_COMMANDS_AND_NO_EXCEPT_BLYAT && strncmp(cmd_str.str, cmd_l[cmd_n_rd].name, (size_t) cmd_str.len); cmd_n_rd++) {}

    if (cmd_n_rd == CMD_N__TOUCH_ONLY_ON_ADDING_NEW_COMMANDS_AND_NO_EXCEPT_BLYAT) {

        return -1;
    }

    return cmd_l[cmd_n_rd].num;
}


int arg_def(string arg_str, double* arg_rd) {

    int nread = 0;
    // double arg_rd = NAN;

    sscanf(arg_str.str, "%lg%n", arg_rd, &nread);
    $d(nread);
    $s(arg_str.str + nread);
    $d(is_onlyspace({.str = arg_str.str + nread, .len = arg_str.len - nread}));
    if ((nread == 0 && arg_str.len > 1) || !is_onlyspace({.str = arg_str.str + nread, .len = arg_str.len - nread})) {

        return 1;
    }

    return 0;
}
