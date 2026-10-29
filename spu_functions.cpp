
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

int spu_pushm(spu_t* spu_t);

int spu_popm(spu_t* spu_t);

int spu_pop(spu_t* spu_t);

int spu_pushmr(spu_t* spu_t);

int spu_popmr(spu_t* spu_t);

int spu_in(spu_t* spu_t);

int spu_sqrt(spu_t* spu_t);

//----------------------------------------------------------------------------------------------------------------

typedef int spu_opfnc_t(spu_t* spu_t);

//----------------------------------------------------------------------------------------------------------------

spu_opfnc_t* cmd_opfnc_t [CMD_N__TOUCH_ONLY_ON_ADDING_NEW_COMMANDS_AND_NO_EXCEPT_BLYAT - 2] =
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
        spu_popr,
        spu_pushm,
        spu_popm,
        spu_pop,
        spu_pushmr,
        spu_popmr,
        spu_in,
        spu_sqrt,
    };



void cmd_opfnc_t_init() {


    // cmd_opfnc_t[0] = spu_ret;
}

#define GET_CMD(IND)    * ( (char*) spu_t->cmd_buf + IND )
#define GET_ARG(IND)    * (int*)( (char*) spu_t->cmd_buf + IND )
#define PC_INCR         spu_t->pc += CMDSZ
#define MEMCPY_ARG 0; memcpy(&copied_arg, &GET_ARG(spu_t->pc), ARGSZ)

//----------------------------------------------------------------------------------------------------------------

int spu_push(spu_t* spu_t) {

    PC_INCR;
    // сделать через memcpy
    // $du(( *(unsigned int*)((char*) spu_t->cmd_buf + spu_t->pc) ));

    int copied_arg = MEMCPY_ARG;

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
    STACK_PUSH(&spu_t->stk, var2 / SPU_EXPANENT_COEFF * var1);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_div(spu_t* spu_t) {

    PC_INCR;

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

int spu_halt(spu_t* spu_t) {

    PC_INCR;
    spu_t->onbreak = cmd_t_NORMAL_EXIT;

    return 0;
}

//----------------------------------------------------------------------------------------------------------------
//УЧЕСТЬ ЧТО МЕТКА В 1000 раз больше
int spu_jump(spu_t* spu_t) {

    PC_INCR;

    int copied_arg = MEMCPY_ARG;

    spu_t->pc = copied_arg / SPU_EXPANENT_COEFF; // VOT TUT NAVERNOE

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

#define JUMP_IF(CMP)                                                    \
PC_INCR;                                                                \
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

    // $err("not bigger | less or equal | LE")
    JUMP_IF(<=);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_jump_if_nsmllr(spu_t* spu_t) {

    // $err("not smaller | greater or equal | GE")
    JUMP_IF(>=);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_jump_if_neq(spu_t* spu_t) {

    JUMP_IF(!=);

    return 0;
}

#undef JUMP_IF
//----------------------------------------------------------------------------------------------------------------


int spu_call(spu_t* spu_t) {

    PC_INCR;

    STACK_PUSH(&spu_t->calltree, spu_t->pc + ARGSZ);

    int copied_arg = MEMCPY_ARG;

    spu_t->pc = copied_arg / SPU_EXPANENT_COEFF;

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
    int copied_arg = MEMCPY_ARG;

    STACK_PUSH(&spu_t->stk, spu_t->reg_buf[copied_arg / SPU_EXPANENT_COEFF]);

    spu_t->pc += ARGSZ;

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_popr(spu_t* spu_t) {

    PC_INCR;
    // $d(GET_ARG(spu_t->pc));
    int copied_arg = MEMCPY_ARG;

    STACK_POP(&spu_t->stk, &spu_t->reg_buf[copied_arg / SPU_EXPANENT_COEFF]);

    spu_t->pc += ARGSZ;

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_pushm(spu_t* spu_t) {

    PC_INCR;

    int copied_arg = MEMCPY_ARG;

    STACK_PUSH(&spu_t->stk, spu_t->mem_buf[copied_arg]);

    spu_t->pc += ARGSZ;

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_popm(spu_t* spu_t) {

    PC_INCR;

    int copied_arg = MEMCPY_ARG;

    STACK_POP(&spu_t->stk, &spu_t->mem_buf[copied_arg]);

    spu_t->pc += ARGSZ;

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_pop(spu_t* spu_t) {

    PC_INCR;

    int copied_arg = MEMCPY_ARG;

    int useless_var_for_pop = 0;
    for (int i = 0; i < copied_arg; i++) {

        STACK_POP(&spu_t->stk, &useless_var_for_pop);
    }

    spu_t->pc += ARGSZ;

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_pushmr(spu_t* spu_t) {

    PC_INCR;

    int copied_arg = MEMCPY_ARG;

    int register_value = spu_t->reg_buf[copied_arg];

    STACK_PUSH(&spu_t->stk, spu_t->mem_buf[register_value]);

    spu_t->pc += ARGSZ;

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_popmr(spu_t* spu_t) {

    PC_INCR;

    int copied_arg = MEMCPY_ARG;

    int register_value = spu_t->reg_buf[copied_arg];

    STACK_POP(&spu_t->stk, &spu_t->mem_buf[register_value]);

    spu_t->pc += ARGSZ;

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_in(spu_t* spu_t) {

    PC_INCR;

    double in_value = NAN;
    scanf("%lg[^\n]", &in_value);

    STACK_PUSH(&spu_t->stk, (int) in_value * SPU_EXPANENT_COEFF);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int spu_sqrt(spu_t* spu_t) {

    PC_INCR;

    int value = 0;
    STACK_POP(&spu_t->stk, &value);

    // $d(value);
    double sqrd_value = sqrt((double) value / SPU_EXPANENT_COEFF);
    // $lg(sqrd_value);
    STACK_PUSH(&spu_t->stk, (int) (sqrd_value * SPU_EXPANENT_COEFF));

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

#undef GET_CMD
#undef GET_ARG
#undef PC_INCR
#undef MEMCPY_ARG
