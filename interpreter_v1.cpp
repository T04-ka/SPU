

#define STK_SANITIZE
#define STK_SANITIZE_LOUD
#define STK_HANDLER_ABORT
#define STK_ELM_T int

#include "./../Stack/stack.h"

#include <stdio.h>
#include <string.h>

str_t const cmd_PSH = "PSH";
str_t const cmd_ADD = "ADD";
str_t const cmd_SUB = "SUB";
str_t const cmd_DIV = "DIV";
str_t const cmd_OUT = "OUT";
str_t const cmd_HLT = "HLT";
str_t const cmd_DMP = "DMP";
str_t const cmd_MUL = "MUL";

#define IS_CMD(CMD) !strcmp(cmd, cmd_##CMD)

int main(){

    stack_t stk = {};
    STACK_CTOR(stk, 4);

    //$;

    char cmd[10] = {};

    do {

        scanf("%s", cmd);
        //$s(cmd);
        if (IS_CMD(PSH)) {

            int var = 0;
            scanf("%d", &var);
            $d(var);
            STACK_PUSH(&stk, var);
            continue;
        }

        if (IS_CMD(ADD)) {

            int var1 = 0, var2 = 0;
            STACK_POP(&stk, &var1);
            STACK_POP(&stk, &var2);
            STACK_PUSH(&stk, var1 + var2);
            continue;
        }

        if (IS_CMD(SUB)) {

            int var1 = 0, var2 = 0;
            STACK_POP(&stk, &var1);
            STACK_POP(&stk, &var2);
            STACK_PUSH(&stk, var2 - var1);
            continue;
        }

        if (IS_CMD(OUT)) {

            int var = 0;
            STACK_POP(&stk, &var);
            printf("Out value: %d\n", var);
            continue;
        }

        if (IS_CMD(DMP)) {

            STACK_DUMP(&stk);
            continue;
        }

        if (IS_CMD(MUL)) {

            int var1 = 0, var2 = 0;
            STACK_POP(&stk, &var1);
            STACK_POP(&stk, &var2);
            STACK_PUSH(&stk, var2 * var1);
            continue;
        }

        if (IS_CMD(DIV)) {

            int var1 = 0, var2 = 0;
            STACK_POP(&stk, &var1);
            STACK_POP(&stk, &var2);
            STACK_PUSH(&stk, var2 / var1);
            continue;
        }

    } while (strcmp(cmd, cmd_HLT));


    STACK_DTOR(stk);
}
