#include "./../Stack/stack.h"
#include "./../Strfuncs/getline.cpp"

#include <stdio.h>

enum cmd_n {
    cmd_n_PSH = 1,
    cmd_n_ADD = 2,
    cmd_n_SUB = 3,
    cmd_n_DIV = 4,
    cmd_n_MUL = 5,
    cmd_n_OUT = 6,
    cmd_n_DMP = 7,
    cmd_n_HLT = 0
};


enum cmd_t {
    cmd_t_OK            = 0,
    cmd_t_NORMAL_EXIT   = 1,
    cmd_t_DIV_BY_ZERO   = 2,
    cmd_t_WRONG_CMD     = -1
};

int calc(stack_t* stkptr, FILE* cmdlist);

int calc_push(stack_t* stkptr, FILE* cmdlistfl);

int calc_add(stack_t* stkptr);

int calc_sub(stack_t* stkptr);

int calc_div(stack_t* stkptr, cmd_t* onbreak);

int calc_mul(stack_t* stkptr);

int calc_out(stack_t* stkptr);

int calc_dmp(stack_t* stkptr);

int calc_halt(/*stack_t* stkptr, */cmd_t* onbreak);


int main(int argc, char** argv) {

    stack_t stk = {};
    STACK_CTOR(stk, 4);

    FILE* cmdlistfl = fopen(argv[1], "r");

    calc(&stk, cmdlistfl);

    fclose(cmdlistfl);

    STACK_DTOR(stk);

    return 0;
}

//char* str = NULL;
#undef ERRLOG
#define ERRLOG(FRMT, ...) fprintf(stderr, FRMT, __VA_ARGS__)
int calc(stack_t* stkptr, FILE* cmdlistfl) {

    int cmd = 0;
    cmd_t onbreak = cmd_t_OK;
    printf("Start calculator program.\n");
    while (!onbreak) {

        cmd = -1;

        fscanf(cmdlistfl, "%d", &cmd);

        switch (cmd) {

            case cmd_n_PSH:
            {
                calc_push(stkptr, cmdlistfl);
                break;
            }

            case cmd_n_ADD:
            {
                calc_add(stkptr);
                break;
            }

            case cmd_n_SUB:
            {
                calc_sub(stkptr);
                break;
            }

            case cmd_n_OUT:
            {
                calc_out(stkptr);
                break;
            }

            case cmd_n_DMP:
            {
                calc_dmp(stkptr);
                break;
            }

            case cmd_n_MUL:
            {
                calc_mul(stkptr);
                break;
            }

            case cmd_n_DIV:
            {
                calc_div(stkptr, &onbreak);
                break;
            }

            case cmd_n_HLT:
            {
                calc_halt(/*stkptr, */&onbreak);
                break;
            }
            default:
            {
                ERRLOG(RED FAT "Wrong command given.\n%s" DEF, "");
                onbreak = cmd_t_WRONG_CMD;
                break;
            }
        }
    }

    if (onbreak == cmd_t_NORMAL_EXIT) {

        printf("End calculator program.\n");
    }

    return 0;
}


int calc_push(stack_t* stkptr, FILE* cmdlistfl) {

    int var = 0;
    fscanf(cmdlistfl, "%d", &var);
    STACK_PUSH(stkptr, var);

    return 0;
}


int calc_add(stack_t* stkptr) {

    int var1 = 0, var2 = 0;
    STACK_POP(stkptr, &var1);
    STACK_POP(stkptr, &var2);
    STACK_PUSH(stkptr, var1 + var2);
    return 0;
}


int calc_sub(stack_t* stkptr) {

    int var1 = 0, var2 = 0;
    STACK_POP(stkptr, &var1);
    STACK_POP(stkptr, &var2);
    STACK_PUSH(stkptr, var2 - var1);
    return 0;
}


int calc_out(stack_t* stkptr) {

    int var = 0;
    STACK_POP(stkptr, &var);
    printf("Out value: %lg\n", var / 1000.0);
    return 0;
}


int calc_dmp(stack_t* stkptr) {

    STACK_DUMP(stkptr);
    return 0;
}


int calc_mul(stack_t* stkptr) {

    int var1 = 0, var2 = 0;
    STACK_POP(stkptr, &var1);
    STACK_POP(stkptr, &var2);
    STACK_PUSH(stkptr, var2 * var1 / 1000);

    return 0;
}


int calc_div(stack_t* stkptr, cmd_t* onbreak) {

    int var1 = 0, var2 = 0;
    STACK_POP(stkptr, &var1);
    STACK_POP(stkptr, &var2);
    if (var1 == 0) {

        ERRLOG(RED FAT "Division by zero.\n%s" DEF, "");
        *onbreak = cmd_t_DIV_BY_ZERO;
    }
    else {

        STACK_PUSH(stkptr, var2 * 1000 / var1);
    }

    return 0;
}


int calc_halt(/*tack_t stkptr, */cmd_t* onbreak) {

    *onbreak = cmd_t_NORMAL_EXIT;

    return 1;
}
