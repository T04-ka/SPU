#define STK_PTR_SANITIZE
#define STK_PTR_SANITIZE_LOUD
#define STK_PTR_HANDLER_ABORT



#ifndef STACK_PTR_H
#define STACK_PTR_H

#include "./../../myformat.cpp"

typedef const char* str_t;

#ifdef STK_PTR_EML_T
typedef STK_PTR_ELM_T stk_ptr_ptr_elm_t;
#else
typedef char* stk_ptr_ptr_elm_t;
#endif


struct stack_ptr_t {
    stk_ptr_ptr_elm_t* bffr;
    unsigned long cpty;
    unsigned long sz;

    str_t _tp;
    str_t _nm;
    str_t _brnfl;
    str_t _brnfnc;
    int _brnln;

    #ifndef STK_PTR_NOCHECK_STRUCT_HASH
    int _strcthsh;
    #endif

    #ifdef STK_PTR_CHECK_BUFFER_HASH
    int _bffrhsh;
    #endif
};


enum ptr_errt
{
    STK_PTR_OK                       = 0,
    STK_PTR_WRONG_PREVFUNC_ACCESS    = 1,
    STK_PTR_STRUCT_NULLPTR           = 2,
    STK_PTR_BUFFER_NULLPTR           = 3,
    STK_PTR_WRONG_CPTY               = 4,
    STK_PTR_WRONG_SZ                 = 5,
    STK_PTR_MALLOC_FAILED            = 6,
    STK_PTR_BUFFER_UNDERFLOW         = 7,
    STK_PTR_DEFINED_CPTY_EXCESS      = 8,
    STK_PTR_WRONG_MXBFFRSZ_DEFINED   = 9,
    STK_PTR_POP_RECIEVER_NULLPTR     = 10,
    STK_PTR_DATA_NO_READ_ACCES       = 11,

    #ifndef STK_PTR_NOCHECK_STRUCT_HASH
    STK_PTR_WRONG_STRUCT_HASH        = 12,
    #endif

    #ifdef STK_PTR_CHECK_BUFFER_HASH
    STK_PTR_WRONG_BUFFER_HASH        = 13,
    #endif
};


#ifdef STK_PTR_SANITIZE
#define ONDEBUG(...) __VA_ARGS__
#else
#define ONDEBUG(...)
#endif

#define STACK_PTR_DUMP(STKPTR)                      stk_ptr_dmp(STKPTR, __PRETTY_FUNCTION__ ONDEBUG(, __FILE__, __PRETTY_FUNCTION__, __LINE__))
#define STACK_PTR_CTOR(STKNM, DEFCPTY)              stk_ptr_ctor(&STKNM, #STKNM, DEFCPTY ONDEBUG(, __FILE__, __PRETTY_FUNCTION__, __LINE__))
#define STACK_PTR_PUSH(STKPTR, VAR)                 stk_ptr_push(STKPTR, VAR ONDEBUG(, __FILE__, __PRETTY_FUNCTION__, __LINE__))
#define STACK_PTR_POP(STKPTR, VARPTR)               stk_ptr_pop(STKPTR, VARPTR ONDEBUG(, __FILE__, __PRETTY_FUNCTION__, __LINE__))
#define STACK_PTR_DTOR(STKNM)                       stk_ptr_dtor(&STKNM ONDEBUG(, __FILE__, __PRETTY_FUNCTION__, __LINE__))


//-------------------------------------------------------------------------------------
int stk_ptr_dmp(stack_ptr_t* Stk, str_t _frmfnc, str_t _fl, str_t _fnc, int _nln);

//-------------------------------------------------------------------------------------
int stk_ptr_vrf(stack_ptr_t* Stk, str_t _prvfnc);

//-------------------------------------------------------------------------------------
void stk_ptr_errhnd(stack_ptr_t* Stk, ptr_errt err, str_t _prvfnc, str_t _fl, str_t _fnc, int _nln);

//-------------------------------------------------------------------------------------
int stk_ptr_push(stack_ptr_t* Stk, char* str_ptr, str_t _fl, str_t _frmfnc, int _nln);

//-------------------------------------------------------------------------------------
int stk_ptr_pop(stack_ptr_t* Stk, char** str_ptr_ptr, str_t _fl, str_t _frmfnc, int _nln);

//-------------------------------------------------------------------------------------
int stk_ptr_grow(stack_ptr_t* Stk, str_t _prvfnc, str_t _fl, str_t _frmfnc, int _nln) ;

//-------------------------------------------------------------------------------------
int stk_ptr_shrnk(stack_ptr_t *Stk, str_t _prvfnc, str_t _fl, str_t _frmfnc, int _nln);

//-------------------------------------------------------------------------------------
int stk_ptr_ctor(stack_ptr_t* Stk, str_t _nm, const unsigned long defcpty, str_t _fl, str_t _frmfnc, int _nln);

//-------------------------------------------------------------------------------------
int stk_ptr_dtor(stack_ptr_t* Stk, str_t _fl, str_t _frmfnc, int _nln);

//-------------------------------------------------------------------------------------
int stk_ptr_pzn(stack_ptr_t* Stk, str_t _prvfnc, str_t _fl, str_t _frmfnc, int _nln);

//-------------------------------------------------------------------------------------
int stk_ptr_hsh(void* ptr, unsigned long sz);

//-------------------------------------------------------------------------------------
bool ptr_isgoodreadptr(const void* memptr, unsigned long sz);

#endif
