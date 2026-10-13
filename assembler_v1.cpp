#include "./../../myformat.cpp"

#include "./../Onegin/strfuncs.h"
#include "./../Onegin/io.h"

#include "enum.h"


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

    int nread = 0;
    int useless = 0;

    if (!sscanf(str.str, "%d%n", &useless, &nread) && nread == str.len) {

        return true;
    }

    return false;
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

            *str_ptr = (char*) str + i;

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

        $s(fldata.prsdbffr[nln].str);
        if (is_onlyspace(fldata.prsdbffr[nln])) {

            $;
            continue;
        }

        string cmd_str = {}, arg_str = {};

        parse_asm(fldata, nln, &cmd_str, &arg_str);

        $s(cmd_str.str);
        $s(arg_str.str);

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

    int now_str_len = fldata.prsdbffr[nln].len;
    char* now_str_ptr = (char*) fldata.prsdbffr[nln].str;

    skipn_spaces(fldata.prsdbffr[nln].str, &now_str_ptr, now_str_len);

    now_str_len = now_str_len - (now_str_ptr - fldata.prsdbffr[nln].str);

    /// comment find
    const char* cmmnt_ptr = strchr(now_str_ptr, ';');
    if (cmmnt_ptr != NULL) {

        now_str_len = cmmnt_ptr - now_str_ptr + 1;
    }

    char* now_str = strndup(now_str_ptr, now_str_len); // NOTE: полная залупа, нахуй убрать

    //$s(now_str);

    now_str[now_str_len - 1] = '\0';

    now_str_ptr = strchr(now_str, ' ');
    //$s(now_str_ptr);
    int cmd_str_len = 0;
    int arg_str_len = 0;

    char* arg_str_str = now_str;

    if (now_str_ptr != NULL) {

        //$;
        cmd_str_len = now_str_ptr - now_str + 1;
        *now_str_ptr = '\0';

        //$s(now_str);

        arg_str_len = now_str_len - cmd_str_len;
        arg_str_str = now_str_ptr + 1;

        //$s(arg_str_str);
        //$std("Before skip spaces in arg\n");
        if (skipn_spaces(arg_str_str, &now_str_ptr, arg_str_len)) {

            //$;
            arg_str_len = arg_str_len - (now_str_ptr - arg_str_str);
            arg_str_str = now_str_ptr;
        }
        else {

            arg_str_str = now_str + now_str_len - 1;
            arg_str_len = 0;
        }

        //$s(arg_str_str);


    }
    else {

        cmd_str_len = now_str_len;
        arg_str_str = now_str + now_str_len - 1;
        arg_str_len = 0;
    }


    *cmd_str = {.str = now_str, .len = (size_t) cmd_str_len};
    *arg_str = {.str = arg_str_str, .len = (size_t) arg_str_len};

    cmd_str->str = strndup(now_str, cmd_str_len);
    arg_str->str = strndup(arg_str_str, cmd_str_len);

    free(now_str);
    return 0;
}


int cmd_def(string cmd_str) {

    int cmd_n_rd = 0;
    for (cmd_n_rd = 0; cmd_n_rd < CMD_N__TOUCH_ONLY_ON_ADDING_NEW_COMMANDS_AND_NO_EXCEPT_BLYAT && strncmp(cmd_str.str, cmd_l[cmd_n_rd].name, cmd_str.len); cmd_n_rd++) {}

    if (cmd_n_rd == CMD_N__TOUCH_ONLY_ON_ADDING_NEW_COMMANDS_AND_NO_EXCEPT_BLYAT) {

        return -1;
    }

    return cmd_l[cmd_n_rd].num;
}


int arg_def(string arg_str, double* arg_rd) {

    int nread = 0;
    // double arg_rd = NAN;

    sscanf(arg_str.str, "%lg%n", arg_rd, &nread);

    if ((nread == 0 && arg_str.len > 1) || is_onlyspace({.str = arg_str.str + nread, .len = arg_str.len - nread})) {

        return 1;
    }

    return 0;
}
