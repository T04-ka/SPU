#ifndef ENUM_H
#define ENUM_H

#include "./../../myformat.cpp"

#define SPU_EXPANENT_COEFF 1000

enum cmd_n {
    cmd_n_PSH = 1,
    cmd_n_ADD = 2,
    cmd_n_SUB = 3,
    cmd_n_DIV = 4,
    cmd_n_MUL = 5,
    cmd_n_OUT = 6,
    cmd_n_HLT = 0
};

const int CMD_N__TOUCH_ONLY_ON_ADDING_NEW_COMMANDS_AND_NO_EXCEPT_BLYAT = 7;

struct cmd_t
{
    str_t   name;
    cmd_n   num;
    int     argqty;
};


#define SET_CMD(CMD, ARGQTY) {#CMD, cmd_n_ ##CMD, ARGQTY}

const cmd_t cmd_l [CMD_N__TOUCH_ONLY_ON_ADDING_NEW_COMMANDS_AND_NO_EXCEPT_BLYAT] =
    {
        SET_CMD(HLT, 0),
        SET_CMD(PSH, 1),
        SET_CMD(ADD, 0),
        SET_CMD(SUB, 0),
        SET_CMD(DIV, 0),
        SET_CMD(MUL, 0),
        SET_CMD(OUT, 0)
    };

//#include "zhr_pdr.h"

#endif
