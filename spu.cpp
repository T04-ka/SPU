
#define DEBUG
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./../Stack/Stack_int/stack.h"
#include "enum.h"

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
    void*       cmd_buf;
    size_t      cmd_buf_size;
    size_t      cmd_qty;
    int         pc;
    stack_t     stk;
    cmd_err     onbreak;
    int*        reg_buf;
    stack_t     calltree;
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

int spu_call(spu_t* spu_t);

int spu_ret(spu_t* spu_t);

int spu_pushr(spu_t* spu_t);

int spu_popr(spu_t* spu_t);

//----------------------------------------------------------------------------------------------------------------

typedef int spu_opfnc_t(spu_t* spu_t);

//----------------------------------------------------------------------------------------------------------------

spu_opfnc_t* cmd_opfnc_t [CMD_N__TOUCH_ONLY_ON_ADDING_NEW_COMMANDS_AND_NO_EXCEPT_BLYAT - 2] =

// int (*CMD_L[CMD_N__TOUCH_ONLY_ON_ADDING_NEW_COMMANDS_AND_NO_EXCEPT_BLYAT])(spu_t*, stack_t*, cmd_err*) =
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
        spu_call,
        spu_ret,
        spu_pushr,
        spu_popr
    };



void cmd_opfnc_t_init() {


    // cmd_opfnc_t[0] = spu_ret;
}

#define GET_CMD(IND)    * ( (char*) spu_t->cmd_buf + IND )
#define GET_ARG(IND)    * (unsigned int*)( (char*) spu_t->cmd_buf + IND )
#define PC_INCR         spu_t->pc += CMDSZ

//----------------------------------------------------------------------------------------------------------------

int spu_push(spu_t* spu_t) {

//     if (spu_t->pc < 0 || spu_t->pc > spu_t->cmd_buflen) {
//
//         spu_t->onbreak = cmd_t_WRONG_PC;
//         return 1;
//     }

    PC_INCR;
    // сделать через memcpy
    // $du(( *(unsigned int*)((char*) spu_t->cmd_buf + spu_t->pc) ));

    int copied_arg = 0;
    memcpy(&copied_arg, &GET_ARG(spu_t->pc), 4);

    int var = copied_arg;
    STACK_PUSH(&spu_t->stk, var);

    spu_t->pc += ARGSZ;

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_add(spu_t* spu_t) {

    PC_INCR;

    int var1 = 0, var2 = 0;
    STACK_POP(&spu_t->stk, &var1);
    STACK_POP(&spu_t->stk, &var2);
    STACK_PUSH(&spu_t->stk, var1 + var2);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_sub(spu_t* spu_t) {

    PC_INCR;

    int var1 = 0, var2 = 0;
    STACK_POP(&spu_t->stk, &var1);
    STACK_POP(&spu_t->stk, &var2);
    STACK_PUSH(&spu_t->stk, var2 - var1);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_out(spu_t* spu_t) {

    PC_INCR;

    int var = 0;
    STACK_POP(&spu_t->stk, &var);
    printf("%lg\n", 1.0 * var / SPU_EXPANENT_COEFF);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_mul(spu_t* spu_t) {

    PC_INCR;

    int var1 = 0, var2 = 0;
    STACK_POP(&spu_t->stk, &var1);
    STACK_POP(&spu_t->stk, &var2);
    STACK_PUSH(&spu_t->stk, var2 * var1 / SPU_EXPANENT_COEFF);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_div(spu_t* spu_t) {

    PC_INCR;

    int var1 = 0, var2 = 0;
    STACK_POP(&spu_t->stk, &var1);
    STACK_POP(&spu_t->stk, &var2);
    if (var1 == 0) {

        ERRLOG(RED FAT "Division by zero.\n%s" DEF, "");
        spu_t->onbreak = cmd_t_DIV_BY_ZERO;
    }
    else {

        STACK_PUSH(&spu_t->stk, var2 * SPU_EXPANENT_COEFF / var1);
    }

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_halt(spu_t* spu_t) {

    PC_INCR;
    spu_t->onbreak = cmd_t_NORMAL_EXIT;

    return 0;
}

//----------------------------------------------------------------------------------------------------------------
//УЧЕСТЬ ЧТО МЕТКА В 1000 раз больше
int spu_jump(spu_t* spu_t) {

    int copied_arg = 0;
    memcpy(&copied_arg, &GET_ARG(spu_t->pc + CMDSZ), 4);
    spu_t->pc = copied_arg / SPU_EXPANENT_COEFF; // VOT TUT NAVERNOE

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

#define JUMP_IF(CMP)                                                    \
int frst_chr = 0, scnd_chr = 0;                                         \
                                                                        \
STACK_POP(&spu_t->stk, &scnd_chr);                                      \
STACK_POP(&spu_t->stk, &frst_chr);                                      \
                                                                        \
if (scnd_chr CMP frst_chr) {                                            \
                                                                        \
                                                                        \
    int copied_arg = 0;                                                 \
    memcpy(&copied_arg, &GET_ARG(spu_t->pc + CMDSZ), 4);                \
    spu_t->pc = copied_arg / SPU_EXPANENT_COEFF;                        \
}                                                                       \
else {                                                                  \
                                                                        \
    PC_INCR;                                                            \
    spu_t->pc += ARGSZ;                                                 \
}                                                                       \
                                                                        \
STACK_PUSH(&spu_t->stk, frst_chr);



int spu_jump_if_bggr(spu_t* spu_t) {

    JUMP_IF(>);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_jump_if_smllr(spu_t* spu_t) {

    JUMP_IF(<);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_jump_if_eq(spu_t* spu_t) {

    JUMP_IF(==);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_jump_if_nbggr(spu_t* spu_t) {

    JUMP_IF(<=);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_jump_if_nsmllr(spu_t* spu_t) {

    JUMP_IF(>=);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_jump_if_neq(spu_t* spu_t) {

    JUMP_IF(!=);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_call(spu_t* spu_t) {

    STACK_PUSH(&spu_t->calltree, spu_t->pc + CMDSZ + ARGSZ);

    int copied_arg = 0;
    memcpy(&copied_arg, &GET_ARG(spu_t->pc + CMDSZ), 4);
    spu_t->pc = copied_arg;

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_ret(spu_t* spu_t) {

    STACK_POP(&spu_t->calltree, &spu_t->pc);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_pushr(spu_t* spu_t) {

    PC_INCR;
    // $d(GET_ARG(spu_t->pc));
    int copied_arg = 0;
    memcpy(&copied_arg, &GET_ARG(spu_t->pc), 4);
    STACK_PUSH(&spu_t->stk, spu_t->reg_buf[copied_arg / SPU_EXPANENT_COEFF]);

    spu_t->pc += ARGSZ;

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_popr(spu_t* spu_t) {

    PC_INCR;
    // $d(GET_ARG(spu_t->pc));
    int copied_arg = 0;
    memcpy(&copied_arg, &GET_ARG(spu_t->pc), 4);
    STACK_POP(&spu_t->stk, &spu_t->reg_buf[copied_arg / SPU_EXPANENT_COEFF]);

    spu_t->pc += ARGSZ;

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

void spu_dump(spu_t spu_t, str_t __file, int __line);

void spu_err_print(spu_t spu_t);

int spu_t_ctor(spu_t* spu_t, int argc, char** argv);

void spu_t_dtor(spu_t* spu_t);

void cmd_opfnc_t_init();

//----------------------------------------------------------------------------------------------------------------

int main(int argc, char** argv) {

    cmd_opfnc_t_init();

    spu_t spu_t = {};
    spu_t_ctor(&spu_t, argc, argv);

    while (!spu_t.onbreak) {

        // $d(spu_t.pc);
        // spu_dump(spu_t, __FILE__, __LINE__);
        cmd_opfnc_t [* (char*)( (char*) spu_t.cmd_buf + spu_t.pc )] (&spu_t);
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
//
//     ERRLOG("SPU dump was called from " FAT"%s:%d.\n\n" DEF, __file, __line);
//
    $ad(spu_t.reg_buf, 5);
//
//     ERRLOG("Command list:\n\n");
//
//     int portlen = 20;
//
//     for (int p = 0; p < spu_t.cmd_buflen / portlen + 1; p ++) {
//
//         for (int i = p * portlen; i < portlen * (p + 1); i++) {
//
//             if (i == spu_t.pc) ERRLOG(FAT YELLOW);
//
//             if (i > spu_t.cmd_buflen) ERRLOG(BLACK);
//
//             ERRLOG(FAT "%9d", i);
//             ERRLOG(DEF);
//         }
//
//         ERRLOG("\n");
//
//         for (int i = p * portlen; i < portlen * (p + 1); i++) {
//
//             if (i == spu_t.pc) ERRLOG(FAT YELLOW);
//
//             if (i > spu_t.cmd_buflen) ERRLOG(BLACK);
//
//             ERRLOG("%9d", spu_t.cmd_buf[i]);
//             ERRLOG(DEF);
//         }
//
//         ERRLOG("\n");
//
//         if (spu_t.pc >= p * portlen && spu_t.pc < (p + 1) * portlen) {
//
//             for (int i = 0; i < spu_t.pc % portlen + 1; i++) {
//
//                 ERRLOG("%9s", "");
//             }
//
//             ERRLOG(YELLOW FAT "^\n" DEF);
//         }
//
//         ERRLOG("\n");
//     }

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

int spu_t_ctor(spu_t* spu_t, int argc, char** argv) {

    STACK_CTOR(spu_t->stk, 4);
    STACK_CTOR(spu_t->calltree, 4);

    spu_t->reg_buf = (int*) calloc(5, sizeof(int));
    // spu_t->cmd_buf = (int*) calloc(MAX_CMD_QTY, sizeof(int));

    FILE* cmd_list_fl = fopen(argv[1], "rb");

    if (cmd_list_fl == NULL) {

        ERRLOG(RED FAT "No such file.\n" DEF);
    }

    void* signa_arr = calloc(1, 8 + 2 * 4);

    fread(signa_arr, 16, 1, cmd_list_fl);

    if (*(unsigned long*) signa_arr != *SIGNATURE___TOUCH_ONLY_IF_YOU_DONT_AFRAID_OF_THE_CONSEQUENSES) {

        fprintf(stderr, RED FAT "CHE ZA GOVNO TI MNE PODSUNUL? IDI COMPILIRUI NORMAL'NO. PIDOR.\n" DEF);
        return 1;
    }
    // $d(*((int*) signa_arr + 3));
    // $ad(spu_t->cmd_buf, 40);

//     for (int i = 0; i < 8; i++) {
//
//         putchar(*((char*) signa_arr + i));
//     }
//
//     $sn;

    spu_t->cmd_qty = (size_t) *( (char*) signa_arr + 2 * 4 );
    spu_t->cmd_buf_size = (size_t) *((int*) signa_arr + 3);


    // printf("Number of cmd = %zu\n", spu_t->cmd_qty);
    // printf("All = %zu\n", spu_t->cmd_buf_size);

    spu_t->cmd_buf = calloc(spu_t->cmd_buf_size, 1);
    fread(spu_t->cmd_buf, 1, spu_t->cmd_buf_size, cmd_list_fl);

    // $ad(((char*) spu_t->cmd_buf), spu_t->cmd_buf_size);

    free(signa_arr);
    fclose(cmd_list_fl);

    spu_t->pc = 0;
    spu_t->onbreak = (cmd_err) 0;

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

void spu_t_dtor(spu_t* spu_t) {

    STACK_DTOR(spu_t->stk);
    STACK_DTOR(spu_t->calltree);
    free(spu_t->reg_buf);
    free(spu_t->cmd_buf);
    spu_t->pc = -1;
    spu_t->cmd_buf_size = -1;
    spu_t->cmd_qty = -1;
    spu_t->onbreak = (cmd_err) 0;
}

//----------------------------------------------------------------------------------------------------------------
