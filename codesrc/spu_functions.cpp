//----------------------------------------------------------------------------------------------------------------

#include "enum.h"
#define GET_CMD(IND)    * ( (char*) spu_t->cmd_buf + IND )
#define GET_ARG(IND)    * (int*)( (char*) spu_t->cmd_buf + IND )
#define PC_INCR         spu_t->pc += CMDSZ
#define MEMCPY_ARG 0; memcpy(&copied_arg, &GET_ARG(spu_t->pc), ARGSZ)

//----------------------------------------------------------------------------------------------------------------

int spu_push(spu_t* spu_t);

int spu_push(spu_t* spu_t) {

    if (spu_t->argument == NULL) {

        spu_t->onbreak = cmd_t_WRONG_ARG;
        return 1;
    }
    int argument = 0;
    // $d(*spu_t->argument);
    memcpy(&argument, spu_t->argument, sizeof(int));

    STACK_PUSH(&spu_t->stk, argument);

    spu_t->pc += ARGSZ;

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_add(spu_t* spu_t);

int spu_add(spu_t* spu_t) {



    int var1 = 0, var2 = 0;
    STACK_POP(&spu_t->stk, &var1);
    STACK_POP(&spu_t->stk, &var2);
    STACK_PUSH(&spu_t->stk, var1 + var2);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_sub(spu_t* spu_t);

int spu_sub(spu_t* spu_t) {



    int var1 = 0, var2 = 0;
    STACK_POP(&spu_t->stk, &var1);
    STACK_POP(&spu_t->stk, &var2);
    STACK_PUSH(&spu_t->stk, var1 - var2);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_out(spu_t* spu_t);

int spu_out(spu_t* spu_t) {



    int var = 0;
    STACK_POP(&spu_t->stk, &var);
    printf("%lg\n", 1.0 * var / SPU_EXPANENT_COEFF);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_mul(spu_t* spu_t);

int spu_mul(spu_t* spu_t) {



    int var1 = 0, var2 = 0;
    STACK_POP(&spu_t->stk, &var1);
    STACK_POP(&spu_t->stk, &var2);
    STACK_PUSH(&spu_t->stk, var2 * var1 / SPU_EXPANENT_COEFF);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_div(spu_t* spu_t);

int spu_div(spu_t* spu_t) {



    int var1 = 0, var2 = 0;
    STACK_POP(&spu_t->stk, &var1);
    STACK_POP(&spu_t->stk, &var2);
    if (var2 == 0) {

        ERRLOG(RED FAT "Division by zero.\n%s" DEF, "");
        spu_t->onbreak = cmd_t_DIV_BY_ZERO;
    }
    else {

        STACK_PUSH(&spu_t->stk, var1 * SPU_EXPANENT_COEFF / var2);
    }

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_halt(spu_t* spu_t);

int spu_halt(spu_t* spu_t) {


    spu_t->onbreak = cmd_t_NORMAL_EXIT;

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_jump(spu_t* spu_t);

int spu_jump(spu_t* spu_t) {



    int copied_arg = MEMCPY_ARG;

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
    int copied_arg = MEMCPY_ARG;                                        \
    spu_t->pc = copied_arg / SPU_EXPANENT_COEFF;                        \
}                                                                       \
else {                                                                  \
                                                                        \
    spu_t->pc += ARGSZ;                                                 \
}                                                                       \
                                                                        \
STACK_PUSH(&spu_t->stk, frst_chr);

//----------------------------------------------------------------------------------------------------------------

int spu_jump_if_bggr(spu_t* spu_t);

int spu_jump_if_bggr(spu_t* spu_t) {

    JUMP_IF(>);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_jump_if_smllr(spu_t* spu_t);

int spu_jump_if_smllr(spu_t* spu_t) {

    JUMP_IF(<);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_jump_if_eq(spu_t* spu_t);

int spu_jump_if_eq(spu_t* spu_t) {

    JUMP_IF(==);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_jump_if_nbggr(spu_t* spu_t);

int spu_jump_if_nbggr(spu_t* spu_t) {

    // $err("not bigger | less or equal | LE")
    JUMP_IF(<=);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_jump_if_nsmllr(spu_t* spu_t);

int spu_jump_if_nsmllr(spu_t* spu_t) {

    // $err("not smaller | greater or equal | GE")
    JUMP_IF(>=);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_jump_if_neq(spu_t* spu_t);

int spu_jump_if_neq(spu_t* spu_t) {

    JUMP_IF(!=);

    return 0;
}

#undef JUMP_IF
//----------------------------------------------------------------------------------------------------------------

int spu_call(spu_t* spu_t);

int spu_call(spu_t* spu_t) {

    STACK_PUSH(&spu_t->calltree, spu_t->pc + ARGSZ);

    int copied_arg = MEMCPY_ARG;

    spu_t->pc = copied_arg / SPU_EXPANENT_COEFF;

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_ret(spu_t* spu_t);

int spu_ret(spu_t* spu_t) {

    STACK_POP(&spu_t->calltree, &spu_t->pc);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_pop(spu_t* spu_t);

int spu_pop(spu_t* spu_t) {

    if (spu_t->argument == NULL) {

        spu_t->onbreak = cmd_t_WRONG_ARG;
        return 1;
    }
    STACK_POP(&spu_t->stk, spu_t->argument);

    spu_t->pc += ARGSZ;

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_in(spu_t* spu_t);

int spu_in(spu_t* spu_t) {



    double in_value = NAN;
    scanf("%lg[^\n]", &in_value);

    STACK_PUSH(&spu_t->stk, (int) in_value * SPU_EXPANENT_COEFF);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_sqrt(spu_t* spu_t);

int spu_sqrt(spu_t* spu_t) {



    int value = 0;
    STACK_POP(&spu_t->stk, &value);

    // $d(value);
    double sqrd_value = sqrt((double) value / SPU_EXPANENT_COEFF);
    // $lg(sqrd_value);
    STACK_PUSH(&spu_t->stk, (int) (sqrd_value * SPU_EXPANENT_COEFF));

    return 0;
}

//----------------------------------------------------------------------------------------------------------------
// TODO: сначала писать память в буффер, а потом скидывать в консоль
int spu_draw(spu_t* spu_t);

int spu_draw(spu_t* spu_t) {



    int window_width = spu_t->reg_buf[RDW] / SPU_EXPANENT_COEFF;
    int memory_first_index = spu_t->reg_buf[RMFI] / SPU_EXPANENT_COEFF;
    int memory_draw_size = spu_t->reg_buf[RMDSZ] / SPU_EXPANENT_COEFF;

    for (int port = 0; port < memory_draw_size / window_width + 1; port++) {

        for (int p = window_width * port; p < window_width * (port + 1); p++) {

            if (spu_t->mem_buf[memory_first_index + p]) {

                putc('Z', stderr);
            }
            else {

                putc('.', stderr);
            }
        }
        putc('\n', stderr);
    }

    return 0;
}

#undef GET_CMD
#undef GET_ARG
#undef PC_INCR
#undef MEMCPY_ARG

//----------------------------------------------------------------------------------------------------------------

typedef int spu_opfnc_t(spu_t* spu_t);

//----------------------------------------------------------------------------------------------------------------

void cmd_opfnc_t_init();

spu_opfnc_t* cmd_opfnc_t [CMD_N__TOUCH_ONLY_ON_ADDING_NEW_COMMANDS_AND_NO_EXCEPT_BLYAT - 2] = {};

void cmd_opfnc_t_init() {

    cmd_opfnc_t[0] = spu_halt;
    cmd_opfnc_t[1] = spu_push;
    cmd_opfnc_t[2] = spu_add;
    cmd_opfnc_t[3] = spu_sub;
    cmd_opfnc_t[4] = spu_div;
    cmd_opfnc_t[5] = spu_mul;
    cmd_opfnc_t[6] = spu_out;
    cmd_opfnc_t[7] = spu_jump;
    cmd_opfnc_t[8] = spu_jump_if_bggr;
    cmd_opfnc_t[9] = spu_jump_if_smllr;
    cmd_opfnc_t[10] = spu_jump_if_eq;
    cmd_opfnc_t[11] = spu_jump_if_neq;
    cmd_opfnc_t[12] = spu_jump_if_nsmllr;
    cmd_opfnc_t[13] = spu_jump_if_neq;
    cmd_opfnc_t[14] = spu_call;
    cmd_opfnc_t[15] = spu_ret;
    cmd_opfnc_t[20] = spu_pop;
    cmd_opfnc_t[23] = spu_in;
    cmd_opfnc_t[24] = spu_sqrt;
    cmd_opfnc_t[25] = spu_draw;
}

//----------------------------------------------------------------------------------------------------------------
