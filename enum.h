#ifndef ENUM_H
#define ENUM_H

//----------------------------------------------------------------------------------------------------------------

#include "./../../myformat.cpp"

//----------------------------------------------------------------------------------------------------------------

#define SPU_EXPANENT_COEFF 100

#define ARGSZ 4
#define CMDSZ 1
#define SIGNA_SIZE 8
#define ADMIN_DATA_SIZE 4

//----------------------------------------------------------------------------------------------------------------

const int MEMORY_SIZE__NE_TROGAI_EGO_ONO_TEBYA_SOJRET = 1048576;

//----------------------------------------------------------------------------------------------------------------

// const char SIGNATURE___TOUCH_ONLY_IF_YOU_DONT_AFRAID_OF_THE_CONSEQUENSES[] = {'Y', 'A', 'S', 'H', 'A', '4', 'M', 'O'};
const unsigned long SIGNATURE___TOUCH_ONLY_IF_YOU_DONT_AFRAID_OF_THE_CONSEQUENSES[] = {0x4f4d344148534159};

//----------------------------------------------------------------------------------------------------------------

enum cmd_n
{
    cmd_n_ERROR     = -2,
    cmd_n_LABEL     = -1,
    cmd_n_HALT      = 0,
    cmd_n_PUSH      = 1,
    cmd_n_ADD       = 2,
    cmd_n_SUB       = 3,
    cmd_n_DIV       = 4,
    cmd_n_MUL       = 5,
    cmd_n_OUT       = 6,
    cmd_n_JUMP      = 7,
    cmd_n_JMPG      = 8,
    cmd_n_JMPL      = 9,
    cmd_n_JMPE      = 10,
    cmd_n_JMPLE     = 11,
    cmd_n_JMPGE     = 12,
    cmd_n_JMPNE     = 13,
    cmd_n_CALL      = 14,
    cmd_n_RET       = 15,
    cmd_n_PUSHR     = 16,
    cmd_n_POPR      = 17,
    cmd_n_PUSHM     = 18,
    cmd_n_POPM      = 19,
    cmd_n_POP       = 20,
    cmd_n_PUSHMR    = 21,
    cmd_n_POPMR     = 22,
    cmd_n_IN        = 23,
    cmd_n_SQRT      = 24,
    cmd_n_DRAW      = 25,
};

const int CMD_N__TOUCH_ONLY_ON_ADDING_NEW_COMMANDS_AND_NO_EXCEPT_BLYAT = 28;

//----------------------------------------------------------------------------------------------------------------

struct cmd_t
{
    str_t   name;
    cmd_n   num;
    int     argqty;
    bool    label;
};

//----------------------------------------------------------------------------------------------------------------

#define SET_CMD(CMD, ARGQTY, TAG) {#CMD, cmd_n_ ##CMD, ARGQTY, TAG}
const cmd_t CMD_L [CMD_N__TOUCH_ONLY_ON_ADDING_NEW_COMMANDS_AND_NO_EXCEPT_BLYAT - 2] =
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
        SET_CMD(JMPLE, 0, 1),
        SET_CMD(JMPGE, 0, 1),
        SET_CMD(JMPNE, 0, 1),
        SET_CMD(CALL, 0, 1),
        SET_CMD(RET, 0, 0),
        SET_CMD(PUSHR, 1, 0),
        SET_CMD(POPR, 1, 0),
        SET_CMD(PUSHM, 1, 0),
        SET_CMD(POPM, 1, 0),
        SET_CMD(POP, 1, 0),
        SET_CMD(PUSHMR, 1, 0),
        SET_CMD(POPMR, 1, 0),
        SET_CMD(IN, 0, 0),
        SET_CMD(SQRT, 0, 0),
        SET_CMD(DRAW, 0, 0),
    };
#undef SET_CMD

//----------------------------------------------------------------------------------------------------------------

const int REG_N__NEVER_TOUCH_IT_AT_ALL_BLYAT = 9;
enum reg_n
{
    RAX     = 0,
    RBX     = 1,
    RCX     = 2,
    RDDLX   = 3,
    REX     = 4,
    RMFI    = 5,
    RMDSZ   = 6,
    RDW     = 7,
    RDH     = 8,
};

const char * const REGISTER_LIST[REG_N__NEVER_TOUCH_IT_AT_ALL_BLYAT] =
    {
        "RAX",
        "RBX",
        "RCX",
        "RDDLX",
        "REX",
        "RMFI",
        "RMDSZ",
	    "RDW",
		"RDH",
    };

//----------------------------------------------------------------------------------------------------------------

#endif
