

#define DEBUG
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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
    int*        mem_buf;
};

#include "spu_functions.cpp"

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
        // $d(*( (char*) spu_t.cmd_buf + spu_t.pc ));
        cmd_opfnc_t [*( (char*) (char*) spu_t.cmd_buf + spu_t.pc )] (&spu_t);
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

    char cmd = *((char*) spu_t.cmd_buf + spu_t.pc);

    ERRLOG("Command: %d", cmd);

    if (CMD_L[cmd].argqty) {

        ERRLOG(" %d\n", *(int*)((char*) spu_t.cmd_buf + spu_t.pc + 1));
    }
    else {

        ERRLOG("\n");
    }

    STACK_DUMP(&spu_t.stk);
    ERRLOG(FORMAT_LINE);

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
    spu_t->reg_buf = (int*) calloc(REG_N__NEVER_TOUCH_IT_AT_ALL_BLYAT, sizeof(int));
    spu_t->mem_buf = (int*) calloc(MEMORY_SIZE__NE_TROGAI_EGO_ONO_TEBYA_SOJRET, sizeof(char));

    FILE* cmd_list_fl = fopen(argv[1], "rb");

    if (cmd_list_fl == NULL) {

        ERRLOG(RED FAT "No such file.\n" DEF);
    }

    void* signa_arr = calloc(1, SIGNA_SIZE + 2 * ADMIN_DATA_SIZE);

    fread(signa_arr, 16, 1, cmd_list_fl);

    if (*(unsigned long*) signa_arr != *SIGNATURE___TOUCH_ONLY_IF_YOU_DONT_AFRAID_OF_THE_CONSEQUENSES) {

        fprintf(stderr, RED FAT "CHE ZA GOVNO TI MNE PODSUNUL? IDI COMPILIRUI NORMAL'NO. PIDOR.\n" DEF);
        return 1;
    }

    spu_t->cmd_qty = (size_t) *( (char*) signa_arr + 2 * 4 );
    spu_t->cmd_buf_size = (size_t) *((int*) signa_arr + 3);

    spu_t->cmd_buf = calloc(spu_t->cmd_buf_size, 1);
    fread(spu_t->cmd_buf, 1, spu_t->cmd_buf_size, cmd_list_fl);

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
    free(spu_t->mem_buf);
    spu_t->pc = -1;
    spu_t->cmd_buf_size = -1;
    spu_t->cmd_qty = -1;
    spu_t->onbreak = (cmd_err) 0;
}

//----------------------------------------------------------------------------------------------------------------
