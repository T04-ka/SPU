#include "./../Stack/stack.h"
#include "./../Onegin/strfuncs.h"
#include "enum.h"
#include <algorithm>
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
    cmd_t_WRONG_CMD     = -1,
    cmd_t_WRONG_PC      = 3
};

//----------------------------------------------------------------------------------------------------------------

struct spu_t
{
    int*        cmdbuf;
    int         cmdbuflen;
    int         pc;
    stack_t    stk;
    cmd_err     onbreak;
    int*        regbuf;
};

//----------------------------------------------------------------------------------------------------------------

int spu_push(spu_t* spu_t);

int spu_add(spu_t* spu_t);

int spu_sub(spu_t* spu_t);

int spu_div(spu_t* spu_t);

int spu_mul(spu_t* spu_t);

int spu_out(spu_t* spu_t);

int spu_halt(spu_t* spu_t);

int spu_jump(spu_t* spu_t);

int spu_jump_if_bggr(spu_t* spu_t);

int spu_jump_if_smllr(spu_t* spu_t);

int spu_jump_if_eq(spu_t* spu_t);

int spu_jump_if_nbggr(spu_t* spu_t);

int spu_jump_if_nsmllr(spu_t* spu_t);

int spu_jump_if_neq(spu_t* spu_t);

int spu_meow(spu_t* spu_t);

//----------------------------------------------------------------------------------------------------------------

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
        spu_jump,
        spu_jump_if_bggr,
        spu_jump_if_smllr,
        spu_jump_if_eq,
        spu_jump_if_nbggr,
        spu_jump_if_nsmllr,
        spu_jump_if_neq,
        spu_meow
    };

//----------------------------------------------------------------------------------------------------------------

int spu_push(spu_t* spu_t) {

    if (spu_t->pc < 0 || spu_t->pc > spu_t->cmdbuflen) {

        spu_t->onbreak = cmd_t_WRONG_PC;
        return 1;
    }

    spu_t->pc++;

    int var = spu_t->cmdbuf[spu_t->pc];
    STACK_PUSH(&spu_t->stk, var);

    spu_t->pc++;

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_add(spu_t* spu_t) {

    spu_t->pc++;

    int var1 = 0, var2 = 0;
    STACK_POP(&spu_t->stk, &var1);
    STACK_POP(&spu_t->stk, &var2);
    STACK_PUSH(&spu_t->stk, var1 + var2);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_sub(spu_t* spu_t) {

    spu_t->pc++;

    int var1 = 0, var2 = 0;
    STACK_POP(&spu_t->stk, &var1);
    STACK_POP(&spu_t->stk, &var2);
    STACK_PUSH(&spu_t->stk, var2 - var1);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_out(spu_t* spu_t) {

    spu_t->pc++;

    int var = 0;
    STACK_POP(&spu_t->stk, &var);
    printf("%lg\n", 1.0 * var / SPU_EXPANENT_COEFF);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_mul(spu_t* spu_t) {

    spu_t->pc++;

    int var1 = 0, var2 = 0;
    STACK_POP(&spu_t->stk, &var1);
    STACK_POP(&spu_t->stk, &var2);
    STACK_PUSH(&spu_t->stk, var2 * var1 / SPU_EXPANENT_COEFF);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_div(spu_t* spu_t) {

    spu_t->pc++;

    int var1 = 0, var2 = 0;
    STACK_POP(&spu_t->stk, &var1);
    STACK_POP(&spu_t->stk, &var2);
    if (var1 == 0) {

        ERRLOG(RED FAT "Division by zero.\n%s" DEF, "");
        spu_t->onbreak = cmd_t_DIV_BY_ZERO;
    }
    else {

        STACK_PUSH(&spu_t->stk, var2 * 1000 / var1);
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

int spu_jump(spu_t* spu_t) {

    spu_t->pc = spu_t->cmdbuf [spu_t->pc + 1];

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_jump_if_bggr(spu_t* spu_t) {

    int frst_chr = 0, scnd_chr = 0;

    STACK_POP(&spu_t->stk, &scnd_chr);
    STACK_POP(&spu_t->stk, &frst_chr);

    if (scnd_chr > frst_chr) {

        spu_t->pc = spu_t->cmdbuf [spu_t->pc + 1];
    }
    else {

        spu_t->pc ++;
        spu_t->pc ++;
    }

    STACK_PUSH(&spu_t->stk, frst_chr);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_jump_if_smllr(spu_t* spu_t) {

    int frst_chr = 0, scnd_chr = 0;

    STACK_POP(&spu_t->stk, &scnd_chr);
    STACK_POP(&spu_t->stk, &frst_chr);

    if (scnd_chr < frst_chr) {

        spu_t->pc = spu_t->cmdbuf [spu_t->pc + 1];
    }
    else {

        spu_t->pc ++;
        spu_t->pc ++;
    }

    STACK_PUSH(&spu_t->stk, frst_chr);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_jump_if_eq(spu_t* spu_t) {

    int frst_chr = 0, scnd_chr = 0;

    STACK_POP(&spu_t->stk, &scnd_chr);
    STACK_POP(&spu_t->stk, &frst_chr);

    if (scnd_chr == frst_chr) {

        spu_t->pc = spu_t->cmdbuf [spu_t->pc + 1];
    }
    else {

        spu_t->pc ++;
        spu_t->pc ++;
    }

    STACK_PUSH(&spu_t->stk, frst_chr);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_jump_if_nbggr(spu_t* spu_t) {

    int frst_chr = 0, scnd_chr = 0;

    STACK_POP(&spu_t->stk, &scnd_chr);
    STACK_POP(&spu_t->stk, &frst_chr);

    if (scnd_chr <= frst_chr) {

        spu_t->pc = spu_t->cmdbuf [spu_t->pc + 1];
    }
    else {

        spu_t->pc ++;
        spu_t->pc ++;
    }

    STACK_PUSH(&spu_t->stk, frst_chr);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_jump_if_nsmllr(spu_t* spu_t) {

    int frst_chr = 0, scnd_chr = 0;

    STACK_POP(&spu_t->stk, &scnd_chr);
    STACK_POP(&spu_t->stk, &frst_chr);

    if (scnd_chr >= frst_chr) {

        spu_t->pc = spu_t->cmdbuf [spu_t->pc + 1];
    }
    else {

        spu_t->pc ++;
        spu_t->pc ++;
    }

    STACK_PUSH(&spu_t->stk, frst_chr);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_jump_if_neq(spu_t* spu_t) {

    int frst_chr = 0, scnd_chr = 0;

    STACK_POP(&spu_t->stk, &scnd_chr);
    STACK_POP(&spu_t->stk, &frst_chr);

    if (scnd_chr != frst_chr) {

        spu_t->pc = spu_t->cmdbuf [spu_t->pc + 1];
    }
    else {

        spu_t->pc ++;
        spu_t->pc ++;
    }

    STACK_PUSH(&spu_t->stk, frst_chr);

    return 0;
}

int spu_meow(spu_t* spu_t) {

    spu_t->pc ++;

    for (int i = 0; i < spu_t->cmdbuf [spu_t->pc] / SPU_EXPANENT_COEFF; i++) {

        printf("DED PIDORAS\n");
    }

    spu_t->pc ++;

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

void spu_dump(spu_t spu_t, str_t __file, int __line);

void spu_err_print(spu_t spu_t);

void spu_t_ctor(spu_t* spu_t, int argc, char** argv);

void spu_t_dtor(spu_t* spu_t);

//----------------------------------------------------------------------------------------------------------------

int main(int argc, char** argv) {

    spu_t spu_t = {};
    spu_t_ctor(&spu_t, argc, argv);

    while (!spu_t.onbreak) {

        //spu_dump(spu_t, __FILE__, __LINE__);
        cmd_opfunc_t [spu_t.cmdbuf[spu_t.pc]] (&spu_t);
    }

    spu_err_print(spu_t);

    spu_t_dtor(&spu_t);

    return 0;
}

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

    STACK_DUMP(&spu_t.stk);

    getchar();
}
#undef FORMAT_LINE

//----------------------------------------------------------------------------------------------------------------

void spu_err_print(spu_t spu_t) {

    switch (spu_t.onbreak) {

        case cmd_t_NORMAL_EXIT:
        {
            ERRLOG("SPU programm finished successfully.\n");
            break;
        }

        case cmd_t_DIV_BY_ZERO:
        {
            ERRLOG(RED FAT "Error: Division by zero.\n" DEF);
            break;
        }

        case cmd_t_WRONG_CMD:
        {
            ERRLOG(RED FAT "Error: Wrong cmd.\n" DEF);
            break;
        }

        case cmd_t_WRONG_PC:
        {
            ERRLOG(RED FAT "Error: Wrong pc.\n" DEF);
            break;
        }

        case cmd_t_OK:
        {
            ERRLOG(RED FAT "Error: This case can't be used.\n" DEF);
            break;
        }

        default: {}
    }
}

//----------------------------------------------------------------------------------------------------------------

void spu_t_ctor(spu_t* spu_t, int argc, char** argv) {

    STACK_CTOR(spu_t->stk, 4);

    spu_t->regbuf = (int*) calloc(5, sizeof(int));
    spu_t->cmdbuf = (int*) calloc(MAX_CMD_QTY, sizeof(int));

    FILE* cmd_list_fl = fopen(argv[1], "rb");

    if (cmd_list_fl == NULL) {

        ERRLOG(RED FAT "No such file.\n" DEF);
    }

    $p(spu_t->cmdbuf);
    fread(spu_t->cmdbuf, sizeof(int), MAX_CMD_QTY, cmd_list_fl);

    $ad(spu_t->cmdbuf, 20);

    fclose(cmd_list_fl);

    spu_t->pc = 0;
    spu_t->onbreak = (cmd_err) 0;
}

//----------------------------------------------------------------------------------------------------------------

void spu_t_dtor(spu_t* spu_t) {

    STACK_DTOR(spu_t->stk);
    free(spu_t->regbuf);
    free(spu_t->cmdbuf);
    spu_t->pc = -1;
    spu_t->onbreak = (cmd_err) 0;
}

//----------------------------------------------------------------------------------------------------------------
