#ifndef ENUM_H
#define ENUM_H

#include "./../../myformat.cpp"


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
    str_t     name;
    cmd_n     n;
};


#define SET_CMD(CMD) {#CMD, cmd_n_ ##CMD}

const cmd_t cmd_l [CMD_N__TOUCH_ONLY_ON_ADDING_NEW_COMMANDS_AND_NO_EXCEPT_BLYAT] =
    {
        SET_CMD(HLT),
        SET_CMD(PSH),
        SET_CMD(ADD),
        SET_CMD(SUB),
        SET_CMD(DIV),
        SET_CMD(MUL),
        SET_CMD(OUT)
    };

//#include "zhr_pdr.h"

#endif
