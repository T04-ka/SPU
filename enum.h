#ifndef ENUM_H
#define ENUM_H

#include "./../../myformat.cpp"

#define SPU_EXPANENT_COEFF 1000

#define MAX_CMD_QTY 1000

enum cmd_n
{
    cmd_n_HALT = 0,
    cmd_n_PUSH = 1,
    cmd_n_ADD = 2,
    cmd_n_SUB = 3,
    cmd_n_DIV = 4,
    cmd_n_MUL = 5,
    cmd_n_OUT = 6,
    cmd_n_JUMP = 7,
    cmd_n_JMPG = 8,
    cmd_n_JMPL = 9,
    cmd_n_JMPE = 10,
    cmd_n_JMPNG = 11,
    cmd_n_JMPNL = 12,
    cmd_n_JMPNE = 13,
    cmd_n_CALL = 14,
    cmd_n_RET = 15,
    cmd_n_PUSHR = 16,
    cmd_n_POPR = 17,
};

const int CMD_N__TOUCH_ONLY_ON_ADDING_NEW_COMMANDS_AND_NO_EXCEPT_BLYAT = 18;

struct cmd_t
{
    str_t   name;
    cmd_n   num;
    int     argqty;
    bool    tag;
};


#define SET_CMD(CMD, ARGQTY, TAG) {#CMD, cmd_n_ ##CMD, ARGQTY, TAG}

const cmd_t cmd_l [CMD_N__TOUCH_ONLY_ON_ADDING_NEW_COMMANDS_AND_NO_EXCEPT_BLYAT] =
    {
        SET_CMD(HALT, 0, 0),
        SET_CMD(PUSH, 1, 0),
        SET_CMD(ADD, 0, 0),
        SET_CMD(SUB, 0, 0),
        SET_CMD(DIV, 0, 0),
        SET_CMD(MUL, 0, 0),
        SET_CMD(OUT, 0, 0),
        SET_CMD(JUMP, 0, 1),
        SET_CMD(JMPG, 0, 1),
        SET_CMD(JMPL, 0, 1),
        SET_CMD(JMPE, 0, 1),
        SET_CMD(JMPNG, 0, 1),
        SET_CMD(JMPNL, 0, 1),
        SET_CMD(JMPNE, 0, 1),
        SET_CMD(CALL, 0, 1),
        SET_CMD(RET, 0, 0),
        SET_CMD(PUSHR, 1, 0),
        SET_CMD(POPR, 1, 0)
    };

//#include "zhr_pdr.h"

#endif
