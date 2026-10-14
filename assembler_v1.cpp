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

int cmd_def(string cmd_str);

int arg_def(string arg_str, double* arg_rd);

#define BREAK_CILC  free(cmd_str.str); free(arg_str.str); break;


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

        string cmd_str = {}, arg_str = {};

        $s(fldata.prsdbffr[nln].str);

        sscanf(fldata.prsdbffr[nln].str, "%ms%n", &cmd_str.str, &cmd_str.len);
        sscanf(fldata.prsdbffr[nln].str + cmd_str.len, "%ms%n", &arg_str.str, &arg_str.len);

        skipn_spaces(fldata.prsdbffr[nln].str, &cmd_str_ptr , cmd_str.len);
        skipn_spaces(fldata.prsdbffr[nln].str + cmd_str.len, &arg_str_ptr , arg_str.len);

        arg_str.len -= (arg_str_ptr - fldata.prsdbffr[nln].str - cmd_str.len - 1);
        cmd_str.len -= (cmd_str_ptr - fldata.prsdbffr[nln].str - 1);

        $s(cmd_str.str);
        $d(cmd_str.len);
        $s(arg_str.str);
        $d(arg_str.len);

        //IF NO ARG - NULL
        int cmd_n_rd = cmd_def(cmd_str);

        $d(cmd_n_rd);

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

        free(cmd_str.str);
        free(arg_str.str);
    }


}

/// return cmd enum if cmd is correct and -1 if not.
int cmd_def(string cmd_str) {

    char* cmd_str_ptr = cmd_str.str;
    skipn_spaces(cmd_str.str, &cmd_str.str, cmd_str.len);

    int cmd_n_rd = 0;
    for (cmd_n_rd = 0; cmd_n_rd < CMD_N__TOUCH_ONLY_ON_ADDING_NEW_COMMANDS_AND_NO_EXCEPT_BLYAT && strncmp(cmd_str.str, cmd_l[cmd_n_rd].name, (size_t) cmd_str.len); cmd_n_rd++) {}

    if (cmd_n_rd == CMD_N__TOUCH_ONLY_ON_ADDING_NEW_COMMANDS_AND_NO_EXCEPT_BLYAT) {

        return -1;
    }

    return cmd_l[cmd_n_rd].num;
}
// nan + ret0 - emoty arg_str, nan + ret1 - wrong arg_str, dbl + ret0 - correct convert
/// return 0 if arg_str was empty and set arg_rd if double converted correctly, return 1 if was no convert (wrong arg_str)
int arg_def(string arg_str, double* arg_rd) {

    int nread = 0;
    // double arg_rd = NAN;

    if (arg_str.str == NULL) {

        return 0;
    }

    sscanf(arg_str.str, "%lg%n", arg_rd, &nread);
    //$d(nread);
    //$s(arg_str.str + nread);
    //$d(is_onlyspace({.str = arg_str.str + nread, .len = arg_str.len - nread}));
    if (nread == 0) {

        return 1;
    }

    return 0;
}
