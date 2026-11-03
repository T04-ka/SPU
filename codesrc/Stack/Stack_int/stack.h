#define STK_SANITIZE
#define STK_SANITIZE_LOUD
#define STK_HANDLER_ABORT

#ifndef STACK_H
#define STACK_H

#include "./../../myformat.cpp"

#ifdef STK_EML_T
typedef STK_ELM_T stkelm_t;
#else
typedef int stkelm_t;
#endif


struct stack_t {
    stkelm_t* bffr;
    unsigned long cpty;
    unsigned long sz;

    str_t _tp;
    str_t _nm;
    str_t _brnfl;
    str_t _brnfnc;
    int _brnln;

    #ifndef STK_NOCHECK_STRUCT_HASH
    int _strcthsh;
    #endif

    #ifdef STK_CHECK_BUFFER_HASH
    int _bffrhsh;
    #endif
};


enum _errt
{
    STK_OK                       = 0,
    STK_WRONG_PREVFUNC_ACCESS    = 1,
    STK_STRUCT_NULLPTR           = 2,
    STK_BUFFER_NULLPTR           = 3,
    STK_WRONG_CPTY               = 4,
    STK_WRONG_SZ                 = 5,
    STK_MALLOC_FAILED            = 6,
    STK_BUFFER_UNDERFLOW         = 7,
    STK_DEFINED_CPTY_EXCESS      = 8,
    STK_WRONG_MXBFFRSZ_DEFINED   = 9,
    STK_POP_RECIEVER_NULLPTR     = 10,
    STK_DATA_NO_READ_ACCES       = 11,

    #ifndef STK_NOCHECK_STRUCT_HASH
    STK_WRONG_STRUCT_HASH        = 12,
    #endif

    #ifdef STK_CHECK_BUFFER_HASH
    STK_WRONG_BUFFER_HASH        = 13,
    #endif
};


#ifdef STK_SANITIZE
#define ONDEBUG(...) __VA_ARGS__
#else
#define ONDEBUG(...)
#endif

#define STACK_DUMP(STKPTR)                      stkdmp(STKPTR, __PRETTY_FUNCTION__ ONDEBUG(, __FILE__, __PRETTY_FUNCTION__, __LINE__))
#define STACK_CTOR(STKNM, DEFCPTY)              stkctor(&STKNM, #STKNM, DEFCPTY ONDEBUG(, __FILE__, __PRETTY_FUNCTION__, __LINE__))
#define STACK_PUSH(STKPTR, VAR)                 stkpush(STKPTR, VAR ONDEBUG(, __FILE__, __PRETTY_FUNCTION__, __LINE__))
#define STACK_POP(STKPTR, VARPTR)               stkpop(STKPTR, VARPTR ONDEBUG(, __FILE__, __PRETTY_FUNCTION__, __LINE__))
#define STACK_DTOR(STKNM)                       stkdtor(&STKNM ONDEBUG(, __FILE__, __PRETTY_FUNCTION__, __LINE__))


//-------------------------------------------------------------------------------------
int stkdmp(stack_t* Stk, str_t _frmfnc, str_t _fl, str_t _fnc, int _nln);

//-------------------------------------------------------------------------------------
int stkvrf(stack_t* Stk, str_t _prvfnc);

//-------------------------------------------------------------------------------------
void stkerrhnd(stack_t* Stk, _errt err, str_t _prvfnc, str_t _fl, str_t _fnc, int _nln);

//-------------------------------------------------------------------------------------
int stkpush(stack_t* Stk, int var, str_t _fl, str_t _frmfnc, int _nln);

//-------------------------------------------------------------------------------------
int stkpop(stack_t* Stk, int* var, str_t _fl, str_t _frmfnc, int _nln);

//-------------------------------------------------------------------------------------
int stkgrow(stack_t* Stk, str_t _prvfnc, str_t _fl, str_t _frmfnc, int _nln) ;

//-------------------------------------------------------------------------------------
int stkshrnk(stack_t *Stk, str_t _prvfnc, str_t _fl, str_t _frmfnc, int _nln);

//-------------------------------------------------------------------------------------
int stkctor(stack_t* Stk, str_t _nm, const unsigned long defcpty, str_t _fl, str_t _frmfnc, int _nln);

//-------------------------------------------------------------------------------------
int stkdtor(stack_t* Stk, str_t _fl, str_t _frmfnc, int _nln);

//-------------------------------------------------------------------------------------
int stkpzn(stack_t* Stk, str_t _prvfnc, str_t _fl, str_t _frmfnc, int _nln);

//-------------------------------------------------------------------------------------
int stkhsh(void* ptr, unsigned long sz);

//-------------------------------------------------------------------------------------
bool isgoodreadptr(const void* memptr, unsigned long sz);

#endif
