#include "./../Stack/stack.h"
#include "./../Onegin/strfuncs.h"
#include "enum.h"
#include <stdio.h>
#include <stdlib.h>

#define LOGFL stderr

#undef ERRLOG
#define ERRLOG(FRMT, ...) fprintf(LOGFL, FRMT, ##__VA_ARGS__)

//----------------------------------------------------------------------------------------------------------------

enum cmd_err {
    cmd_t_OK            = 0,
    cmd_t_NORMAL_EXIT   = 1,
    cmd_t_DIV_BY_ZERO   = 2,
    cmd_t_WRONG_CMD     = -1
};

//----------------------------------------------------------------------------------------------------------------

struct spu_t
{
    int*        cmdbuf;
    int         cmdbuflen;
    int         pc;
    stack_t*    stkptr;
    cmd_err     onbreak;
};

//--------------------//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

int spu(stack_t* stkptr, FILE* cmd_l);

int spu_push(spu_t* spu_t);

int spu_add(spu_t* spu_t);

int spu_sub(spu_t* spu_t);

int spu_div(spu_t* spu_t);

int spu_mul(spu_t* spu_t);

int spu_out(spu_t* spu_t);

int spu_halt(spu_t* spu_t);

typedef int spu_opfnc_t(spu_t* spu_t);

//----------------------------------------------------------------------------------------------------------------

spu_opfnc_t* cmd_opfunc_t [CMD_N__TOUCH_ONLY_ON_ADDING_NEW_COMMANDS_AND_NO_EXCEPT_BLYAT] =

// int (*cmd_l[CMD_N__TOUCH_ONLY_ON_ADDING_NEW_COMMANDS_AND_NO_EXCEPT_BLYAT])(spu_t*, stack_t*, cmd_err*) =
    {
        spu_halt,
        spu_push,
        spu_add,
        spu_sub,
        spu_div,
        spu_mul,
        spu_out,
    };

//----------------------------------------------------------------------------------------------------------------

spu_t parse_cmd(filedata filedata);

int read_filedata(filedata* filedata);

void spu_str_dtor(spu_t* spu_t);

void spu_dump(spu_t spu_t, str_t __file, int __line);

//----------------------------------------------------------------------------------------------------------------

int main(int argc, char** argv) {

    stack_t stk = {};
    STACK_CTOR(stk, 4);

    FILE* cmdlistfl = fopen(argv[1], "r");

    spu(&stk, cmdlistfl);

    fclose(cmdlistfl);

    STACK_DTOR(stk);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu(stack_t* stkptr, FILE* cmdlistfl) {

    filedata filedata = {.fl = cmdlistfl};
    read_filedata(&filedata);

    spu_t spu_t = parse_cmd(filedata);
    spu_t.stkptr = stkptr;

    while (!spu_t.onbreak) {

        spu_dump(spu_t, __FILE__, __LINE__);
        cmd_opfunc_t [spu_t.cmdbuf[spu_t.pc]] (&spu_t);
    }

    filedatastrdestr(&filedata);
    spu_str_dtor(&spu_t);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_push(spu_t* spu_t) {

    spu_t->pc++;

    int var = spu_t->cmdbuf[spu_t->pc];
    STACK_PUSH(spu_t->stkptr, var);

    spu_t->pc++;

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_add(spu_t* spu_t) {

    spu_t->pc++;

    int var1 = 0, var2 = 0;
    STACK_POP(spu_t->stkptr, &var1);
    STACK_POP(spu_t->stkptr, &var2);
    STACK_PUSH(spu_t->stkptr, var1 + var2);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_sub(spu_t* spu_t) {

    spu_t->pc++;

    int var1 = 0, var2 = 0;
    STACK_POP(spu_t->stkptr, &var1);
    STACK_POP(spu_t->stkptr, &var2);
    STACK_PUSH(spu_t->stkptr, var2 - var1);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_out(spu_t* spu_t) {

    spu_t->pc++;

    int var = 0;
    STACK_POP(spu_t->stkptr, &var);
    printf("%lg\n", 1.0 * var / SPU_EXPANENT_COEFF);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_mul(spu_t* spu_t) {

    spu_t->pc++;

    int var1 = 0, var2 = 0;
    STACK_POP(spu_t->stkptr, &var1);
    STACK_POP(spu_t->stkptr, &var2);
    STACK_PUSH(spu_t->stkptr, var2 * var1 / SPU_EXPANENT_COEFF);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_div(spu_t* spu_t) {

    spu_t->pc++;

    int var1 = 0, var2 = 0;
    STACK_POP(spu_t->stkptr, &var1);
    STACK_POP(spu_t->stkptr, &var2);
    if (var1 == 0) {

        ERRLOG(RED FAT "Division by zero.\n%s" DEF, "");
        spu_t->onbreak = cmd_t_DIV_BY_ZERO;
    }
    else {

        STACK_PUSH(spu_t->stkptr, var2 * 1000 / var1);
    }

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_halt(spu_t* spu_t) {

    spu_t->pc++;
    spu_t->onbreak = cmd_t_NORMAL_EXIT;

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

spu_t parse_cmd(filedata fldt){

    spu_t spu_t  = {.pc = 0, .onbreak = (cmd_err) 0};

    size_t buflen = (size_t) chrncnt(fldt.rdbffr, ' ', fldt.sz);
    spu_t.cmdbuflen = (int) buflen;

    int* tmp = (int*) calloc(buflen + 1, sizeof(int));
    int* cmd_buf = tmp;

    char* endptr = fldt.rdbffr;

    //$zu(buflen);

    for (size_t i = 0; i < buflen + 1; i++){

        //$s(endptr);
        cmd_buf[i] = (int) strtol(endptr, &endptr, 10);
        //$ad(cmd_buf, buflen);
    }

    spu_t.cmdbuf = cmd_buf;

    return spu_t;
}

//----------------------------------------------------------------------------------------------------------------

int read_filedata(filedata* filedata) {

    long long sz = rdflsz(filedata -> fl);

    if (sz == -1){

        return 1;
    }

    filedata->sz = (size_t) sz;
    filedata->rdbffr = (char*) calloc(filedata->sz + 1, 1);

    fread(filedata->rdbffr, sizeof(char), filedata->sz, filedata->fl);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

void spu_str_dtor(spu_t* spu_t) {

    free(spu_t->cmdbuf);
    spu_t->pc = -1;
    spu_t->stkptr = NULL;
    spu_t->onbreak = (cmd_err) 0;
};

//----------------------------------------------------------------------------------------------------------------

#undef FORMAT_LINE
#define FORMAT_LINE "-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n"
void spu_dump(spu_t spu_t, str_t __file, int __line) {

    ERRLOG("\n");
    ERRLOG(FORMAT_LINE);
    ERRLOG("\n");

    ERRLOG("SPU dump was called from " FAT"%s:%d.\n\n" DEF, __file, __line);
    ERRLOG("Command list:\n\n");

    for (int i = 0; i < spu_t.cmdbuflen; i++) {

        if (i == spu_t.pc) ERRLOG(FAT YELLOW);

        ERRLOG("%10d", i);
        ERRLOG(DEF);
    }

    ERRLOG("\n");

    for (int i = 0; i < spu_t.cmdbuflen; i++) {

        if (i == spu_t.pc) ERRLOG(FAT YELLOW);

        ERRLOG("%10d", spu_t.cmdbuf[i]);
        ERRLOG(DEF);
    }

    ERRLOG("\n");

    for (int i = 0; i < spu_t.pc + 1; i++) {

        ERRLOG("%10s", "");
    }

    ERRLOG(YELLOW FAT "^\n" DEF);

    ERRLOG("\n");
    ERRLOG(FORMAT_LINE);
    ERRLOG("\n");

    STACK_DUMP(spu_t.stkptr);
}
#undef FORMAT_LINE

//----------------------------------------------------------------------------------------------------------------
