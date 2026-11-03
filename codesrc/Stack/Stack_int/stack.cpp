#include "stack.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>


#define STK_HEAD_MXBFFRSZ (SIZE_MAX / 8)
#define MXSTKNM 100

#ifndef STK_MXBFFRSZ
#define STK_MXBFFRSZ STK_HEAD_MXBFFRSZ
#endif

#define PZN 69

const char* FNCNMS[] = {
    "int stkdmp(stack_t*, str_t, str_t, str_t, int)",
    "int stkvrf(stack_t*, str_t)",
    "void stkerrhnd(stack_t*, _errt, str_t, str_t, str_t, int)",
    "int stkpush(stack_t*, int, str_t, str_t, int)",
    "int stkpop(stack_t*, int*, str_t, str_t, int)",
    "int stkgrow(stack_t*, str_t, str_t, str_t, int)",
    "int stkshrnk(stack_t*, str_t, str_t, str_t, int)",
    "int stkctor(stack_t*, str_t, size_t, str_t, str_t, int)",
    "int stkdtor(stack_t*, str_t, str_t, int)",
    "int stkpzn(stack_t*, str_t, str_t, str_t, int)"
};

const int NFNCS = 10;

//TODO: СДЕЛАТЬ ПОД УСЛОВНУЮ КОМПИЛЯЦИЮ ПЕРЕДАЧУ ФУНКЦИЙ
//TODO: СДЕЛАТЬ ОТКЛЮЧЕНИЕ ПЕРЕДАЧИ СЛУЖЕБНЫХ ПЕРЕЕННЫХ ПОД МАКРОСС


#ifdef STK_SANITIZE_LOUD
FILE* logfl = stderr;
#else
FILE* logfl = fopen("log.log", "w");
#endif


#include "stack.h"

#ifdef STK_SANITIZE
#define HANDLER_TOGGLE  stkerrhnd(Stk, (_errt) err, __PRETTY_FUNCTION__, _fl, _fnc, _nln);
#else
#define HANDLER_TOGGLE void(0)
#endif

//-------------------------------------------------------------------------------------
#define STACK_OK                                                                                \
    int err = stkvrf(Stk, __PRETTY_FUNCTION__);                                                 \
    HANDLER_TOGGLE;                                                                             \
    if (err) return err;


//-------------------------------------------------------------------------------------
#define ERROR_HANDLER_CALL  HANDLER_TOGGLE;     \
                            return err;


//-------------------------------------------------------------------------------------
// //printf("strcmp(%s, %s) = %d\n", __PRETTY_FUNCTION__, FNCNMS[i], strcmp(__PRETTY_FUNCTION__, FNCNMS[i]));
#define PREVFUNC_ACCESS_OK                                                                  \
    int prvfnc_ok = 0;                                                                      \
    for (int i = 0; i < NFNCS; i++) {                                                       \
        if (!strcmp(_prvfnc, FNCNMS[i])) {                                                  \
                                                                                            \
            prvfnc_ok = 1;                                                                  \
            break;                                                                          \
        }                                                                                   \
    }                                                                                       \
    if (!prvfnc_ok) {                                                                       \
                                                                                            \
        stkerrhnd(Stk, STK_WRONG_PREVFUNC_ACCESS,  __PRETTY_FUNCTION__, _fl, _fnc, _nln);   \
    }


//-------------------------------------------------------------------------------------
#ifndef STK_NOCHECK_STRUCT_HASH

#ifdef STK_CHECK_BUFFER_HASH
#define REWRITE_STRUCT_HASH_VAL Stk->_strcthsh = stkhsh((void*) Stk, sizeof(*Stk) - sizeof(Stk->_strcthsh) - sizeof(Stk->_bffrhsh));
#else
#define REWRITE_STRUCT_HASH_VAL Stk->_strcthsh = stkhsh((void*) Stk, sizeof(*Stk) - sizeof(Stk->_strcthsh));
#endif

#else
#define REWRITE_STRUCT_HASH_VAL (void) 0;
#endif


//-------------------------------------------------------------------------------------
#ifdef STK_CHECK_BUFFER_HASH
#define REWRITE_BUFFER_HASH_VAL Stk->_bffrhsh = stkhsh((void*) Stk->bffr, Stk->cpty * sizeof(*Stk->bffr));
#else
#define REWRITE_BUFFER_HASH_VAL (void) 0;
#endif

//-------------------------------------------------------------------------------------
#define LOG(format, ...) fprintf(outfl, format, __VA_ARGS__);
#define SEP fprintf(outfl, "\n-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
#define NLN putc('\n', outfl);
#define IF_CALLEDFUNCTION_STKERRHND if (!strcmp(FNCNMS[2], _frmfnc))

int stkdmp(stack_t* Stk, str_t _frmfnc, str_t _fl, str_t _fnc, int _nln) {

    FILE* outfl = stderr;

    IF_CALLEDFUNCTION_STKERRHND {

        outfl = logfl;
        SEP;
        LOG("Dump was called by error handler. %s", "");
    }
    else {

        STACK_OK;

        outfl = stdout;
        SEP;
        LOG("Dump was called from function " FAT "%s" DEF ", exactly from " FAT"%s:%d" DEF". ", _frmfnc, _fl, _nln);
    }


    LOG(FAT "\"%s\"" DEF " created in file " FAT "%s" DEF" in " FAT "%s" DEF " on line " FAT "%d" DEF ".\n\n",
              Stk->_nm,                              Stk->_brnfl,          Stk->_brnfnc,               Stk->_brnln);


    LOG("\tStack address: [%p]. Stack type: \"%s\".\n", Stk, Stk->_tp);

    LOG("\tCapacity = %zu. ", Stk->cpty);
    LOG("Size = %zu. ", Stk->sz);
    LOG("Buffer adress = [%p]. \n", Stk->bffr);

    if (Stk->bffr == NULL) {

        return STK_BUFFER_NULLPTR;
    }

    NLN;
    NLN;
    size_t i = 0;
    for (i = 0; i < Stk->sz; i++){

        LOG("\t*[%zu] = %d\n", i, Stk->bffr [i]); //TODO СДЕЛАТЬ МАКРОСС ДЛЯ ПРИЗВОЛЬНОГО ТИПА
    }

    for (; i < Stk->cpty; i++) {

        LOG("\t[%zu] = %d (PZN)\n", i, PZN);
    }
    NLN;
    SEP;

    return 0;
}

#undef LOG
#undef SEP
#undef NLN


//-------------------------------------------------------------------------------------
#define IS_FUNCTION_STKCDTOR !strcmp(FNCNMS[7], _prvfnc) || !strcmp(FNCNMS[8], _prvfnc)


#ifndef STK_NOCHECK_STRUCT_HASH
#ifdef STK_CHECK_BUFFER_HASH
#define STRUCT_HASH_VAL_OK                                                                                          \
    if (Stk->_strcthsh != stkhsh((void*) Stk, sizeof(*Stk) - sizeof(Stk->_strcthsh) - sizeof(Stk->_bffrhsh))) {     \
                                                                                                                    \
        return STK_WRONG_STRUCT_HASH;                                                                               \
    }
#else
#define STRUCT_HASH_VAL_OK                                                                  \
    if (Stk->_strcthsh != stkhsh((void*) Stk, sizeof(*Stk) - sizeof(Stk->_strcthsh))) {     \
                                                                                            \
        return STK_WRONG_STRUCT_HASH;                                                       \
    }
#endif
#else
#define STRUCT_HASH_VAL_OK (void) 0
#endif


#ifdef STK_CHECK_BUFFER_HASH
#define BUFFER_HASH_OK                                                                      \
    if (Stk->_bffrhsh != stkhsh((void*) Stk->bffr, Stk->cpty * sizeof(*Stk->bffr))) {       \
                                                                                            \
        return STK_WRONG_BUFFER_HASH;                                                       \
}
#else
#define BUFFER_HASH_OK (void) 0
#endif


int stkvrf(stack_t* Stk, str_t _prvfnc) {

    //PREVFUNC_ACCESS_OK;

    if (Stk == NULL) {

        return STK_STRUCT_NULLPTR;
    }

    if (IS_FUNCTION_STKCDTOR) {

        return STK_OK;
    }

    if (Stk->cpty > STK_MXBFFRSZ) {

        return STK_WRONG_CPTY; //WRONG CAPACITY //TODO сделать проверку на длину динамической памяти
    }

    if (Stk->sz > Stk->cpty) {

        return STK_WRONG_SZ;
    }

    if (Stk->bffr == NULL) {

        return STK_BUFFER_NULLPTR;
    }

    if (!isgoodreadptr(Stk->bffr, sizeof(Stk->bffr))) {

        return STK_DATA_NO_READ_ACCES;
    }

    STRUCT_HASH_VAL_OK;

    BUFFER_HASH_OK;

    //TODO КАНАРЕЙКИ И ХЭШИ

    return STK_OK;
}


#ifdef STK_SANITIZE
//-------------------------------------------------------------------------------------
#define PRINT_END_MESSAGE fprintf(logfl, RED FAT "\n================================================================================ YASHA PIDORAS ================================================================================\n" DEF);

#define ERRLOG(format, ...) fprintf(logfl, format, __VA_ARGS__);

//-------------------------------------------------------------------------------------
#define FRMT1 "=================================================="
#define FRMT2 "======================================================="
#define FRMT3 "======================================================="
#define FRMT4 "========================================================="
#define FRMT5 "==========================================================="
#define FRMT6 "========================================================"
#define FRMT7 "======================================================="
#define FRMT8 "====================================================="
#define FRMT9 "====================================================="
#define FRMT10 "===================================================="
#define FRMT11 "====================================================="
#ifndef STK_NOCHECK_STRUCT_HASH
#define FRMT12 "====================================================="
#endif
#ifdef STK_CHECK_BUFFER_HASH
#define FRMT13 "====================================================="
#endif
//-------------------------------------------------------------------------------------
#define ERRMSG1  ERRLOG("Function " FAT "%s" DEF" was called from function " FAT "%s" DEF ", that has no acces to сall.", _prvfnc, _fnc);
#define ERRMSG2  ERRLOG("For some reason, NULL was passed as a pointer to the structure.%s", "");
#define ERRMSG3  ERRLOG("For some reason, the pointer to the buffer turned out to be NULL.%s", "");
#define ERRMSG4  ERRLOG("For some reason, stack buffer capacity has wrong value: " FAT "%zu" DEF ". In particular, it's larger than maximum size of stack buffer: " FAT"%zu" DEF".", Stk->cpty, STK_MXBFFRSZ);
#define ERRMSG5  ERRLOG("For some reason, size of filled stack buffer has wrong value: " FAT "%zu" DEF ". In particular, it's larger than current stack buffer capacity: " FAT"%zu" DEF".", \
                                                                                        Stk->sz,                                                                             Stk->cpty);
#define ERRMSG6  ERRLOG("For some reason, memory allocation in function " FAT"%s" DEF" failed.", _prvfnc);
#define ERRMSG7  ERRLOG("Stack buffer underflow.%s", "");
#define ERRMSG8  ERRLOG("An attempt to create an array that is too large. Max stack buffer len: " FAT "%zu" DEF ". Given capacity: " FAT "%zu" DEF".", \
                                                                                    STK_MXBFFRSZ,                       Stk->cpty);
#define ERRMSG9  ERRLOG("Bad attempt to define STK_MXBFFRSZ. STK_MXBFFRSZ defined as " FAT "%zu" DEF ", which is too large.", \
                                                                            STK_MXBFFRSZ);
#define ERRMSG10 ERRLOG("For some reason, NULL was passed as a pointer to the poped value reciever.%s", "");
#define ERRMSG11 ERRLOG("For some reason, pointer on NON-readble data was passed as a pointer on a buffer.%s", "");
#ifndef STK_NOCHECK_STRUCT_HASH
#define ERRMSG12 ERRLOG("For some reason, the structure hash changed the value, even though it shouldn't have.%s", "");
#endif

#ifdef STK_CHECK_BUFFER_HASH
#define ERRMSG13 ERRLOG("For some reason, the structure buffer hash changed the value, even though it shouldn't have.%s", "");
#endif
//-------------------------------------------------------------------------------------

#define PRINT_WHERE_FROM_CALLED ERRLOG(" Error handler was called from function " FAT"%s" DEF". Last stack call was in " FAT"%s" DEF":" FAT"%d" DEF" in function " FAT"%s" DEF".\n", _prvfnc, _fl, _nln, _fnc);

//-------------------------------------------------------------------------------------
#define PRINT_ERROR_MESSAGE(N, ERR)                                                                                 \
    ERRLOG(RED FAT FRMT##N " CRITICAL FATAL PANIC UNRECOVERABLE ERROR " #N ": " #ERR " %s" FRMT##N "\n\n" DEF, ""); \
    ERRMSG##N                                                                                                       \
    PRINT_WHERE_FROM_CALLED                                                                                         \
    PRINT_END_MESSAGE                                                                                               \


//-------------------------------------------------------------------------------------
#ifdef STK_HANDLER_ABORT

#ifdef STK_SANITIZE_LOUD
#define TOGGLE_ABORT abort();
#else
#define TOGGLE_ABORT fclose(logfl); abort();
#endif

#else
#define TOGGLE_ABORT (void) 0
#endif


void stkerrhnd(stack_t* Stk, _errt err, str_t _prvfnc, str_t _fl, str_t _fnc, int _nln) {

    PREVFUNC_ACCESS_OK

    switch (err)
    {
        case STK_OK: {

            break;
        }

        case STK_WRONG_PREVFUNC_ACCESS://WRONG FUNCTION CALLED VERIFICATOR
        {
            PRINT_ERROR_MESSAGE(1, STK_WRONG_PREVFUNC_ACCESS);
            stkdmp(Stk, __PRETTY_FUNCTION__, _fl, _fnc, _nln);
            TOGGLE_ABORT;
            break;
        }

        case STK_STRUCT_NULLPTR: //STRUCTURE HAS NULL POINTER
        {
            PRINT_ERROR_MESSAGE(2, STK_STRUCT_NULLPTR);
            TOGGLE_ABORT;
            break;
        }

        case STK_BUFFER_NULLPTR: //STACK BUFFER HAS NULL POINTER
        {
            PRINT_ERROR_MESSAGE(3, STK_BUFFER_NULLPTR);
            stkdmp(Stk, __PRETTY_FUNCTION__, _fl, _fnc, _nln);
            TOGGLE_ABORT;
            break;
        }

        case STK_WRONG_CPTY: //CAPACITY HAS WRONG VALUE
        {
            PRINT_ERROR_MESSAGE(4, STK_WRONG_CPTY);
            stkdmp(Stk, __PRETTY_FUNCTION__, _fl, _fnc, _nln);
            TOGGLE_ABORT;
            break;
        }

        case STK_WRONG_SZ: //SIZE HAS WRONG VALUE
        {
            PRINT_ERROR_MESSAGE(5, STK_WRONG_SZ);
            stkdmp(Stk, __PRETTY_FUNCTION__, _fl, _fnc, _nln);
            TOGGLE_ABORT;
            break;
        }

        case STK_MALLOC_FAILED: //6: FAILED MALLOCATION
        {
            PRINT_ERROR_MESSAGE(6, STK_MALLOC_FAILED);
            stkdmp(Stk, __PRETTY_FUNCTION__, _fl, _fnc, _nln);
            TOGGLE_ABORT;
            break;
        }

        case STK_BUFFER_UNDERFLOW: //STACKBUFFERUNDERFLOW
        {
            PRINT_ERROR_MESSAGE(7, STK_BUFFER_UNDERFLOW);
            stkdmp(Stk, __PRETTY_FUNCTION__, _fl, _fnc, _nln);
            TOGGLE_ABORT;
            break;
        }

        case STK_DEFINED_CPTY_EXCESS:
        {
            PRINT_ERROR_MESSAGE(8, STK_DEFINED_CPTY_EXCESS);
            stkdmp(Stk, __PRETTY_FUNCTION__, _fl, _fnc, _nln);
            TOGGLE_ABORT;
            break;
        }

        case STK_WRONG_MXBFFRSZ_DEFINED:
        {
            PRINT_ERROR_MESSAGE(9, STK_WRONG_MXBFFRSZ_DEFINED);
            stkdmp(Stk, __PRETTY_FUNCTION__, _fl, _fnc, _nln);
            TOGGLE_ABORT;
            break;
        }

        case STK_POP_RECIEVER_NULLPTR:
        {
            PRINT_ERROR_MESSAGE(10, STK_POP_RECIEVER_NULLPTR);
            stkdmp(Stk, __PRETTY_FUNCTION__, _fl, _fnc, _nln);
            TOGGLE_ABORT;
            break;
        }

        case STK_DATA_NO_READ_ACCES:
        {
            PRINT_ERROR_MESSAGE(11,STK_DATA_NO_READ_ACCES);
            stkdmp(Stk, __PRETTY_FUNCTION__, _fl, _fnc, _nln);
            TOGGLE_ABORT;
            break;
        }

        #ifndef STK_NOCHECK_STRUCT_HASH
        case STK_WRONG_STRUCT_HASH:
        {
            PRINT_ERROR_MESSAGE(11, STK_WRONG_STRUCT_HASH);
            stkdmp(Stk, __PRETTY_FUNCTION__, _fl, _fnc, _nln);
            TOGGLE_ABORT;
            break;
        }
        #endif

        #ifdef STK_CHECK_BUFFER_HASH
        case STK_WRONG_BUFFER_HASH:
        {
            PRINT_ERROR_MESSAGE(12, STK_WRONG_BUFFER_HASH);
            stkdmp(Stk, __PRETTY_FUNCTION__, _fl, _fnc, _nln);
            TOGGLE_ABORT;
            break;
        }
        #endif

        default: {}
    }

}

#undef PRINT_END_MESSAGE
#undef ERRLOG
#undef FRMT1
#undef FRMT2
#undef FRMT3
#undef FRMT4
#undef FRMT5
#undef FRMT6
#undef FRMT7
#undef FRMT8
#undef FRMT9
#undef FRMT10
#undef FRMT11
#ifndef STK_NOCHECK_STRUCT_HASH
#undef FRMT12
#ifdef STK_CHECK_BUFFER_HASH
#undef FRMT13
#endif
#endif
#undef ERRMSG1
#undef ERRMSG2
#undef ERRMSG3
#undef ERRMSG4
#undef ERRMSG5
#undef ERRMSG6
#undef ERRMSG7
#undef ERRMSG8
#undef ERRMSG9
#undef ERRMSG10
#undef ERRMSG11
#ifndef STK_NOCHECK_STRUCT_HASH
#undef ERRMSG12
#endif
#ifdef STK_CHECK_BUFFER_HASH
#undef ERRMSG13
#endif
#else
void stkerrhnd(stack_t* Stk, _errt err, str_t _prvfnc, str_t _fl, str_t _fnc, int _nln) {}
#endif
//YASHADOLBAEB

//-------------------------------------------------------------------------------------
int stkpush(stack_t* Stk, int var, str_t _fl, str_t _fnc, int _nln){

    STACK_OK

    if (Stk->cpty == Stk->sz) {

        stkgrow(Stk, __PRETTY_FUNCTION__, _fl, _fnc, _nln);
    }

    *(Stk->bffr + Stk->sz) = var;
    Stk->sz ++;

    REWRITE_STRUCT_HASH_VAL;
    REWRITE_BUFFER_HASH_VAL;

    return 0;
}


//-------------------------------------------------------------------------------------
int stkpop(stack_t* Stk, int* var, str_t _fl, str_t _fnc, int _nln){

    STACK_OK;

    if (var == NULL) {

        err = 10;
        ERROR_HANDLER_CALL
    }

    if (Stk->sz == 0) {

        err = 7;
        ERROR_HANDLER_CALL
    }

    Stk->sz --;
    *var = *(Stk->bffr + Stk->sz);

    REWRITE_STRUCT_HASH_VAL;
    REWRITE_BUFFER_HASH_VAL;

    if (4 * Stk->sz < Stk->cpty && Stk->cpty > 5){

        stkshrnk(Stk, __PRETTY_FUNCTION__, _fl, _fnc, _nln);
    }

    return 0;
}


//-------------------------------------------------------------------------------------
int stkgrow(stack_t* Stk, str_t _prvfnc, str_t _fl, str_t _fnc, int _nln) {

    PREVFUNC_ACCESS_OK;

    STACK_OK;

    stkelm_t* tmpbf = (stkelm_t*) realloc(Stk->bffr, 2 * (Stk->cpty) * sizeof((Stk->bffr)[0]));
    //tmpbf = NULL;
    if (tmpbf == NULL) {

        err = 6;
        ERROR_HANDLER_CALL
    }

    Stk->bffr = tmpbf;
    Stk->cpty *= 2;

    REWRITE_STRUCT_HASH_VAL;
    REWRITE_BUFFER_HASH_VAL;

    stkpzn(Stk, __PRETTY_FUNCTION__, _fl, _fnc, _nln);

    return 0;
}


//-------------------------------------------------------------------------------------
int stkshrnk(stack_t *Stk, str_t _prvfnc, str_t _fl, str_t _fnc, int _nln){

    PREVFUNC_ACCESS_OK

    STACK_OK

    stkelm_t* tmpbf = (stkelm_t*) realloc(Stk->bffr, (Stk->cpty / 4) * sizeof((Stk->bffr)[0]));

    //FOR ERR CHECK
    //tmpbf = NULL;
    if (tmpbf == NULL) {

        err = 6;
        ERROR_HANDLER_CALL
    }

    Stk->bffr = tmpbf;
    Stk->cpty /= 4;

    REWRITE_STRUCT_HASH_VAL;
    REWRITE_BUFFER_HASH_VAL;

    return 0;
}


//-------------------------------------------------------------------------------------
int stkctor(stack_t* Stk, str_t _nm, const size_t defcpty, str_t _fl, str_t _fnc, int _nln){

    // $;

    STACK_OK

    Stk->_tp = "int";
    Stk->_nm = _nm;
    Stk->_brnfl = _fl;
    Stk->_brnfnc = _fnc;
    Stk->_brnln = _nln;


    //printf("read: %zu, head: %zu", STK_MXBFFRSZ, STK_HEAD_MXBFFRSZ);
    if (STK_MXBFFRSZ > STK_HEAD_MXBFFRSZ) {

        err = STK_WRONG_MXBFFRSZ_DEFINED;
        ERROR_HANDLER_CALL
    }

    Stk->cpty = defcpty;

    //todo: не работает defcpty (-1u)

    if (defcpty > STK_MXBFFRSZ) {

        err = STK_DEFINED_CPTY_EXCESS;
        ERROR_HANDLER_CALL

    }


    stkelm_t* tmpbf = (stkelm_t*) calloc(defcpty, sizeof(stack_t));

    if (tmpbf == NULL) {

        err = 6;
        ERROR_HANDLER_CALL
    }

    Stk->bffr = tmpbf;

    Stk->sz = 0;

    REWRITE_STRUCT_HASH_VAL;
    REWRITE_BUFFER_HASH_VAL;

    stkpzn(Stk, __PRETTY_FUNCTION__, _fl, _fnc, _nln);

    return 0;
}


//-------------------------------------------------------------------------------------
int stkdtor(stack_t* Stk, str_t _fl, str_t _fnc, int _nln){

    STACK_OK

    free(Stk->bffr); //TODO IS_MALLOCED
    Stk->cpty =  -1u;
    Stk->sz = -1u;

    Stk->_brnfl = "DED_LOH";
    Stk->_brnfnc = "DED_LOH";
    Stk->_brnln = -1;

    #ifndef STK_NOCHECK_STRUCT_HASH
    Stk->_strcthsh = 0;
    #endif

    #ifndef STK_SANITIZE_LOUD
    fclose(logfl);
    #endif

    return 0;
}


//-------------------------------------------------------------------------------------
int stkpzn(stack_t* Stk, str_t _prvfnc, str_t _fl, str_t _fnc, int _nln) {
// $;
    PREVFUNC_ACCESS_OK;
//$;
    STACK_OK;

    for (size_t i = Stk->sz; i < Stk->cpty; i++){

        Stk->bffr [i] = PZN;
        }

    REWRITE_STRUCT_HASH_VAL;

    REWRITE_BUFFER_HASH_VAL;

    return 0;

}


//-------------------------------------------------------------------------------------
int stkhsh(void* ptr, size_t sz) {

    int hsh = 0;

    for (size_t i = 0; i < sz; i++){

        //printf("%d\n", *((char*) ptr + i));
        hsh += *((char*) ptr + i);
    }

    return hsh;
}



///
///
///
/// @return 0 if ptr non readeble, 1 if readblde
//-------------------------------------------------------------------------------------
bool isgoodreadptr(const void* memptr, size_t sz) {

    int fldes[2] = {};
    int err = pipe(fldes);

    if (err) {

        return 1; //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    }

    int res = (int) write(fldes[1], memptr, sz);

    close(fldes[0]);
    close(fldes[1]);

    return (res != -1);

}
