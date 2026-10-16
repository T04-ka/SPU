#include "./../../myformat.cpp"

#include "./../Onegin/strfuncs.h"
#include "./../Onegin/io.h"

#include "./../Stack/stack.h"

#include "enum.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

//----------------------------------------------------------------------------------------------------------------

#define CALC_EXPANENT_COEFF 1000
#define MAX_TAG_QTY 1000

//----------------------------------------------------------------------------------------------------------------

struct asm_data_t
{
    stack_t     code_buf;
    int*        tag_buf;
    FILE*       inp_fl;
    FILE*       asm_lst_fl;
    FILE*       bin_fl;
    str_t       inpfl_nm, asm_lst_fl_nm, bin_fl_nm;
};

//----------------------------------------------------------------------------------------------------------------

enum asm_err_t
{
    asm_err_NO_ERR      = 0,
    asm_err_WRONG_TAG   = 1,
    asm_err_WRONG_CMD   = 2,
    asm_err_WRONG_ARG   = 3,
    asm_err_MISS_TAG    = 4,
    asm_err_MISS_ARG    = 5,
    asm_err_ARG_QTY_EXC = 6,
    asm_err_TAG_VAL_EXC = 7,
    asm_err_TAG_NO_EXST = 8,
    asm_err_TAG_QTY_EXC = 9
};

enum arg_def_ret_t
{
    arg_def_ret_OK          = 0,
    arg_def_ret_WRONG_ARG   = 1,
    arg_def_ret_TAG         = -1
};

//----------------------------------------------------------------------------------------------------------------

void errlog(str_t flnm, int nln, str_t str, str_t err_msg);

bool is_onlyspace(string str);

/// Skips no more then n first spaces, or until '\0' is found
/// If all n symbols are spaces, return 0, else return 1 and put in "str_ptr" the pointer on first non-space symbol.
int skipn_spaces(const char* str, const char** str_ptr, int n);

asm_err_t compile_lines(bool on_write, filedata fldata, asm_data_t* asm_data_ptr);

int cmd_def(string cmd_str);

arg_def_ret_t arg_def(string arg_str, double* arg_rd);

int return_data(asm_data_t asm_data);

int asm_data_ctor(asm_data_t* asm_data_ptr);

int asm_data_dtor(asm_data_t* asm_data_ptr);

//----------------------------------------------------------------------------------------------------------------

int main(int argc, char** argv) {

    asm_data_t asm_fl_data = {.inpfl_nm = argv[1], .asm_lst_fl_nm = "asmlist.txt", .bin_fl_nm = "code.bin"};
    asm_data_ctor(&asm_fl_data);

    if (asm_fl_data.inp_fl == NULL) {

        fprintf(stderr, "No such file.\n");
        asm_data_dtor(&asm_fl_data);
        return 1;
    }

    filedata fldata = {.fl = asm_fl_data.inp_fl};
    rdfrmfl(&fldata);

    asm_err_t compile_ok = compile_lines(false, fldata, &asm_fl_data);

    //$ad(asm_fl_data.tag_buf, 10);

    if (!compile_ok) {

        compile_lines(true, fldata, &asm_fl_data);
    }
    else {

        //asm_err_handle();
    }

    return_data(asm_fl_data);

    asm_data_dtor(&asm_fl_data);
    filedatastrdestr(&fldata); //YASHA YEBANIY PIDOR DOLBOEB, NE UMEET CHITAT` SLOVA NA ANGLIYSKOM

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

void errlog(str_t flnm, int nln, str_t str, str_t err_msg) {

    fprintf(stderr, FAT"%s:%d: " RED"Syntax error: " DEF"%s.\n", flnm, nln + 1, err_msg);
    fprintf(stderr, "%5d | %s\n%5s |\n", nln, str, "");
}

//----------------------------------------------------------------------------------------------------------------

bool is_onlyspace(string str) {

    for (int i = 0; i < str.len - 1; i++) {

        if (!isspace(str.str[i])) {

            return false;
        }
    }

    return true;
}

//----------------------------------------------------------------------------------------------------------------

/// Skips no more then n first spaces, or until '\0' is found
/// If all symbols are spaces, return 0, else return 1 and put in "str_ptr" the pointer on first non-space symbol.
int skipn_spaces(const char* str, const char** str_ptr, int n) {

    int i = 0;
    for (i = 0; i < n && str[i] == ' ' && str[i] != '\0'; i++) {}

    if (i < n && str[i] != '\0') {

        if (str_ptr != NULL) {

            *str_ptr = str + i;
        }

        return 1;
    }

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

//TODO: указатели
//TODO: писать данные в массив
#define CONTINUE_CICL  free(current_asm_t.cmd_str.str); free(current_asm_t.arg_str.str); free(current_asm_t.tag_str.str); continue;

#define ON_RETURN(TP)                                                                      \
    free(current_asm_t.cmd_str.str);                                                      \
    free(current_asm_t.arg_str.str);                                                      \
    free(current_asm_t.tag_str.str);                                                      \
    return TP;

//----------------------------------------------------------------------------------------------------------------

struct current_asm_t {

    char *cmd_str_ptr = {}, *arg_str_ptr = {}, *tag_str_ptr = {};
    string cmd_str = {}, arg_str = {}, tag_str = {};
    int cmd_str_def_len = 0, arg_str_def_len = 0, tag_str_def_len = 0;

};


int parse_line(filedata fldata, int nln, current_asm_t* current_asm_t);

asm_err_t compile_lines(bool on_write, filedata fldata, asm_data_t* asm_data_ptr) {

    for (int nln = 0; nln < fldata.nlns; nln++) {

        //$s(fldata.prsdbffr[nln].str);
        if (is_onlyspace(fldata.prsdbffr[nln])) {

            //$;
            continue;
        }

        current_asm_t current_asm_t = {};

        parse_line(fldata, nln, &current_asm_t);

        // $s(cmd_str.str);
        // $d(cmd_str.len);
        // $s(arg_str.str);
        // $d(arg_str.len);
        // $s(tag_str.str);
        // $d(tag_str.len);


        int cmd_n_rd = cmd_def(current_asm_t.cmd_str);

        if (cmd_n_rd == CMD_N__TOUCH_ONLY_ON_ADDING_NEW_COMMANDS_AND_NO_EXCEPT_BLYAT) {

            ON_RETURN(asm_err_WRONG_CMD);
        }

        double arg_rd = NAN;
        int arg_def_ret = arg_def(current_asm_t.arg_str, &arg_rd);

        if (cmd_n_rd == -1) { //: 7

            if (arg_def_ret){

                ON_RETURN(asm_err_WRONG_TAG);
            }

            if (isnan(arg_rd)) {

                ON_RETURN(asm_err_MISS_TAG);
            }

            if (!is_onlyspace({.str = fldata.prsdbffr[nln].str + current_asm_t.cmd_str_def_len + current_asm_t.arg_str_def_len, .len = fldata.prsdbffr[nln].len - current_asm_t.cmd_str_def_len - current_asm_t.arg_str_def_len})) {

                ON_RETURN(asm_err_ARG_QTY_EXC);
            }

            if ((int) arg_rd > MAX_TAG_QTY) {

                ON_RETURN(asm_err_TAG_VAL_EXC);
            }
            //МЕТКА УСТАНАВЛИВАЕТСЯ НА ЗНАЧЕНИЕ ПС, СЛЕДУЮЩЕЕ ПОСЛЕ МЕТКИ

            $lg(arg_rd);

            asm_data_ptr->tag_buf[(int) arg_rd] = (int) asm_data_ptr->code_buf.sz;

            CONTINUE_CICL;
        }

         if (arg_def_ret == 1) {

             ON_RETURN(asm_err_WRONG_ARG);
         }

        if (cmd_l[cmd_n_rd].tag) {

            if (arg_def_ret == -1) { //значит тут метка JUMP : 7

                double tag_rd = NAN;
                int tag_def_ret = arg_def(current_asm_t.tag_str, &tag_rd);

                if (tag_def_ret) {

                    ON_RETURN(asm_err_WRONG_TAG);
                }

                if (isnan(tag_rd)) {

                    ON_RETURN(asm_err_MISS_TAG);
                }


                if (!is_onlyspace({.str = fldata.prsdbffr[nln].str + current_asm_t.cmd_str_def_len + current_asm_t.arg_str_def_len + current_asm_t.tag_str_def_len, \
                                   .len = fldata.prsdbffr[nln].len - current_asm_t.cmd_str_def_len - current_asm_t.arg_str_def_len - current_asm_t.tag_str_def_len})) {

                    ON_RETURN(asm_err_ARG_QTY_EXC);
                }

                if (!on_write && asm_data_ptr->tag_buf[(int) tag_rd] == -1) {

                    ON_RETURN(asm_err_TAG_NO_EXST);
                }
                if (on_write) {

                    STACK_PUSH(&asm_data_ptr->code_buf, cmd_n_rd);
                    STACK_PUSH(&asm_data_ptr->code_buf, asm_data_ptr->tag_buf[(int) tag_rd]);
                }
                CONTINUE_CICL;
                //fprintf(outfl, "%d %d ", cmd_n_rd, (int) (arg_rd * SPU_EXPANENT_COEFF));
            }
            else {

                ON_RETURN(asm_err_MISS_TAG);
            }
        }

        if (arg_def_ret == -1) {

            ON_RETURN(asm_err_TAG_QTY_EXC)
        }

        if  (cmd_l[cmd_n_rd].argqty) {

            if (isnan(arg_rd)) {

                ON_RETURN(asm_err_MISS_ARG);
            }


            if (!is_onlyspace({.str = fldata.prsdbffr[nln].str + current_asm_t.cmd_str_def_len + current_asm_t.arg_str_def_len, \
                               .len = fldata.prsdbffr[nln].len - current_asm_t.cmd_str_def_len - current_asm_t.arg_str_def_len})) {

                ON_RETURN(asm_err_ARG_QTY_EXC);
            }

            if (on_write) {
                STACK_PUSH(&asm_data_ptr->code_buf, cmd_n_rd);
                STACK_PUSH(&asm_data_ptr->code_buf, (int) (arg_rd * SPU_EXPANENT_COEFF));
            }

            CONTINUE_CICL;
            //fprintf(outfl, "%d %d ", cmd_n_rd, (int) (arg_rd * SPU_EXPANENT_COEFF));
        }
        else {

            if (!isnan(arg_rd)) {

                ON_RETURN(asm_err_ARG_QTY_EXC);
            }

            if (on_write) {

                STACK_PUSH(&asm_data_ptr->code_buf, cmd_n_rd);
            }

            // fprintf(outfl, "%d ", cmd_n_rd);
        }

        free(current_asm_t.cmd_str.str);
        free(current_asm_t.arg_str.str);
        free(current_asm_t.tag_str.str);
    }

    return asm_err_NO_ERR;
}


int parse_line(filedata fldata, int nln, current_asm_t* current_asm_t) {

    sscanf(fldata.prsdbffr[nln].str, "%ms%n", &current_asm_t->cmd_str.str, &current_asm_t->cmd_str_def_len);
    sscanf(fldata.prsdbffr[nln].str + current_asm_t->cmd_str_def_len, "%ms%n", &current_asm_t->arg_str.str, &current_asm_t->arg_str_def_len);
    sscanf(fldata.prsdbffr[nln].str + current_asm_t->cmd_str_def_len + current_asm_t->arg_str_def_len, "%ms%n", &current_asm_t->tag_str.str, &current_asm_t->tag_str_def_len);

    skipn_spaces(fldata.prsdbffr[nln].str, (const char**) &current_asm_t->cmd_str_ptr , current_asm_t->cmd_str_def_len);
    skipn_spaces(fldata.prsdbffr[nln].str + current_asm_t->cmd_str_def_len, (const char**) &current_asm_t->arg_str_ptr , current_asm_t->arg_str_def_len);
    skipn_spaces(fldata.prsdbffr[nln].str + current_asm_t->cmd_str_def_len + current_asm_t->arg_str_def_len, (const char**) &current_asm_t->tag_str_ptr, current_asm_t->tag_str_def_len);


    current_asm_t->tag_str.len = current_asm_t->tag_str_def_len - (int) (current_asm_t->tag_str_ptr - fldata.prsdbffr[nln].str - current_asm_t->cmd_str_def_len - current_asm_t->arg_str_def_len - 1);
    current_asm_t->arg_str.len = current_asm_t->arg_str_def_len - (int) (current_asm_t->arg_str_ptr - fldata.prsdbffr[nln].str - current_asm_t->cmd_str_def_len - 1);
    current_asm_t->cmd_str.len = current_asm_t->cmd_str_def_len - (int) (current_asm_t->cmd_str_ptr - fldata.prsdbffr[nln].str - 1);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

/// return cmd enum if cmd is correct and -1 if not.
int cmd_def(string cmd_str) {

    skipn_spaces(cmd_str.str, (const char**) &cmd_str.str, cmd_str.len);

    int cmd_n_rd = 0;

    if (!strncmp(cmd_str.str, ":", (size_t) cmd_str.len)) {

        return -1;
    }

    for (cmd_n_rd = 0; cmd_n_rd < CMD_N__TOUCH_ONLY_ON_ADDING_NEW_COMMANDS_AND_NO_EXCEPT_BLYAT && strncmp(cmd_str.str, cmd_l[cmd_n_rd].name, (size_t) cmd_str.len); cmd_n_rd++) {}

    if (cmd_n_rd == CMD_N__TOUCH_ONLY_ON_ADDING_NEW_COMMANDS_AND_NO_EXCEPT_BLYAT) {

        return CMD_N__TOUCH_ONLY_ON_ADDING_NEW_COMMANDS_AND_NO_EXCEPT_BLYAT;
    }

    return cmd_l[cmd_n_rd].num;
}

//----------------------------------------------------------------------------------------------------------------

// nan + ret0 - emoty arg_str, nan + ret1 - wrong arg_str, dbl + ret0 - correct convert
/// return 0 if arg_str was empty and set arg_rd if double converted correctly, return 1 if was no convert (wrong arg_str)
/// return -1 if argement is tag
arg_def_ret_t arg_def(string arg_str, double* arg_rd) {

    int nread = 0;
    // double arg_rd = NAN;

    if (arg_str.str == NULL) {

        return arg_def_ret_OK;
    }

    if (!strncmp(arg_str.str, ":", (size_t) arg_str.len)) {

        return arg_def_ret_TAG;
    }

    sscanf(arg_str.str, "%lg%n", arg_rd, &nread);
    //$d(nread);
    //$s(arg_str.str + nread);
    //$d(is_onlyspace({.str = arg_str.str + nread, .len = arg_str.len - nread}));
    if (nread == 0) {

        return arg_def_ret_WRONG_ARG;
    }

    return arg_def_ret_OK;
}

//----------------------------------------------------------------------------------------------------------------

int return_data(asm_data_t asm_data) {

    fwrite(asm_data.code_buf.bffr, sizeof(int), asm_data.code_buf.cpty, asm_data.bin_fl);

    for (size_t pc = 0; pc < asm_data.code_buf.sz; pc++) {

        fprintf(asm_data.asm_lst_fl, "%d ", asm_data.code_buf.bffr[pc]);
    }

    return 0;
}

//----------------------------------------------------------------------------------------------------------------
//TODO СДЕЛАТЬ ОБРАБТКУ ОШИБОК
int asm_data_ctor(asm_data_t* asm_data_ptr) {

    STACK_CTOR(asm_data_ptr->code_buf, 4);

    asm_data_ptr->tag_buf = (int*) calloc(MAX_TAG_QTY, sizeof(int));

    for (int i = 0; i < MAX_TAG_QTY; i++){

        asm_data_ptr->tag_buf[i] = -1;
    }

    asm_data_ptr->inp_fl      = fopen(asm_data_ptr->inpfl_nm, "r");
    asm_data_ptr->asm_lst_fl  = fopen(asm_data_ptr->asm_lst_fl_nm, "w");
    asm_data_ptr->bin_fl      = fopen(asm_data_ptr->bin_fl_nm, "wb");

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

//TODO СДЕЛАТЬ ОБРАБТКУ ОШИБОК
int asm_data_dtor(asm_data_t* asm_data_ptr) {

    STACK_DTOR(asm_data_ptr->code_buf);

    free(asm_data_ptr->tag_buf);

    fclose(asm_data_ptr->inp_fl);
    fclose(asm_data_ptr->asm_lst_fl);
    fclose(asm_data_ptr->bin_fl);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------
