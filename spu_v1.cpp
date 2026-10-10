#include "./../Stack/stack.h"
#include "./../Onegin/strfuncs.h"
#include "./../Onegin/io.h"
#include "enum.h"
#include <stdio.h>

#define SPU_EXPANENT_COEFF 1000

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

struct spu_str
{
    int*        cmdbuf;
    int         cmdbuflen;
    int         pc;
    stack_t*    stkptr;
    cmd_err     onbreak;
};

//--------------------//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

int spu(stack_t* stkptr, FILE* cmd_l);

int spu_push(spu_str* spu_str);

int spu_add(spu_str* spu_str);

int spu_sub(spu_str* spu_str);

int spu_div(spu_str* spu_str);

int spu_mul(spu_str* spu_str);

int spu_out(spu_str* spu_str);

int spu_halt(spu_str* spu_str);

typedef int spu_opfnc_t(spu_str* spu_str);

//----------------------------------------------------------------------------------------------------------------

spu_opfnc_t* cmd_opfunc_t [CMD_N__TOUCH_ONLY_ON_ADDING_NEW_COMMANDS_AND_NO_EXCEPT_BLYAT] =

// int (*cmd_l[CMD_N__TOUCH_ONLY_ON_ADDING_NEW_COMMANDS_AND_NO_EXCEPT_BLYAT])(spu_str*, stack_t*, cmd_err*) =
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

spu_str parse_cmd(filedata filedata);

int read_filedata(filedata* filedata);

void spu_str_dtor(spu_str* spu_str);

void spu_dump(spu_str spu_str, str_t __file, int __line);

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

    spu_str spu_str = parse_cmd(filedata);
    spu_str.stkptr = stkptr;

    while (!spu_str.onbreak) {

        spu_dump(spu_str, __FILE__, __LINE__);
        cmd_opfunc_t [spu_str.cmdbuf[spu_str.pc]] (&spu_str);
    }

    filedatastrdestr(&filedata);
    spu_str_dtor(&spu_str);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_push(spu_str* spu_str) {

    spu_str->pc++;

    int var = spu_str->cmdbuf[spu_str->pc];
    STACK_PUSH(spu_str->stkptr, var);

    spu_str->pc++;

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_add(spu_str* spu_str) {

    spu_str->pc++;

    int var1 = 0, var2 = 0;
    STACK_POP(spu_str->stkptr, &var1);
    STACK_POP(spu_str->stkptr, &var2);
    STACK_PUSH(spu_str->stkptr, var1 + var2);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_sub(spu_str* spu_str) {

    spu_str->pc++;

    int var1 = 0, var2 = 0;
    STACK_POP(spu_str->stkptr, &var1);
    STACK_POP(spu_str->stkptr, &var2);
    STACK_PUSH(spu_str->stkptr, var2 - var1);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_out(spu_str* spu_str) {

    spu_str->pc++;

    int var = 0;
    STACK_POP(spu_str->stkptr, &var);
    printf("%lg\n", var / 1000.0);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_mul(spu_str* spu_str) {

    spu_str->pc++;

    int var1 = 0, var2 = 0;
    STACK_POP(spu_str->stkptr, &var1);
    STACK_POP(spu_str->stkptr, &var2);
    STACK_PUSH(spu_str->stkptr, var2 * var1 / 1000);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_div(spu_str* spu_str) {

    spu_str->pc++;

    int var1 = 0, var2 = 0;
    STACK_POP(spu_str->stkptr, &var1);
    STACK_POP(spu_str->stkptr, &var2);
    if (var1 == 0) {

        ERRLOG(RED FAT "Division by zero.\n%s" DEF, "");
        spu_str->onbreak = cmd_t_DIV_BY_ZERO;
    }
    else {

        STACK_PUSH(spu_str->stkptr, var2 * 1000 / var1);
    }

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_halt(spu_str* spu_str) {

    spu_str->pc++;
    spu_str->onbreak = cmd_t_NORMAL_EXIT;

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

spu_str parse_cmd(filedata fldt){

    spu_str spu_str  = {.pc = 0, .onbreak = (cmd_err) 0};

    size_t buflen = chrncnt(fldt.rdbffr, ' ', fldt.sz);
    spu_str.cmdbuflen = buflen;

    int* tmp = (int*) calloc(buflen + 1, sizeof(int));
    int* cmd_buf = tmp;

    char* endptr = fldt.rdbffr;

    //$zu(buflen);

    for (size_t i = 0; i < buflen + 1; i++){

        //$s(endptr);
        cmd_buf[i] = (int) strtol(endptr, &endptr, 10);
        //$ad(cmd_buf, buflen);
    }

    spu_str.cmdbuf = cmd_buf;

    return spu_str;
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

void spu_str_dtor(spu_str* spu_str) {

    free(spu_str->cmdbuf);
    spu_str->pc = -1;
    spu_str->stkptr = NULL;
    spu_str->onbreak = (cmd_err) 0;
};

//----------------------------------------------------------------------------------------------------------------

#undef FORMAT_LINE
#define FORMAT_LINE "-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n"
void spu_dump(spu_str spu_str, str_t __file, int __line) {

    ERRLOG("\n");
    ERRLOG(FORMAT_LINE);
    ERRLOG("\n");

    ERRLOG("SPU dump was called from " FAT"%s:%d.\n\n" DEF, __file, __line);
    ERRLOG("Command list:\n\n");

    for (int i = 0; i < spu_str.cmdbuflen; i++) {

        if (i == spu_str.pc) ERRLOG(FAT YELLOW);

        ERRLOG("%10d", i);
        ERRLOG(DEF);
    }

    ERRLOG("\n");

    for (int i = 0; i < spu_str.cmdbuflen; i++) {

        if (i == spu_str.pc) ERRLOG(FAT YELLOW);

        ERRLOG("%10d", spu_str.cmdbuf[i]);
        ERRLOG(DEF);
    }

    ERRLOG("\n");

    for (int i = 0; i < spu_str.pc + 1; i++) {

        ERRLOG("%10s", "");
    }

    ERRLOG(YELLOW FAT "^\n" DEF);

    ERRLOG("\n");
    ERRLOG(FORMAT_LINE);
    ERRLOG("\n");

    STACK_DUMP(spu_str.stkptr);
}
#undef FORMAT_LINE

//----------------------------------------------------------------------------------------------------------------
